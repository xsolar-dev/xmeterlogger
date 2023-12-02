/**
 * @file influxdb.c
 * @author longdh (longdh@xsolar.vn)
 * @brief 
 * @version 0.1
 * @date 2023-09-21
 * 
 * @copyright Copyright (c) 2023
 * 
 * Note: Update e-meter
 */


#include <stdio.h>
#include <unistd.h>
#include "influxdb_sink.h"
#include "cjson/cJSON.h"
#include "meter/datalog.h"
#include "meter/meter_data.h"
#include "utils/logger.h"
#include "utils/error.h"
#include "utils/message.h"

#define NNG
#define DEBUG

//FIXME
extern char* strdup(const char*);

#ifdef CURL
#include <curl/curl.h>

/**
 * @brief V1
 * 
 * @param data 
 */
static void sendDataToInfluxDB(influx_sink_config* cfg, const char* data) 
{
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, cfg->url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

/**
 * @brief send data to InfluxDB v2 using libcurl
 * 
 * @param data 
 */
static void sendDataToInfluxDBv2(influx_sink_config* cfg, const char* data) {
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: text/plain; charset=utf-8");

        char auth[512];
        snprintf(auth, sizeof(auth), "Authorization: Token %s", cfg->token);
        headers = curl_slist_append(headers, auth);

        // char url[256]; 
        // sprintf(url, INFLUXDB2_ADDRESS); // Modify URL parameters accordingly

        // curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_URL, cfg->url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}
#endif

#ifdef NNG

#include <nng/nng.h>
#include <nng/supplemental/http/http.h>

/**
 * @brief send data to InfluxDB v2 using libnng
 * 
 * @param data 
 */
static void sendDataToInfluxDBv2(influx_sink_config* cfg, const char* data, int datalen) 
{
    nng_http_client *client = NULL;
	nng_http_conn *  conn   = NULL;
	nng_url *        url    = NULL;
	nng_aio *        aio    = NULL;
	nng_http_req *   req    = NULL;
	nng_http_res *   res    = NULL;
	int              rv;

	if (((rv = nng_url_parse(&url, cfg->url)) != 0) ||
	    ((rv = nng_http_client_alloc(&client, url)) != 0) ||
	    ((rv = nng_http_req_alloc(&req, url)) != 0) ||
	    ((rv = nng_http_res_alloc(&res)) != 0) ||
	    ((rv = nng_aio_alloc(&aio, NULL, NULL)) != 0)) 
	{
		log_message(LOG_ERR, "init failed: %s\n", nng_strerror(rv));
		goto out;
	}

	// Start connection process...
	nng_aio_set_timeout(aio, 1000);
	nng_http_client_connect(client, aio);

	// Wait for it to finish.

	nng_aio_wait(aio);
	if ((rv = nng_aio_result(aio)) != 0) 
	{
		log_message(LOG_ERR, "Connect failed: %s", nng_strerror(rv));
		nng_aio_finish(aio, rv);

		goto out;
	}

	// Get the connection, at the 0th output.
	conn = nng_aio_get_output(aio, 0);

    char auth[512];
    snprintf(auth, sizeof(auth), "Token %s", cfg->token);

    nng_http_req_add_header(req, "Authorization", auth);
	
    

	nng_http_req_set_method(req, "POST");
	nng_http_req_set_data(req, data, datalen);
	nng_http_conn_write_req(conn, req, aio);
	nng_aio_set_timeout(aio, 1000);
	nng_aio_wait(aio);

	if ((rv = nng_aio_result(aio)) != 0) {
		log_message(LOG_ERR, "Write req failed: %s", nng_strerror(rv));
		nng_aio_finish(aio, rv);

		goto out;
	}

out:
	if (url) {
		nng_url_free(url);
	}
	if (conn) {
		nng_http_conn_close(conn);
	}
	if (client) {
		nng_http_client_free(client);
	}
	if (req) {
		nng_http_req_free(req);
	}
	if (res) {
		nng_http_res_free(res);
	}
	if (aio) {
		nng_aio_free(aio);
	}
}

#endif

// Function to convert meter_data_log to InfluxDB line protocol
static char* convert_to_influxdb_line(const char* measurement, struct meter_data_log* data) 
{
    // Create a buffer to hold the line protocol string
    char* line_protocol = (char*)malloc(256);  // Adjust size as needed

    // Assuming "meter_measurement" is the name of your InfluxDB measurement
    sprintf(line_protocol, "%s "
                            "voltage=%f,"
                            "current=%f,"
                            "power=%f,"
                            "reactive_power=%f,"
                            "power_factor=%f,"
                            "freq=%f,"
                            "import_active=%f,"
                            "export_active=%f",
            measurement, data->voltage, data->current, data->power, data->reactive_power,
            data->power_factor, data->freq, data->import_active, data->export_active);

    return line_protocol;
}

/**
 * @brief Influx Write function
 * 
 * @param arg 
 * @return void* 
 */
static void* influxdb_write_task(void* arg) 
{
    influx_sink_config* cfg = (influx_sink_config*) arg;

    // get queue 
    BusReader* br = cfg->br;

    while (1) 
    {        
        meter_data_log* mdl;
        void* data;
        int datalen;

        if (0 == bus_read(br, (void**) &data, &datalen))
        {
            if (datalen == sizeof(meter_data_log))
            {
                mdl = (meter_data_log*) data;
                
                char* inf_linedata = convert_to_influxdb_line(cfg->measurement, mdl);

                #ifdef DEBUG                
                log_message(LOG_INFO, "%s\n", (char*) inf_linedata);
                #endif // DEBUG

                sendDataToInfluxDBv2(cfg, inf_linedata, strlen(inf_linedata));
                free(inf_linedata);
            }

            if (data) free(data);
        }
    }
    
    return NULL;
}

/**
 * @brief Init Source task
 * 
 * @param cfg 
 * @param host 
 * @param port 
 * @param username 
 * @param password 
 * @return int 
 */
int influx_sink_init(influx_sink_config* cfg, Bus* b, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic)
{
    memset(cfg, 0, sizeof (influx_sink_config));

    cfg->b = b;
    create_bus_reader(&cfg->br, cfg->b);

    if (host != NULL)
        cfg->host = strdup(host);

    cfg->port = port;
    if (username != NULL)
        cfg->username = strdup(username);

    if (password != NULL)
        cfg->password = strdup(password);

    if (topic != NULL)
        cfg->topic = strdup(topic);

    if (client_id != NULL)
        cfg->client_id = strdup(client_id);
    else {
        char id[128];
        sprintf(id, "client-src-%lu\n", (unsigned long)time(NULL));

        cfg->client_id = strdup(id);
    } 

    return 0;
}

/**
 * @brief init for v2
 * 
 * @param cfg 
 * @param q 
 * @param url 
 * @param orgid 
 * @param token 
 * @return int 
 */
int influx_sink_init2(influx_sink_config* cfg, Bus* b, const char* url, const char* orgid, const char* token, const char* measurement)
{
    memset(cfg, 0, sizeof (influx_sink_config));

    cfg->b = b;
    create_bus_reader(&cfg->br, cfg->b);

    if (url != NULL)
        cfg->url = strdup(url);

    if (orgid != NULL)
        cfg->orgid = strdup(orgid);

    if (token != NULL)
        cfg->token = strdup(token);

    if (measurement != NULL)
        cfg->measurement = strdup(measurement);

    return 0;
}

/**
 * @brief Free task's data
 * 
 * @param cfg 
 * @return int 
 */
int influx_sink_term(influx_sink_config* cfg)
{
    if (cfg->host != NULL)
        free(cfg->host);

    if (cfg->username != NULL)
        free(cfg->username);

    if (cfg->password != NULL)
        free(cfg->password);
    
    if (cfg->client_id != NULL)
        free(cfg->client_id);

    if (cfg->topic != NULL)
        free(cfg->topic);

    return 0;
}

/**
 * @brief Do the task
 * 
 * @param cfg 
 * @return int 
 */
int influx_sink_run(influx_sink_config* cfg)
{   
    return 
        pthread_create(&cfg->task_thread, NULL, influxdb_write_task, cfg);
    
}

/**
 * @brief Wait until end
 * 
 * @param cfg 
 * @return int 
 */
int influx_sink_wait(influx_sink_config* cfg)
{
    return 
        pthread_join(cfg->task_thread, NULL);    
}
#include "data_forwarder.h"
#include "error.h"
#include "logger.h"
#include "configuration.h"
#include "sbus.h"

// queue 
Bus   df_bus;
#define URL "inproc://dfqueue"

#ifdef MOSQUITTO
#include "mosquitto.h"
#include "mosq_sink.h"
#include "mosq_src.h"

mosq_sync_config mosq_sink_conf;
int mosq_sink_task_init(config_t* cfg)
{
    // read config
    // Access the 'mosq-sink' subsetting
    config_setting_t* mosq_src = config_lookup(cfg, "mosq-sink");

    if (mosq_src != NULL) 
    {
        char* host = (char*)read_string_setting(mosq_src, "host", "103.161.39.186");
        int port = read_int_setting(mosq_src, "port", 1883);
        char* username = (char*)read_string_setting(mosq_src, "username", "lxdvinhnguyen01");
        char* password = (char*)read_string_setting(mosq_src, "password", "lxd@123");
        char* clientid = (char*)read_string_setting(mosq_src, "clientid", "sinktaskcli-01");
        char* topic = (char*)read_string_setting(mosq_src, "topic", "lxd/BA31605780");

        mosq_sink_init(&mosq_sink_conf, &df_bus, host, port, username, password, clientid, topic);
        mosq_sink_run(&mosq_sink_conf);

    } 
    else 
    {
        log_message(LOG_ERR, "The 'mosq-sink' subsetting is missing.\n");
    }   

    return 0;
}

int mosq_sink_task_cleanup()
{
    mosq_sink_wait(&mosq_sink_conf);
    mosq_sink_term(&mosq_sink_conf);

    return 0;
}

mosq_source_config mosq_source_conf;
int mosq_source_task_init(config_t* cfg)
{
    config_setting_t* mosq_src = config_lookup(cfg, "mosq-src");

    if (mosq_src != NULL) 
    {
        char* host = (char*)read_string_setting(mosq_src, "host", "192.168.31.166");
        int port = read_int_setting(mosq_src, "port", 1883);
        char* username = (char*)read_string_setting(mosq_src, "username", "lxdvinhnguyen01");
        char* password = (char*)read_string_setting(mosq_src, "password", "lxd@123");
        char* clientid = (char*)read_string_setting(mosq_src, "clientid", "sourcetaskcli-01");
        char* topic = (char*)read_string_setting(mosq_src, "topic", "lxp/BA31605780");

        // init queue
        init_bus(&df_bus, URL);

        mosq_source_init(&mosq_source_conf, &df_bus, host, port, username, password, clientid, topic);
        mosq_source_run(&mosq_source_conf);


        if (host != NULL) free(host);
        if (username != NULL) free(username);
        if (password != NULL) free(password);
        if (clientid != NULL) free(clientid);
        if (topic != NULL) free(topic);

    } else {
        log_message(LOG_ERR, "The 'mosq-src' subsetting is missing.\n");
    }   

    return 0;

}

/**
 * @brief Cleanup
 * 
 * @return int 
 */
int mosq_source_task_cleanup()
{
    mosq_source_wait(&mosq_source_conf);
    mosq_source_term(&mosq_source_conf);

    return 0;
}

#endif // MOSQUITTO


#ifdef INFLUXDB
#include "influxdb_sink.h"

influx_sink_config influx_sink_conf;
int influx_sink_task_init(config_t* cfg)
{
        // read config
    // Access the 'mqtt-sink' subsetting
    config_setting_t* influx_sink = config_lookup(cfg, "influx-sink");

    if (influx_sink != NULL) {
        char* url = (char*)read_string_setting(influx_sink, "url", "");
        char* orgid = (char*)read_string_setting(influx_sink, "orgid", "lxdvinhnguyen01");
        char* token = (char*)read_string_setting(influx_sink, "token", "lxd@123");
        
        printf("url: %s\n", url);
        printf("orgid: %s\n", orgid);
        printf("token: %s\n", token);
    
        influx_sink_init2(&influx_sink_conf, &df_bus, url, orgid, token);
        influx_sink_run(&influx_sink_conf);

    } else {
        fprintf(stderr, "The 'influx-sink' subsetting is missing.\n");
    }   

    return 0;
}

int influx_sink_task_cleanup()
{
    influx_sink_wait(&influx_sink_conf);
    influx_sink_term(&influx_sink_conf);

    return 0;
}
#endif

int data_forwarder_task_init(config_t* cfg)
{
    mosquitto_lib_init();
    
    // mqtt source task
    log_message(LOG_INFO, "Init MQTT source reader task\n");
    mosq_source_task_init(cfg);

     // mqtt target task
    log_message(LOG_INFO, "Init MQTT send task\n");
    mosq_sink_task_init(cfg);

    // influx
    log_message(LOG_INFO, "Init Influxdb send task\n");
    influx_sink_task_init(cfg);

    return ENOERR;
}

int data_forwarder_task_cleanup()
{
    mosq_source_task_cleanup();
    mosq_sink_task_cleanup();
    influx_sink_task_cleanup();

    return ENOERR;
}
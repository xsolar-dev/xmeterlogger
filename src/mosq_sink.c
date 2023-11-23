/**
 * @file mosq_sink.c
 * @author longdh (longdh@xsolar.vn)
 * @brief 
 * @version 0.1
 * @date 2027-09-20
 * 
 * @copyright Copyright (c) 2023
 * 
 * Note: chu y cho ID-client
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "mosquitto.h"
#include "datalog.h"
#include "error.h"
#include "logger.h"
#include "message.h"
#include "mosq_sink.h"

//FIXME
extern char* strdup(const char*);

#define DEBUG

volatile int _connected;

/**
 * @brief connect callback
 * 
 * @param mosq 
 * @param obj 
 * @param rc 
 */
static void on_connect(struct mosquitto *mosq, void *obj, int rc) 
{
    _connected = 1;

    if (rc == 0) 
    {
        log_message(LOG_INFO, "Sink: Connected successfully\n");        
    } 
    else 
    {
        log_message(LOG_ERR, "Connect failed: %s\n", mosquitto_strerror(rc));
    }
}

/**
 * @brief 
 * 
 * @param mosq 
 * @param obj 
 * @param rc 
 */
static void on_disconnect(struct mosquitto *mosq, void *obj, int rc) 
{
    _connected = 0;

    if (rc == MOSQ_ERR_SUCCESS) 
    {
        log_message(LOG_INFO, "Disconnecting gracefully...\n");
    } 
    else 
    {
        log_message(LOG_ERR, "Disconnected unexpectedly, will try to reconnect...\n");
        mosquitto_reconnect(mosq);
    }
}

/**
 * @brief send data to mqtt server
 * 
 * @param mosq 
 * @param topic 
 * @param message 
 * @param data_len 
 * @return int 
 */
static int send_to_mqtt(struct mosquitto *mosq, const char* topic, void* message, int data_len) 
{
    int rc = mosquitto_publish(mosq, NULL, topic, data_len, message, 0, false);
    
    if (rc != MOSQ_ERR_SUCCESS) 
    {
        log_message(LOG_ERR, "Unable to publish (%d): %s\n", rc, mosquitto_strerror(rc));
    }

    log_message(LOG_INFO, "send ok to server\n");

    return rc;
}

/**
 * @brief sink thread (forward thread)
 * 
 * @param arg 
 * @return void* 
 */
void* mosq_sink_task(void* arg) 
{
    mosq_sync_config* cfg= (mosq_sync_config*) arg;
    if (cfg == NULL)
    {
        #ifdef DEBUG
        log_message(LOG_ERR, "Error queue...\n");
        #endif // DEBUG
        
        exit(EQUERR);
    }

    // get queue 
    BusReader* br = cfg->br;
    if (br == NULL)
    {
        #ifdef DEBUG
        log_message(LOG_ERR, "Error queue...\n");
        #endif // DEBUG
        
        exit(EQUERR);
    }

    // Initialize mosquitto library
    mosquitto_lib_init();

    struct mosquitto *mosq = NULL;
    int rc;

    // Create a mosquitto client instance
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) 
    {
        log_message(LOG_ERR, "Error: Out of memory.\n");
        exit(ESYSERR);
    }

    // Set callback functions
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_disconnect_callback_set(mosq, on_disconnect);

    if (cfg->username != NULL)
        mosquitto_username_pw_set(mosq, cfg->username, cfg->password);

    _connected = 0;

    // Connect to MQTT broker
    rc = mosquitto_connect(mosq, cfg->host, cfg->port, 60);
    if (rc != MOSQ_ERR_SUCCESS) 
    {
        log_message(LOG_ERR, "Unable to connect (%d): %s\n", rc, mosquitto_strerror(rc));
        exit(ESVRERR);
    }

    rc = mosquitto_loop_start(mosq);
	if(rc != MOSQ_ERR_SUCCESS)
    {
		mosquitto_destroy(mosq);
		log_message(LOG_ERR, "Error: %s\n", mosquitto_strerror(rc));
		
        exit(ESVRERR);
	}

    // wait until connected
    while (!_connected)
        usleep(10000);

    while (1) 
    {
        struct Message* msg;
        void* data;
        int datalen;

        if (0 == bus_read(br, (void**) &data, &datalen))
        {
            if (datalen == sizeof(struct Message))
            {
                msg = (struct Message*) data;

                #ifdef DEBUG
                // log_message(LOG_INFO, "%s\n", msg->source_topic);
                // log_message(LOG_INFO, "%s\n", (char*) msg->data);
                #endif // DEBUG

                // Publish to MQTT broker and topic
                send_to_mqtt(mosq, msg->source_topic, msg->data, msg->datalen);

                // free
                free(data);
            }
        }
    }

    // Clean up
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();


    return NULL;
}

/**
 * @brief Init sync task
 * 
 * @param cfg 
 * @param host 
 * @param port 
 * @param username 
 * @param password 
 * @return int 
 */
int mosq_sink_init(mosq_sync_config* cfg, Bus* b, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic)
{
    memset(cfg, 0, sizeof (mosq_sync_config));

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
        sprintf(id, "cli_%lu\n", (unsigned long)time(NULL));

        cfg->client_id = strdup(id);
    } 

    return 0;
}

/**
 * @brief Free task's data
 * 
 * @param cfg 
 * @return int 
 */
int mosq_sink_term(mosq_sync_config* cfg)
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
int mosq_sink_run(mosq_sync_config* cfg)
{   
    return 
        pthread_create(&cfg->task_thread, NULL, mosq_sink_task, cfg);
    
}

/**
 * @brief Wait until end
 * 
 * @param cfg 
 * @return int 
 */
int mosq_sink_wait(mosq_sync_config* cfg)
{
    return 
        pthread_join(cfg->task_thread, NULL);    
}
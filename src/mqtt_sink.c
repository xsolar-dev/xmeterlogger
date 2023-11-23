/**
 * @file mqtt_sink.c
 * @author longdh (longdh@xsolar.vn)
 * @brief 
 * @version 0.1
 * @date 2023-09-20
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

#include "mqtt_sink.h"
#include "MQTTClient.h"
#include "error.h"
#include "logger.h"

//FIXME
extern char* strdup(const char*);

#define DEBUG

volatile int _connected = 0;
/**
 * @brief Connection Disconnect callback
 * 
 * @param context 
 * @param cause 
 */
static void connectionLost(void* context, char* cause) {
    #ifdef DEBUG
    log_message(LOG_INFO, "Connection lost, cause: %s\n", cause);
    #endif // DEBUG
    
    _connected = 0;
}

volatile MQTTClient_deliveryToken _deliveredtoken;
/**
 * @brief For qos 1
 * 
 * @param context 
 * @param dt 
 */
static void delivered(void* context, MQTTClient_deliveryToken dt) 
{
    #ifdef DEBUG
    log_message(LOG_INFO, "Message with token value %d delivery confirmed\n", dt);
    #endif // DEBUG   

    _deliveredtoken = dt;
}

/**
 * @brief sink thread (forward thread)
 * 
 * @param arg 
 * @return void* 
 */
void* mqtt_sink_task(void* arg) 
{
    mqtt_sync_config* cfg= (mqtt_sync_config*) arg;
    if (cfg == NULL)
    {
        #ifdef DEBUG
        printf("Error queue...\n");
        #endif // DEBUG
        
        exit(ESYSERR);
    }

    Queue* q = (Queue*) cfg->q;
    char data[MAX_QUEUE_DATA_SIZE];

    if (q == NULL)
    {
        #ifdef DEBUG
        printf("Error queue...\n");
        #endif // DEBUG
        
        exit(EQUERR);
    }

    char mqtt_addr[256];
    sprintf(mqtt_addr, "mqtt://%s:%d", cfg->host, cfg->port);
    #ifdef DEBUG
    printf("connect to %s\n", mqtt_addr);
    #endif // DEBUG

    while(1)
    {
        MQTTClient client;
        MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
        int rc;
        
        if (MQTTCLIENT_SUCCESS != (rc = MQTTClient_create(&client, mqtt_addr, cfg->client_id, MQTTCLIENT_PERSISTENCE_DEFAULT, NULL)))
        {
            log_message(LOG_ERROR, "Create ClientSink Error, code = %d\n", rc);
            exit(ESVRERR);
        }
            

        conn_opts.keepAliveInterval = 20;
        conn_opts.cleansession = 1;

        if (cfg->username != NULL)
        {
            conn_opts.username = cfg->username;
            conn_opts.password = cfg->password;
        }
        
        conn_opts.connectTimeout = 5; // 5 seconds
        // conn_opts.MQTTVersion = 3;

        MQTTClient_setCallbacks(client, NULL, connectionLost, NULL, NULL);


        if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) 
        {
            #ifdef DEBUG
            log_message(LOG_INFO, "Failed to connect to sink, return code %d, %s retrying ...\n", rc, MQTTClient_strerror(rc));
            #endif

            MQTTClient_destroy(&client);

            sleep(5);

            continue;
        }

        _connected = 1;

    
        while (_connected) 
        {
            if (dequeue(q, data))
            {
                #ifdef DEBUG
                log_message(LOG_INFO, "%s\n", data);
                #endif // DEBUG

                MQTTClient_message pubmsg = MQTTClient_message_initializer;
                MQTTClient_deliveryToken token;

                pubmsg.payload = data;
                pubmsg.payloadlen = strlen(data);
                pubmsg.qos = 0;
                pubmsg.retained = 0;

                MQTTClient_publishMessage(client, cfg->topic, &pubmsg, &token);
                // MQTTClient_waitForCompletion(client, token, 1000);
            }
            else
            {
                usleep(10000);
            }
        }

        // connect fail, reconnect
        MQTTClient_disconnect(client, 10000);
        MQTTClient_destroy(&client);
    }


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
int mqtt_sink_init(mqtt_sync_config* cfg, Queue* q, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic)
{
    memset(cfg, 0, sizeof (mqtt_sync_config));

    cfg->q = q;

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
int mqtt_sink_term(mqtt_sync_config* cfg)
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
int mqtt_sink_run(mqtt_sync_config* cfg)
{   
    return 
        pthread_create(&cfg->task_thread, NULL, mqtt_sink_task, cfg);
    
}

/**
 * @brief Wait until end
 * 
 * @param cfg 
 * @return int 
 */
int mqtt_sink_wait(mqtt_sync_config* cfg)
{
    return 
        pthread_join(cfg->task_thread, NULL);    
}
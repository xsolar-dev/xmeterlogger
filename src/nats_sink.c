/**
 * @file nats_sink.c
 * @author longdh
 * @brief 
 * @version 0.1
 * @date 2023-09-23
 * 
 * @copyright Copyright (c) 2023
 *  
 * TODO: support multiple nats servers & jetstream
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include <nats/nats.h>
#include "nats_sink.h"
#include "error.h"
#include "logger.h"

//FIXME
extern char* strdup(const char*);

#define DEBUG

/**
 * @brief sink thread (forward thread)
 * 
 * @param arg 
 * @return void* 
 */
void* nats_sink_task(void* arg) {

    nats_sync_config* cfg= (nats_sync_config*) arg;
    if (cfg == NULL)
    {
        #ifdef DEBUG
        log_message(LOG_ERR, "Error config system ...\n");
        #endif // DEBUG
        
        exit(EGENERR);
    }

    Queue* q = (Queue*) cfg->q;
    char data[MAX_QUEUE_DATA_SIZE];

    if (q == NULL)
    {
        #ifdef DEBUG
        log_message(LOG_ERR, "Error queue...\n");
        #endif // DEBUG
        
        exit(EQUERR);
    }

    // nats connect here
    while(1)
    {
        // nats connect
        natsConnection* nc = NULL;

        // url
        char url[128];
        sprintf(url, "nats://%s:%d", cfg->host, cfg->port);

        // Connect to NATS server
        natsStatus status = natsConnection_ConnectTo(&nc, url);
        if (status != NATS_OK) 
        {
            fprintf(stderr, "Failed to connect to NATS: %s\n", natsStatus_GetText(status));
            nats_Close();

            exit(ESVRERR);
        }

        while (1) 
        {
            if (dequeue(q, data))
            {
                #ifdef DEBUG
                printf("%s\n", data);
                #endif // DEBUG
        
                // Produce a message to nats
                const char *message = data;
                status = natsConnection_PublishString(nc, cfg->topic, message);

                if (status != NATS_OK)
                {
                    fprintf(stderr, "Error publishing message: %s\n", natsStatus_GetText(status));
                    natsConnection_Destroy(nc);
                    
                    break;
                }
            }
            else
            {
                usleep(10000);
            }
        }

        // Cleanup and destroy nats objects
        
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
int nats_sink_init(nats_sync_config* cfg, Queue* q, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic)
{
    memset(cfg, 0, sizeof (nats_sync_config));

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
int nats_sink_term(nats_sync_config* cfg)
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
int nats_sink_run(nats_sync_config* cfg)
{   
    return 
        pthread_create(&cfg->task_thread, NULL, nats_sink_task, cfg);
    
}

/**
 * @brief Wait until end
 * 
 * @param cfg 
 * @return int 
 */
int nats_sink_wait(nats_sync_config* cfg)
{
    return 
        pthread_join(cfg->task_thread, NULL);    
}
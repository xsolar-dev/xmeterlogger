/**
 * @file redis_sink.c
 * @author longdh (longdh@xsolar.vn)
 * @brief 
 * @version 0.1
 * @date 2023-09-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>

#include <hiredis/hiredis.h>
#include "redis_sink.h"
#include "error.h"
#include "logger.h"

//FIXME
extern char* strdup(const char*);

/**
 * @brief sink thread (forward thread)
 * 
 * @param arg 
 * @return void* 
 */
void* redis_sink_task(void* arg) 
{

    redis_sync_config* cfg= (redis_sync_config*) arg;
    if (cfg == NULL)
    {
        #ifdef DEBUG
        log_message(LOG_ERROR, "Config error ...\n");
        #endif // DEBUG
        
        exit(EGENERR);
    }

    // redis task
    Queue* q = (Queue*) cfg->q;
    char data[MAX_QUEUE_DATA_SIZE];

    if (q == NULL)
    {
        #ifdef DEBUG
        log_message(LOG_ERROR, "Error queue...\n");
        #endif // DEBUG
        
        exit(EQUERR);
    }
    
    while(1)
    {
        redisContext *ctx;
        redisReply *reply;

        // Connect to Redis
        ctx = redisConnect(cfg->host, cfg->port);
        if (ctx == NULL || ctx->err) 
        {
            if (ctx) 
            {
                log_message(LOG_INFO, "Connection error: %s\n", ctx->errstr);
                redisFree(ctx);
            } 
            else 
            {
                log_message(LOG_INFO, "Connection error: Can't allocate redis context\n");
            }

            sleep(1);
            continue;
        }

        // 2. get data from queue
        if (dequeue(q, data))
        {
            #ifdef DEBUG
            log_message(LOG_INFO, "%s\n", data);
            #endif // DEBUG

            // Perform Redis operation to set the key with binary data
            reply = redisCommand(ctx, "SET %s %s", cfg->key, data);

            if (reply == NULL) 
            {
                log_message(LOG_INFO, "Error: %s\n", ctx->errstr);
                redisFree(ctx);

                continue;
            }

            // Free the reply
            freeReplyObject(reply);
        }
        else
        {
            usleep(10000);
        }

    }
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
int redis_sink_init(redis_sync_config* cfg, Queue* q, const char* host, int port, const char* username, const char* password, const char* client_id, const char* key)
{
    memset(cfg, 0, sizeof (redis_sync_config));

    cfg->q = q;

    if (host != NULL)
        cfg->host = strdup(host);

    cfg->port = port;
    if (username != NULL)
        cfg->username = strdup(username);

    if (password != NULL)
        cfg->password = strdup(password);

    if (key != NULL)
        cfg->key = strdup(key);

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
int redis_sink_term(redis_sync_config* cfg)
{
    if (cfg->host != NULL)
        free(cfg->host);

    if (cfg->username != NULL)
        free(cfg->username);

    if (cfg->password != NULL)
        free(cfg->password);
    
    if (cfg->client_id != NULL)
        free(cfg->client_id);

    if (cfg->key != NULL)
        free(cfg->key);

    return 0;
}

/**
 * @brief Do the task
 * 
 * @param cfg 
 * @return int 
 */
int redis_sink_run(redis_sync_config* cfg)
{   
    return 
        pthread_create(&cfg->task_thread, NULL, redis_sink_task, cfg);
    
}

/**
 * @brief Wait until end
 * 
 * @param cfg 
 * @return int 
 */
int redis_sink_wait(redis_sync_config* cfg)
{
    return 
        pthread_join( cfg->task_thread, NULL);    
}
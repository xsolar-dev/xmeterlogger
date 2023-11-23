/**
 * @file kafka_sink.c
 * @author longdh
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

#include <librdkafka/rdkafka.h>
#include "kafka_sink.h"
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
void* kafka_sink_task(void* arg) {

    kafka_sync_config* cfg= (kafka_sync_config*) arg;
    if (cfg == NULL)
    {
        #ifdef DEBUG
        printf("Error queue...\n");
        #endif // DEBUG
        
        exit(EGENERR);
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

    // kafka connect here
    while(1)
    {
        rd_kafka_t *rk;
        rd_kafka_conf_t *conf;
        char errstr[512];
        char broker[128];

        sprintf(broker, "%s:%d", cfg->host, cfg->port);

        conf = rd_kafka_conf_new();
        if (rd_kafka_conf_set(conf, "bootstrap.servers", broker, errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
            fprintf(stderr, "Error configuring Kafka: %s\n", errstr);
            
            exit(ESVRERR);
        }

        // Create a Kafka producer instance
        rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
        if (!rk) 
        {
            log_message(LOG_ERROR, "Error creating Kafka producer: %s\n", errstr);
            exit(ESVRERR);
        }

        // Create a Kafka topic producer instance
        rd_kafka_topic_t *rkt;
        rkt = rd_kafka_topic_new(rk, cfg->topic, NULL);
        if (!rkt) 
        {
            log_message(LOG_ERROR, "Error creating topic object: %s\n", rd_kafka_err2str(rd_kafka_last_error()));
            rd_kafka_destroy(rk);

            exit(ESVRERR);
        }

        while (1) 
        {
            if (dequeue(q, data))
            {
                #ifdef DEBUG
                log_message(LOG_INFO, "%s\n", data);
                #endif // DEBUG
        
                // Produce a message to Kafka
                const char *message = data;
                size_t len = strlen(message);

                int partition = RD_KAFKA_PARTITION_UA; // Let Kafka choose the partition

                if (rd_kafka_produce(
                        rkt,         // Topic
                        partition,   // Partition (RD_KAFKA_PARTITION_UA for automatic)
                        RD_KAFKA_MSG_F_COPY, // Message flag (copies the payload)
                        (void *)message, len, // Message payload and length
                        NULL, 0,  // Optional key and key length (NULL for no key)
                        NULL      // Optional message opaque (used for callbacks, can be NULL)
                    ) == -1) 
                {
                    log_message(LOG_ERROR, "Error producing message: %s\n", rd_kafka_err2str(rd_kafka_last_error()));
                    break;
                } 
                else 
                {
                    log_message(LOG_ERROR, "Produced message: %s\n", message);
                    // Wait for any outstanding messages to be delivered and delivery reports to be received
                    rd_kafka_flush(rk, 2000);
                }
            }
            else
            {
                usleep(10000);
            }
        }

        // Cleanup and destroy Kafka objects
        rd_kafka_topic_destroy(rkt);
        rd_kafka_destroy(rk);
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
int kafka_sink_init(kafka_sync_config* cfg, Queue* q, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic)
{
    memset(cfg, 0, sizeof (kafka_sync_config));

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
int kafka_sink_term(kafka_sync_config* cfg)
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
int kafka_sink_run(kafka_sync_config* cfg)
{   
    return 
        pthread_create(&cfg->task_thread, NULL, kafka_sink_task, cfg);
    
}

/**
 * @brief Wait until end
 * 
 * @param cfg 
 * @return int 
 */
int kafka_sink_wait(kafka_sync_config* cfg)
{
    return 
        pthread_join(cfg->task_thread, NULL);    
}
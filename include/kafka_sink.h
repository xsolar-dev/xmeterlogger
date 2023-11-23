#ifndef __KAFKA_SINK_H__
#define __KAFKA_SINK_H__

#include "squeue.h"

typedef struct {
    char*   host;
    int     port;
    char*   username;
    char*   password;
    char*   topic;
    char*   client_id;

    Queue*  q;
    pthread_t task_thread;

} kafka_sync_config;

int kafka_sink_init(kafka_sync_config* cfg, Queue* q, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic);
int kafka_sink_term(kafka_sync_config* cfg);
int kafka_sink_run(kafka_sync_config* cfg);
int kafka_sink_wait(kafka_sync_config* cfg);

#endif
#ifndef REDIS_SINK_H
#define REDIS_SINK_H

#include "squeue.h"

typedef struct {
    char*   host;
    int     port;
    char*   username;
    char*   password;
    char*   key;
    char*   client_id;

    Queue*  q;
    pthread_t task_thread;

} redis_sync_config;

int redis_sink_init(redis_sync_config* cfg, Queue* q, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic);
int redis_sink_term(redis_sync_config* cfg);
int redis_sink_run(redis_sync_config* cfg);
int redis_sink_wait(redis_sync_config* cfg);

#endif
#ifndef __NATS_SINK_H__
#define __NATS_SINK_H__

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

} nats_sync_config;

int nats_sink_init(nats_sync_config* cfg, Queue* q, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic);
int nats_sink_term(nats_sync_config* cfg);
int nats_sink_run(nats_sync_config* cfg);
int nats_sink_wait(nats_sync_config* cfg);

#endif
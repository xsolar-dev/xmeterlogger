#ifndef __MOSQ_SINK_H__
#define __MOSQ_SINK_H__

#include <pthread.h>
#include "sbus.h"

typedef struct {
    char*   host;
    int     port;
    char*   username;
    char*   password;
    char*   topic;
    char*   client_id;

    Bus*        b;
    BusReader*  br;
    pthread_t task_thread;

} mosq_sync_config;

int mosq_sink_init(mosq_sync_config* cfg, Bus* b, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic);
int mosq_sink_term(mosq_sync_config* cfg);
int mosq_sink_run(mosq_sync_config* cfg);
int mosq_sink_wait(mosq_sync_config* cfg);


#endif
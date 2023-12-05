#ifndef __MQTTC_SINK_H__
#define __MQTTC_SINK_H__

#include "utils/squeue.h"
#include "utils/sbus.h"

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

} mqttc_sync_config;

int mqttc_sink_init(mqttc_sync_config* cfg, Bus* b, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic);
int mqttc_sink_term(mqttc_sync_config* cfg);
int mqttc_sink_run(mqttc_sync_config* cfg);
int mqttc_sink_wait(mqttc_sync_config* cfg);


#endif
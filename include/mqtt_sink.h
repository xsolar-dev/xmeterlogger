#ifndef __MQTT_SINK_H__
#define __MQTT_SINK_H__

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

} mqtt_sync_config;

int mqtt_sink_init(mqtt_sync_config* cfg, Queue* q, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic);
int mqtt_sink_term(mqtt_sync_config* cfg);
int mqtt_sink_run(mqtt_sync_config* cfg);
int mqtt_sink_wait(mqtt_sync_config* cfg);


#endif
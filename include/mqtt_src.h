#ifndef MQTT_SRC_H
#define MQTT_SRC_H 

#include "MQTTClient.h"
#include "squeue.h"

typedef struct {
    char*   host;
    int     port;
    char*   username;
    char*   password;
    char*   topic;
    char*   client_id;

    Queue *q;
    pthread_t task_thread;

} mqtt_source_config;

int mqtt_source_init(mqtt_source_config* cfg, Queue* q, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic);
int mqtt_source_term(mqtt_source_config* cfg);
int mqtt_source_run(mqtt_source_config* cfg);
int mqtt_source_wait(mqtt_source_config* cfg);

#endif // !MQTT_SRC_H

#ifndef MQTTC_SRC_H
#define MQTTC_SRC_H 

#include "utils/sbus.h"


typedef struct {
    char*   host;
    int     port;
    char*   username;
    char*   password;
    char*   topic;
    char*   client_id;

    // bus reader
    Bus*        b;
    BusReader*  br;

    pthread_t task_thread;

} mqttc_source_config;

int mqttc_source_init(mqttc_source_config* cfg, Bus* b, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic);
int mqttc_source_term(mqttc_source_config* cfg);
int mqttc_source_run(mqttc_source_config* cfg);
int mqttc_source_wait(mqttc_source_config* cfg);

#endif // !mqttc_SRC_H

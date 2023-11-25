#ifndef __INFLUXDB_H__
#define __INFLUXDB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


#include "sbus.h"

typedef struct {
    /* v1*/
    char*   host;
    int     port;
    char*   username;
    char*   password;
    char*   topic;
    char*   client_id;

    /*v2*/
    char*   url;
    char*   orgid;
    char*   token;

    /* table name */
    char*   measurement;


    Bus*        b;
    BusReader*  br;
    pthread_t task_thread;

} influx_sink_config;

int influx_sink_init(influx_sink_config* cfg, Bus* b, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic);
int influx_sink_init2(influx_sink_config* cfg, Bus* b, const char* url, const char* orgid, const char* token, const char* measurement);
int influx_sink_term(influx_sink_config* cfg);
int influx_sink_run(influx_sink_config* cfg);
int influx_sink_wait(influx_sink_config* cfg);

#endif // !__INFLUXDB_H__
#define __INFLUXDB_H__
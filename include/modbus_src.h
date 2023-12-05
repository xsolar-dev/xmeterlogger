#ifndef MODBUS_SRC_H
#define MODBUS_SRC_H 

#include <pthread.h>
#include "utils/sbus.h"
#include "meter/meter_data.h"

typedef struct {
    char*   mb_type; // RTU, TCP
    void*   mb_context;
    int     mtype;  // meter type

    // modbus TCP
    char*   host;
    int     port;

    // modbus RTU
    char*   path;
    int     baud;
    char    parity;
    int     data_bit;
    int     stop_bit;

    int     slave_id;

    // other
    Bus *b;
    BusWriter *bw;
    pthread_t task_thread;

} modbus_source_config;

int modbus_source_init(modbus_source_config* cfg, Bus *b, int mtype, const char* mbtype, const char* host, int port, const char* path, int baud, char parity, int data_bit, int stop_bit, int slave_id);
int modbus_source_term(modbus_source_config* cfg);
int modbus_source_run(modbus_source_config* cfg);
int modbus_source_wait(modbus_source_config* cfg);

#endif // !MODBUS_SRC_H

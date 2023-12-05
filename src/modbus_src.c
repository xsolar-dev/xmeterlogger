/**
 * @file modbus_src.c
 * @author longdh (longdh@xsolar.vn)
 * @brief 
 * @version 0.1
 * @date 2027-09-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <modbus/modbus.h>
#include "modbus_src.h"
#include "utils/error.h"
#include "utils/logger.h"
#include "utils/message.h"
#include "utils/sbus.h"
#include "meter/datalog.h"
#include "meter/meter_reader.h"

//FIXME
extern char* strdup(const char*);

#define DEBUG

/**
 * @brief Main thread
 * 
 * @param arg 
 * @return void* 
 */
static void* modbus_source_reader_task(void* arg) 
{
    modbus_source_config* cfg = (modbus_source_config*) arg;
    
    // modbus
    modbus_t *modbus_ctx = NULL;
    
    if (0 == strcmp(cfg->mb_type, "RTU"))
    {        
        modbus_ctx = modbus_new_rtu(cfg->path, cfg->baud, cfg->parity, cfg->data_bit, cfg->stop_bit);

        if (modbus_ctx == NULL) 
        {
            log_message(LOG_ERR, "Unable to create Modbus context\n");
            exit(EXIT_FAILURE);
        }

        modbus_set_slave(modbus_ctx, cfg->slave_id);

        if (modbus_connect(modbus_ctx) == -1) 
        {
            log_message(LOG_ERR, "Modbus connection failed: %s\n", modbus_strerror(errno));
            modbus_free(modbus_ctx);
            exit(EXIT_FAILURE);
        }

        // save context
        cfg->mb_context = (void*) modbus_ctx;   
    }

    // Main loop
    while (1) 
    {
        int rc;
        meter_data_log md_log;

        rc = read_meter_data(cfg->mtype, modbus_ctx, &md_log);

        if (rc == 0)
        {
            //write to queue
            bus_write(cfg->bw, (void*) &md_log, sizeof(md_log));
        }        
        

        // sleep
        usleep(1000000 * 10); // 10 second
    
    }

    // Clean up
    return NULL;
}

/**
 * @brief Init modbus master source
 * 
 * @param cfg 
 * @param b 
 * @param mb_type 
 * @param host 
 * @param port 
 * @param path 
 * @param baud 
 * @param parity 
 * @param data_bit 
 * @param stop_bit 
 * @param slave_id 
 * @return int 
 */
int modbus_source_init(modbus_source_config* cfg, Bus *b, int mtype, const char* mb_type, const char* host, int port, const char* path, int baud, char parity, int data_bit, int stop_bit, int slave_id)
{
    memset(cfg, 0, sizeof (modbus_source_config));

    cfg->b = b;
    create_bus_writer(&cfg->bw, cfg->b);

    cfg->mtype = mtype;

    if (mb_type != NULL)
        cfg->mb_type = strdup(mb_type);

    if (host != NULL)
        cfg->host = strdup(host);

    cfg->port = port;

    if (path != NULL)
        cfg->path = strdup(path);

    cfg->baud = baud;
    cfg->parity = parity;
    cfg->data_bit = data_bit;    
    cfg->stop_bit = stop_bit;
    cfg->slave_id = slave_id;

    return 0;
}

/**
 * @brief Free task's data
 * 
 * @param cfg 
 * @return int 
 */
int modbus_source_term(modbus_source_config* cfg)
{
    if (cfg->host != NULL)
        free(cfg->host);

    if (cfg->path != NULL)
        free(cfg->path);


    return 0;
}

/**
 * @brief Do the task
 * 
 * @param cfg 
 * @return int 
 */
int modbus_source_run(modbus_source_config* cfg)
{   
    return 
        pthread_create(&cfg->task_thread, NULL, modbus_source_reader_task, cfg);
    
}

/**
 * @brief Wait until end
 * 
 * @param cfg 
 * @return int 
 */
int modbus_source_wait(modbus_source_config* cfg)
{
    return 
        pthread_join(cfg->task_thread, NULL);    
}
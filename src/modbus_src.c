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
#include "error.h"
#include "logger.h"
#include "message.h"
#include "sbus.h"
#include "datalog.h"

//FIXME
extern char* strdup(const char*);

#define DEBUG

static float convert_2w_to_float(uint16_t* data)
{
    union
	{
		uint32_t j;
		float f;
	} u;
	
	u.j = ((uint32_t) data[0] << 16 | data[1]);
	return (float) u.f; 
}

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
        //  DSSU666
        const int base_regs = 9;
        const int base_addr = 0x2000;
        const int base_addr_impw = 0x4000;
        const int base_addr_expw = 0x4000;

        int rc;
        uint16_t chint_meter_regw[2];
        uint16_t chint_meter_data[2*base_regs];
        meter_data_log md_log;  
        
        rc = modbus_read_registers(modbus_ctx, base_addr, base_regs * 2, (uint16_t*) chint_meter_data);
        if (rc == -1) 
        {
            log_message(LOG_ERR, "Modbus read error: %s\n", modbus_strerror(errno));
            exit(EXIT_FAILURE);
        }

        md_log.voltage          = convert_2w_to_float((uint16_t *)&chint_meter_data[0x00]);
        md_log.current          = convert_2w_to_float((uint16_t *)&chint_meter_data[0x02]);
        md_log.power            = convert_2w_to_float((uint16_t *)&chint_meter_data[0x04]);
        md_log.reactive_power   = convert_2w_to_float((uint16_t *)&chint_meter_data[0x06]);
        md_log.power_factor     = convert_2w_to_float((uint16_t *)&chint_meter_data[0x0A]);
        md_log.freq             = convert_2w_to_float((uint16_t *)&chint_meter_data[0x0E]);

        rc = modbus_read_registers(modbus_ctx, base_addr_impw, 2, (uint16_t*) chint_meter_regw);
        if (rc == -1) 
        {
            log_message(LOG_ERR, "Modbus read error: %s\n", modbus_strerror(errno));
            exit(EXIT_FAILURE);
        }
        md_log.import_active = convert_2w_to_float(chint_meter_regw);

        rc = modbus_read_registers(modbus_ctx, base_addr_expw, 2, (uint16_t*) chint_meter_regw);
        if (rc == -1) 
        {
            log_message(LOG_ERR, "Modbus read error: %s\n", modbus_strerror(errno));
            exit(EXIT_FAILURE);
        }
        md_log.import_active = convert_2w_to_float(chint_meter_regw);
        
        //write to queue
        bus_write(cfg->bw, (void*) &md_log, sizeof(md_log));

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
int modbus_source_init(modbus_source_config* cfg, Bus *b, const char* mb_type, const char* host, int port, const char* path, int baud, char parity, int data_bit, int stop_bit, int slave_id)
{
    memset(cfg, 0, sizeof (modbus_source_config));

    cfg->b = b;
    create_bus_writer(&cfg->bw, cfg->b);

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
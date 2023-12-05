/**
 * @file meter_reader.c
 * @author longdh
 * @brief 
 * @version 0.1
 * @date 2023-12-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdlib.h>
#include <errno.h>
#include "meter/meter_reader.h"
#include "utils/error.h"
#include "utils/logger.h"

/**
 * @brief converter
 * 
 * @param data 
 * @return float 
 */
static float convert_2w_to_float(uint16_t* data)
{
    union
	{
		uint32_t j;
		float f;
	} u;
	
	u.j = ((uint32_t) data[0] << 16 | data[1]); // litle endian
    

	return (float) u.f; 
}

/**
 * @brief read meter data from modbus, dss666
 * 
 * @param modbus_ctx 
 * @param data 
 * @return int 
 */
int read_dss666_meter_data(modbus_t *modbus_ctx, meter_data_log* data)
{
    //  DSSU666
    const int base_regs = 9;
    const int base_addr = 0x2000;
    const int base_addr_impw = 0x4000;
    const int base_addr_expw = 0x400A;

    int rc;
    
    uint16_t chint_meter_regw[2];
    uint16_t chint_meter_data[2*base_regs];
    
    
    rc = modbus_read_registers(modbus_ctx, base_addr, base_regs * 2, (uint16_t*) chint_meter_data);
    if (rc == -1) 
    {
        log_message(LOG_ERR, "Modbus read error: %s\n", modbus_strerror(errno));
        exit(EXIT_FAILURE);
    }

    // data->voltage          = convert_2w_to_float((uint16_t *)&chint_meter_data[0x00]);
    // data->current          = convert_2w_to_float((uint16_t *)&chint_meter_data[0x02]);
    // data->power            = convert_2w_to_float((uint16_t *)&chint_meter_data[0x04]);
    // data->reactive_power   = convert_2w_to_float((uint16_t *)&chint_meter_data[0x06]);
    // data->power_factor     = convert_2w_to_float((uint16_t *)&chint_meter_data[0x0A]);
    // data->freq             = convert_2w_to_float((uint16_t *)&chint_meter_data[0x0E]);

    data->voltage          = modbus_get_float_dcba((uint16_t *)&chint_meter_data[0x00]);
    data->current          = modbus_get_float_dcba((uint16_t *)&chint_meter_data[0x02]);
    data->power            = modbus_get_float_dcba((uint16_t *)&chint_meter_data[0x04]);
    data->reactive_power   = modbus_get_float_dcba((uint16_t *)&chint_meter_data[0x06]);
    data->power_factor     = modbus_get_float_dcba((uint16_t *)&chint_meter_data[0x0A]);
    data->freq             = modbus_get_float_dcba((uint16_t *)&chint_meter_data[0x0E]);
    

    rc = modbus_read_registers(modbus_ctx, base_addr_impw, 2, (uint16_t*) chint_meter_regw);
    if (rc == -1) 
    {
        log_message(LOG_ERR, "Modbus read error: %s\n", modbus_strerror(errno));
        exit(EXIT_FAILURE);
    }
    data->import_active = modbus_get_float_dcba(chint_meter_regw);

    rc = modbus_read_registers(modbus_ctx, base_addr_expw, 2, (uint16_t*) chint_meter_regw);
    if (rc == -1) 
    {
        log_message(LOG_ERR, "Modbus read error: %s\n", modbus_strerror(errno));
        exit(EXIT_FAILURE);
    }

    data->export_active = modbus_get_float_dcba(chint_meter_regw);

    return 0;
}

// PZEM-16
int read_pzem16_meter_data(modbus_t *modbus_ctx, meter_data_log* data)
{
    //  PZEM16
    const int base_regs = 9;
    const int base_addr = 0x0;
    
    int rc;
    uint16_t chint_meter_data[2*base_regs];
    
    
    rc = modbus_read_registers(modbus_ctx, base_addr, base_regs, (uint16_t*) chint_meter_data);
    if (rc == -1) 
    {
        log_message(LOG_ERR, "Modbus read error: %s\n", modbus_strerror(errno));
        exit(EXIT_FAILURE);
    }

    data->voltage          = (float)chint_meter_data[0x00] * 0.1f;
    data->current          = convert_2w_to_float((uint16_t *)&chint_meter_data[0x01]) * 0.001f;
    data->power            = convert_2w_to_float((uint16_t *)&chint_meter_data[0x03]) * 0.1f;
    data->reactive_power   = 0;
    data->import_active    = convert_2w_to_float((uint16_t *)&chint_meter_data[0x05]); // total energy consumed
    data->freq             = (float)chint_meter_data[0x07] * 0.1f;
    data->power_factor     = (float)chint_meter_data[0x08] * 0.01f;

    return 0;
}

/**
 * @brief 
 * 
 * @param mtype 
 * @param modbus_ctx 
 * @param data 
 * @return int 
 */
int read_meter_data(enum METER_TYPE mtype, modbus_t *modbus_ctx, meter_data_log* data)
{
    if (mtype == DSSU666)
        return read_dss666_meter_data(modbus_ctx, data);

    if (mtype == PZEM16)
        return read_pzem16_meter_data(modbus_ctx, data);
}
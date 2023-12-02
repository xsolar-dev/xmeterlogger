#include <stdlib.h>
#include <errno.h>
#include <modbus/modbus.h>
#include "meter/meter_data.h"
#include "meter/meter_reader.h"
#include "utils/error.h"
#include "utils/logger.h"


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
 * @brief read meter data from modbus
 * 
 * @param modbus_ctx 
 * @param data 
 * @return int 
 */
int read_meter_data(modbus_t *modbus_ctx, meter_data_log* data)
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
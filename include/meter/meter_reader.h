#ifndef METER_READER_HH
#define METER_READER_HH

#include <modbus/modbus.h>
#include "meter/meter_data.h"

enum METER_TYPE {
    DSSU666 = 0,
    PZEM16
};

int read_meter_data(enum METER_TYPE mtype, modbus_t *modbus_ctx, meter_data_log* data);

#endif
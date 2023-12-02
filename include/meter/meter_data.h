#ifndef METER_DATA_H
#define METER_DATA_H

typedef struct meter_data_log 
{
    float voltage;
    float current;
    float power;
    float reactive_power;
    float power_factor;
    float freq;
    float import_active;
    float export_active;

} meter_data_log;

#endif // !METER_DATA_H


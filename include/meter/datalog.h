#ifndef __DATALOG_H__
#define __DATALOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <hashtable.h>
#include <list.h>

#ifndef cJSON_Bool
#define cJSON_Bool (cJSON_True | cJSON_False)
#endif
#ifndef cJSON_Map
#define cJSON_Map (1 << 16)
#endif
#ifndef cJSON_Enum
#define cJSON_Enum (1 << 17)
#endif

struct DataLog {
    int64_t * status;
    double * v_pv_1;
    int64_t * v_pv_2;
    double * v_pv_3;
    int64_t * v_bat;
    int64_t * soc;
    int64_t * soh;
    int64_t * p_pv;
    int64_t * p_pv_1;
    int64_t * p_pv_2;
    int64_t * p_pv_3;
    int64_t * p_charge;
    int64_t * p_discharge;
    double * v_ac_r;
    int64_t * v_ac_s;
    int64_t * v_ac_t;
    double * f_ac;
    int64_t * p_inv;
    int64_t * p_rec;
    int64_t * pf;
    int64_t * v_eps_r;
    double * v_eps_s;
    int64_t * v_eps_t;
    double * f_eps;
    int64_t * p_eps;
    int64_t * s_eps;
    int64_t * p_to_grid;
    int64_t * p_to_user;
    double * e_pv_day;
    double * e_pv_day_1;
    int64_t * e_pv_day_2;
    int64_t * e_pv_day_3;
    double * e_inv_day;
    int64_t * e_rec_day;
    int64_t * e_chg_day;
    int64_t * e_dischg_day;
    int64_t * e_eps_day;
    int64_t * e_to_grid_day;
    double * e_to_user_day;
    double * v_bus_1;
    double * v_bus_2;
    double * e_pv_all;
    double * e_pv_all_1;
    int64_t * e_pv_all_2;
    int64_t * e_pv_all_3;
    double * e_inv_all;
    int64_t * e_rec_all;
    int64_t * e_chg_all;
    int64_t * e_dischg_all;
    int64_t * e_eps_all;
    int64_t * e_to_grid_all;
    double * e_to_user_all;
    int64_t * t_inner;
    int64_t * t_rad_1;
    int64_t * t_rad_2;
    int64_t * t_bat;
    int64_t * runtime;
    int64_t * max_chg_curr;
    int64_t * max_dischg_curr;
    int64_t * charge_volt_ref;
    int64_t * dischg_cut_volt;
    int64_t * bat_status_0;
    int64_t * bat_status_1;
    int64_t * bat_status_2;
    int64_t * bat_status_3;
    int64_t * bat_status_4;
    int64_t * bat_status_5;
    int64_t * bat_status_6;
    int64_t * bat_status_7;
    int64_t * bat_status_8;
    int64_t * bat_status_9;
    int64_t * bat_status_inv;
    int64_t * bat_count;
    int64_t * bat_capacity;
    int64_t * bat_current;
    int64_t * bms_event_1;
    int64_t * bms_event_2;
    int64_t * max_cell_voltage;
    int64_t * min_cell_voltage;
    int64_t * max_cell_temp;
    int64_t * min_cell_temp;
    int64_t * bms_fw_update_state;
    int64_t * cycle_count;
    double * vbat_inv;
    int64_t * time;
    char * datalog;
};

struct DataLog * cJSON_ParseDataLog(const char * s);
struct DataLog * cJSON_GetDataLogValue(const cJSON * j);
cJSON * cJSON_CreateDataLog(const struct DataLog * x);
char * cJSON_PrintDataLog(const struct DataLog * x);
void cJSON_DeleteDataLog(struct DataLog * x);

#ifdef __cplusplus
}
#endif

#endif /* __STDOUT__ */


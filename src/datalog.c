#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "datalog.h"

//FIXME
extern char* strdup(const char*);

struct DataLog * cJSON_ParseDataLog(const char * s) {
    struct DataLog * x = NULL;
    if (NULL != s) {
        cJSON * j = cJSON_Parse(s);
        if (NULL != j) {
            x = cJSON_GetDataLogValue(j);
            cJSON_Delete(j);
        }
    }
    return x;
}

struct DataLog * cJSON_GetDataLogValue(const cJSON * j) {
    struct DataLog * x = NULL;
    if (NULL != j) {
        if (NULL != (x = cJSON_malloc(sizeof(struct DataLog)))) {
            memset(x, 0, sizeof(struct DataLog));
            if (cJSON_HasObjectItem(j, "status")) {
                if (NULL != (x->status = cJSON_malloc(sizeof(int64_t)))) {
                    *x->status = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "status"));
                }
            }
            if (cJSON_HasObjectItem(j, "v_pv_1")) {
                if (NULL != (x->v_pv_1 = cJSON_malloc(sizeof(double)))) {
                    *x->v_pv_1 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "v_pv_1"));
                }
            }
            if (cJSON_HasObjectItem(j, "v_pv_2")) {
                if (NULL != (x->v_pv_2 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->v_pv_2 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "v_pv_2"));
                }
            }
            if (cJSON_HasObjectItem(j, "v_pv_3")) {
                if (NULL != (x->v_pv_3 = cJSON_malloc(sizeof(double)))) {
                    *x->v_pv_3 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "v_pv_3"));
                }
            }
            if (cJSON_HasObjectItem(j, "v_bat")) {
                if (NULL != (x->v_bat = cJSON_malloc(sizeof(int64_t)))) {
                    *x->v_bat = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "v_bat"));
                }
            }
            if (cJSON_HasObjectItem(j, "soc")) {
                if (NULL != (x->soc = cJSON_malloc(sizeof(int64_t)))) {
                    *x->soc = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "soc"));
                }
            }
            if (cJSON_HasObjectItem(j, "soh")) {
                if (NULL != (x->soh = cJSON_malloc(sizeof(int64_t)))) {
                    *x->soh = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "soh"));
                }
            }
            if (cJSON_HasObjectItem(j, "p_pv")) {
                if (NULL != (x->p_pv = cJSON_malloc(sizeof(int64_t)))) {
                    *x->p_pv = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "p_pv"));
                }
            }
            if (cJSON_HasObjectItem(j, "p_pv_1")) {
                if (NULL != (x->p_pv_1 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->p_pv_1 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "p_pv_1"));
                }
            }
            if (cJSON_HasObjectItem(j, "p_pv_2")) {
                if (NULL != (x->p_pv_2 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->p_pv_2 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "p_pv_2"));
                }
            }
            if (cJSON_HasObjectItem(j, "p_pv_3")) {
                if (NULL != (x->p_pv_3 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->p_pv_3 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "p_pv_3"));
                }
            }
            if (cJSON_HasObjectItem(j, "p_charge")) {
                if (NULL != (x->p_charge = cJSON_malloc(sizeof(int64_t)))) {
                    *x->p_charge = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "p_charge"));
                }
            }
            if (cJSON_HasObjectItem(j, "p_discharge")) {
                if (NULL != (x->p_discharge = cJSON_malloc(sizeof(int64_t)))) {
                    *x->p_discharge = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "p_discharge"));
                }
            }
            if (cJSON_HasObjectItem(j, "v_ac_r")) {
                if (NULL != (x->v_ac_r = cJSON_malloc(sizeof(double)))) {
                    *x->v_ac_r = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "v_ac_r"));
                }
            }
            if (cJSON_HasObjectItem(j, "v_ac_s")) {
                if (NULL != (x->v_ac_s = cJSON_malloc(sizeof(int64_t)))) {
                    *x->v_ac_s = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "v_ac_s"));
                }
            }
            if (cJSON_HasObjectItem(j, "v_ac_t")) {
                if (NULL != (x->v_ac_t = cJSON_malloc(sizeof(int64_t)))) {
                    *x->v_ac_t = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "v_ac_t"));
                }
            }
            if (cJSON_HasObjectItem(j, "f_ac")) {
                if (NULL != (x->f_ac = cJSON_malloc(sizeof(double)))) {
                    *x->f_ac = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "f_ac"));
                }
            }
            if (cJSON_HasObjectItem(j, "p_inv")) {
                if (NULL != (x->p_inv = cJSON_malloc(sizeof(int64_t)))) {
                    *x->p_inv = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "p_inv"));
                }
            }
            if (cJSON_HasObjectItem(j, "p_rec")) {
                if (NULL != (x->p_rec = cJSON_malloc(sizeof(int64_t)))) {
                    *x->p_rec = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "p_rec"));
                }
            }
            if (cJSON_HasObjectItem(j, "pf")) {
                if (NULL != (x->pf = cJSON_malloc(sizeof(int64_t)))) {
                    *x->pf = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "pf"));
                }
            }
            if (cJSON_HasObjectItem(j, "v_eps_r")) {
                if (NULL != (x->v_eps_r = cJSON_malloc(sizeof(int64_t)))) {
                    *x->v_eps_r = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "v_eps_r"));
                }
            }
            if (cJSON_HasObjectItem(j, "v_eps_s")) {
                if (NULL != (x->v_eps_s = cJSON_malloc(sizeof(double)))) {
                    *x->v_eps_s = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "v_eps_s"));
                }
            }
            if (cJSON_HasObjectItem(j, "v_eps_t")) {
                if (NULL != (x->v_eps_t = cJSON_malloc(sizeof(int64_t)))) {
                    *x->v_eps_t = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "v_eps_t"));
                }
            }
            if (cJSON_HasObjectItem(j, "f_eps")) {
                if (NULL != (x->f_eps = cJSON_malloc(sizeof(double)))) {
                    *x->f_eps = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "f_eps"));
                }
            }
            if (cJSON_HasObjectItem(j, "p_eps")) {
                if (NULL != (x->p_eps = cJSON_malloc(sizeof(int64_t)))) {
                    *x->p_eps = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "p_eps"));
                }
            }
            if (cJSON_HasObjectItem(j, "s_eps")) {
                if (NULL != (x->s_eps = cJSON_malloc(sizeof(int64_t)))) {
                    *x->s_eps = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "s_eps"));
                }
            }
            if (cJSON_HasObjectItem(j, "p_to_grid")) {
                if (NULL != (x->p_to_grid = cJSON_malloc(sizeof(int64_t)))) {
                    *x->p_to_grid = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "p_to_grid"));
                }
            }
            if (cJSON_HasObjectItem(j, "p_to_user")) {
                if (NULL != (x->p_to_user = cJSON_malloc(sizeof(int64_t)))) {
                    *x->p_to_user = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "p_to_user"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_pv_day")) {
                if (NULL != (x->e_pv_day = cJSON_malloc(sizeof(double)))) {
                    *x->e_pv_day = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_pv_day"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_pv_day_1")) {
                if (NULL != (x->e_pv_day_1 = cJSON_malloc(sizeof(double)))) {
                    *x->e_pv_day_1 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_pv_day_1"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_pv_day_2")) {
                if (NULL != (x->e_pv_day_2 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->e_pv_day_2 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_pv_day_2"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_pv_day_3")) {
                if (NULL != (x->e_pv_day_3 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->e_pv_day_3 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_pv_day_3"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_inv_day")) {
                if (NULL != (x->e_inv_day = cJSON_malloc(sizeof(double)))) {
                    *x->e_inv_day = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_inv_day"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_rec_day")) {
                if (NULL != (x->e_rec_day = cJSON_malloc(sizeof(int64_t)))) {
                    *x->e_rec_day = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_rec_day"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_chg_day")) {
                if (NULL != (x->e_chg_day = cJSON_malloc(sizeof(int64_t)))) {
                    *x->e_chg_day = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_chg_day"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_dischg_day")) {
                if (NULL != (x->e_dischg_day = cJSON_malloc(sizeof(int64_t)))) {
                    *x->e_dischg_day = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_dischg_day"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_eps_day")) {
                if (NULL != (x->e_eps_day = cJSON_malloc(sizeof(int64_t)))) {
                    *x->e_eps_day = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_eps_day"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_to_grid_day")) {
                if (NULL != (x->e_to_grid_day = cJSON_malloc(sizeof(int64_t)))) {
                    *x->e_to_grid_day = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_to_grid_day"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_to_user_day")) {
                if (NULL != (x->e_to_user_day = cJSON_malloc(sizeof(double)))) {
                    *x->e_to_user_day = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_to_user_day"));
                }
            }
            if (cJSON_HasObjectItem(j, "v_bus_1")) {
                if (NULL != (x->v_bus_1 = cJSON_malloc(sizeof(double)))) {
                    *x->v_bus_1 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "v_bus_1"));
                }
            }
            if (cJSON_HasObjectItem(j, "v_bus_2")) {
                if (NULL != (x->v_bus_2 = cJSON_malloc(sizeof(double)))) {
                    *x->v_bus_2 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "v_bus_2"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_pv_all")) {
                if (NULL != (x->e_pv_all = cJSON_malloc(sizeof(double)))) {
                    *x->e_pv_all = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_pv_all"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_pv_all_1")) {
                if (NULL != (x->e_pv_all_1 = cJSON_malloc(sizeof(double)))) {
                    *x->e_pv_all_1 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_pv_all_1"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_pv_all_2")) {
                if (NULL != (x->e_pv_all_2 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->e_pv_all_2 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_pv_all_2"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_pv_all_3")) {
                if (NULL != (x->e_pv_all_3 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->e_pv_all_3 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_pv_all_3"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_inv_all")) {
                if (NULL != (x->e_inv_all = cJSON_malloc(sizeof(double)))) {
                    *x->e_inv_all = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_inv_all"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_rec_all")) {
                if (NULL != (x->e_rec_all = cJSON_malloc(sizeof(int64_t)))) {
                    *x->e_rec_all = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_rec_all"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_chg_all")) {
                if (NULL != (x->e_chg_all = cJSON_malloc(sizeof(int64_t)))) {
                    *x->e_chg_all = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_chg_all"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_dischg_all")) {
                if (NULL != (x->e_dischg_all = cJSON_malloc(sizeof(int64_t)))) {
                    *x->e_dischg_all = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_dischg_all"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_eps_all")) {
                if (NULL != (x->e_eps_all = cJSON_malloc(sizeof(int64_t)))) {
                    *x->e_eps_all = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_eps_all"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_to_grid_all")) {
                if (NULL != (x->e_to_grid_all = cJSON_malloc(sizeof(int64_t)))) {
                    *x->e_to_grid_all = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_to_grid_all"));
                }
            }
            if (cJSON_HasObjectItem(j, "e_to_user_all")) {
                if (NULL != (x->e_to_user_all = cJSON_malloc(sizeof(double)))) {
                    *x->e_to_user_all = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "e_to_user_all"));
                }
            }
            if (cJSON_HasObjectItem(j, "t_inner")) {
                if (NULL != (x->t_inner = cJSON_malloc(sizeof(int64_t)))) {
                    *x->t_inner = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "t_inner"));
                }
            }
            if (cJSON_HasObjectItem(j, "t_rad_1")) {
                if (NULL != (x->t_rad_1 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->t_rad_1 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "t_rad_1"));
                }
            }
            if (cJSON_HasObjectItem(j, "t_rad_2")) {
                if (NULL != (x->t_rad_2 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->t_rad_2 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "t_rad_2"));
                }
            }
            if (cJSON_HasObjectItem(j, "t_bat")) {
                if (NULL != (x->t_bat = cJSON_malloc(sizeof(int64_t)))) {
                    *x->t_bat = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "t_bat"));
                }
            }
            if (cJSON_HasObjectItem(j, "runtime")) {
                if (NULL != (x->runtime = cJSON_malloc(sizeof(int64_t)))) {
                    *x->runtime = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "runtime"));
                }
            }
            if (cJSON_HasObjectItem(j, "max_chg_curr")) {
                if (NULL != (x->max_chg_curr = cJSON_malloc(sizeof(int64_t)))) {
                    *x->max_chg_curr = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "max_chg_curr"));
                }
            }
            if (cJSON_HasObjectItem(j, "max_dischg_curr")) {
                if (NULL != (x->max_dischg_curr = cJSON_malloc(sizeof(int64_t)))) {
                    *x->max_dischg_curr = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "max_dischg_curr"));
                }
            }
            if (cJSON_HasObjectItem(j, "charge_volt_ref")) {
                if (NULL != (x->charge_volt_ref = cJSON_malloc(sizeof(int64_t)))) {
                    *x->charge_volt_ref = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "charge_volt_ref"));
                }
            }
            if (cJSON_HasObjectItem(j, "dischg_cut_volt")) {
                if (NULL != (x->dischg_cut_volt = cJSON_malloc(sizeof(int64_t)))) {
                    *x->dischg_cut_volt = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "dischg_cut_volt"));
                }
            }
            if (cJSON_HasObjectItem(j, "bat_status_0")) {
                if (NULL != (x->bat_status_0 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bat_status_0 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bat_status_0"));
                }
            }
            if (cJSON_HasObjectItem(j, "bat_status_1")) {
                if (NULL != (x->bat_status_1 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bat_status_1 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bat_status_1"));
                }
            }
            if (cJSON_HasObjectItem(j, "bat_status_2")) {
                if (NULL != (x->bat_status_2 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bat_status_2 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bat_status_2"));
                }
            }
            if (cJSON_HasObjectItem(j, "bat_status_3")) {
                if (NULL != (x->bat_status_3 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bat_status_3 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bat_status_3"));
                }
            }
            if (cJSON_HasObjectItem(j, "bat_status_4")) {
                if (NULL != (x->bat_status_4 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bat_status_4 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bat_status_4"));
                }
            }
            if (cJSON_HasObjectItem(j, "bat_status_5")) {
                if (NULL != (x->bat_status_5 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bat_status_5 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bat_status_5"));
                }
            }
            if (cJSON_HasObjectItem(j, "bat_status_6")) {
                if (NULL != (x->bat_status_6 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bat_status_6 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bat_status_6"));
                }
            }
            if (cJSON_HasObjectItem(j, "bat_status_7")) {
                if (NULL != (x->bat_status_7 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bat_status_7 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bat_status_7"));
                }
            }
            if (cJSON_HasObjectItem(j, "bat_status_8")) {
                if (NULL != (x->bat_status_8 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bat_status_8 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bat_status_8"));
                }
            }
            if (cJSON_HasObjectItem(j, "bat_status_9")) {
                if (NULL != (x->bat_status_9 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bat_status_9 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bat_status_9"));
                }
            }
            if (cJSON_HasObjectItem(j, "bat_status_inv")) {
                if (NULL != (x->bat_status_inv = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bat_status_inv = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bat_status_inv"));
                }
            }
            if (cJSON_HasObjectItem(j, "bat_count")) {
                if (NULL != (x->bat_count = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bat_count = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bat_count"));
                }
            }
            if (cJSON_HasObjectItem(j, "bat_capacity")) {
                if (NULL != (x->bat_capacity = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bat_capacity = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bat_capacity"));
                }
            }
            if (cJSON_HasObjectItem(j, "bat_current")) {
                if (NULL != (x->bat_current = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bat_current = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bat_current"));
                }
            }
            if (cJSON_HasObjectItem(j, "bms_event_1")) {
                if (NULL != (x->bms_event_1 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bms_event_1 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bms_event_1"));
                }
            }
            if (cJSON_HasObjectItem(j, "bms_event_2")) {
                if (NULL != (x->bms_event_2 = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bms_event_2 = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bms_event_2"));
                }
            }
            if (cJSON_HasObjectItem(j, "max_cell_voltage")) {
                if (NULL != (x->max_cell_voltage = cJSON_malloc(sizeof(int64_t)))) {
                    *x->max_cell_voltage = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "max_cell_voltage"));
                }
            }
            if (cJSON_HasObjectItem(j, "min_cell_voltage")) {
                if (NULL != (x->min_cell_voltage = cJSON_malloc(sizeof(int64_t)))) {
                    *x->min_cell_voltage = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "min_cell_voltage"));
                }
            }
            if (cJSON_HasObjectItem(j, "max_cell_temp")) {
                if (NULL != (x->max_cell_temp = cJSON_malloc(sizeof(int64_t)))) {
                    *x->max_cell_temp = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "max_cell_temp"));
                }
            }
            if (cJSON_HasObjectItem(j, "min_cell_temp")) {
                if (NULL != (x->min_cell_temp = cJSON_malloc(sizeof(int64_t)))) {
                    *x->min_cell_temp = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "min_cell_temp"));
                }
            }
            if (cJSON_HasObjectItem(j, "bms_fw_update_state")) {
                if (NULL != (x->bms_fw_update_state = cJSON_malloc(sizeof(int64_t)))) {
                    *x->bms_fw_update_state = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "bms_fw_update_state"));
                }
            }
            if (cJSON_HasObjectItem(j, "cycle_count")) {
                if (NULL != (x->cycle_count = cJSON_malloc(sizeof(int64_t)))) {
                    *x->cycle_count = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "cycle_count"));
                }
            }
            if (cJSON_HasObjectItem(j, "vbat_inv")) {
                if (NULL != (x->vbat_inv = cJSON_malloc(sizeof(double)))) {
                    *x->vbat_inv = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "vbat_inv"));
                }
            }
            if (cJSON_HasObjectItem(j, "time")) {
                if (NULL != (x->time = cJSON_malloc(sizeof(int64_t)))) {
                    *x->time = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(j, "time"));
                }
            }
            if (cJSON_HasObjectItem(j, "datalog")) {
                x->datalog = strdup(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(j, "datalog")));
            }
        }
    }
    return x;
}

cJSON * cJSON_CreateDataLog(const struct DataLog * x) {
    cJSON * j = NULL;
    if (NULL != x) {
        if (NULL != (j = cJSON_CreateObject())) {
            if (NULL != x->status) {
                cJSON_AddNumberToObject(j, "status", *x->status);
            }
            if (NULL != x->v_pv_1) {
                cJSON_AddNumberToObject(j, "v_pv_1", *x->v_pv_1);
            }
            if (NULL != x->v_pv_2) {
                cJSON_AddNumberToObject(j, "v_pv_2", *x->v_pv_2);
            }
            if (NULL != x->v_pv_3) {
                cJSON_AddNumberToObject(j, "v_pv_3", *x->v_pv_3);
            }
            if (NULL != x->v_bat) {
                cJSON_AddNumberToObject(j, "v_bat", *x->v_bat);
            }
            if (NULL != x->soc) {
                cJSON_AddNumberToObject(j, "soc", *x->soc);
            }
            if (NULL != x->soh) {
                cJSON_AddNumberToObject(j, "soh", *x->soh);
            }
            if (NULL != x->p_pv) {
                cJSON_AddNumberToObject(j, "p_pv", *x->p_pv);
            }
            if (NULL != x->p_pv_1) {
                cJSON_AddNumberToObject(j, "p_pv_1", *x->p_pv_1);
            }
            if (NULL != x->p_pv_2) {
                cJSON_AddNumberToObject(j, "p_pv_2", *x->p_pv_2);
            }
            if (NULL != x->p_pv_3) {
                cJSON_AddNumberToObject(j, "p_pv_3", *x->p_pv_3);
            }
            if (NULL != x->p_charge) {
                cJSON_AddNumberToObject(j, "p_charge", *x->p_charge);
            }
            if (NULL != x->p_discharge) {
                cJSON_AddNumberToObject(j, "p_discharge", *x->p_discharge);
            }
            if (NULL != x->v_ac_r) {
                cJSON_AddNumberToObject(j, "v_ac_r", *x->v_ac_r);
            }
            if (NULL != x->v_ac_s) {
                cJSON_AddNumberToObject(j, "v_ac_s", *x->v_ac_s);
            }
            if (NULL != x->v_ac_t) {
                cJSON_AddNumberToObject(j, "v_ac_t", *x->v_ac_t);
            }
            if (NULL != x->f_ac) {
                cJSON_AddNumberToObject(j, "f_ac", *x->f_ac);
            }
            if (NULL != x->p_inv) {
                cJSON_AddNumberToObject(j, "p_inv", *x->p_inv);
            }
            if (NULL != x->p_rec) {
                cJSON_AddNumberToObject(j, "p_rec", *x->p_rec);
            }
            if (NULL != x->pf) {
                cJSON_AddNumberToObject(j, "pf", *x->pf);
            }
            if (NULL != x->v_eps_r) {
                cJSON_AddNumberToObject(j, "v_eps_r", *x->v_eps_r);
            }
            if (NULL != x->v_eps_s) {
                cJSON_AddNumberToObject(j, "v_eps_s", *x->v_eps_s);
            }
            if (NULL != x->v_eps_t) {
                cJSON_AddNumberToObject(j, "v_eps_t", *x->v_eps_t);
            }
            if (NULL != x->f_eps) {
                cJSON_AddNumberToObject(j, "f_eps", *x->f_eps);
            }
            if (NULL != x->p_eps) {
                cJSON_AddNumberToObject(j, "p_eps", *x->p_eps);
            }
            if (NULL != x->s_eps) {
                cJSON_AddNumberToObject(j, "s_eps", *x->s_eps);
            }
            if (NULL != x->p_to_grid) {
                cJSON_AddNumberToObject(j, "p_to_grid", *x->p_to_grid);
            }
            if (NULL != x->p_to_user) {
                cJSON_AddNumberToObject(j, "p_to_user", *x->p_to_user);
            }
            if (NULL != x->e_pv_day) {
                cJSON_AddNumberToObject(j, "e_pv_day", *x->e_pv_day);
            }
            if (NULL != x->e_pv_day_1) {
                cJSON_AddNumberToObject(j, "e_pv_day_1", *x->e_pv_day_1);
            }
            if (NULL != x->e_pv_day_2) {
                cJSON_AddNumberToObject(j, "e_pv_day_2", *x->e_pv_day_2);
            }
            if (NULL != x->e_pv_day_3) {
                cJSON_AddNumberToObject(j, "e_pv_day_3", *x->e_pv_day_3);
            }
            if (NULL != x->e_inv_day) {
                cJSON_AddNumberToObject(j, "e_inv_day", *x->e_inv_day);
            }
            if (NULL != x->e_rec_day) {
                cJSON_AddNumberToObject(j, "e_rec_day", *x->e_rec_day);
            }
            if (NULL != x->e_chg_day) {
                cJSON_AddNumberToObject(j, "e_chg_day", *x->e_chg_day);
            }
            if (NULL != x->e_dischg_day) {
                cJSON_AddNumberToObject(j, "e_dischg_day", *x->e_dischg_day);
            }
            if (NULL != x->e_eps_day) {
                cJSON_AddNumberToObject(j, "e_eps_day", *x->e_eps_day);
            }
            if (NULL != x->e_to_grid_day) {
                cJSON_AddNumberToObject(j, "e_to_grid_day", *x->e_to_grid_day);
            }
            if (NULL != x->e_to_user_day) {
                cJSON_AddNumberToObject(j, "e_to_user_day", *x->e_to_user_day);
            }
            if (NULL != x->v_bus_1) {
                cJSON_AddNumberToObject(j, "v_bus_1", *x->v_bus_1);
            }
            if (NULL != x->v_bus_2) {
                cJSON_AddNumberToObject(j, "v_bus_2", *x->v_bus_2);
            }
            if (NULL != x->e_pv_all) {
                cJSON_AddNumberToObject(j, "e_pv_all", *x->e_pv_all);
            }
            if (NULL != x->e_pv_all_1) {
                cJSON_AddNumberToObject(j, "e_pv_all_1", *x->e_pv_all_1);
            }
            if (NULL != x->e_pv_all_2) {
                cJSON_AddNumberToObject(j, "e_pv_all_2", *x->e_pv_all_2);
            }
            if (NULL != x->e_pv_all_3) {
                cJSON_AddNumberToObject(j, "e_pv_all_3", *x->e_pv_all_3);
            }
            if (NULL != x->e_inv_all) {
                cJSON_AddNumberToObject(j, "e_inv_all", *x->e_inv_all);
            }
            if (NULL != x->e_rec_all) {
                cJSON_AddNumberToObject(j, "e_rec_all", *x->e_rec_all);
            }
            if (NULL != x->e_chg_all) {
                cJSON_AddNumberToObject(j, "e_chg_all", *x->e_chg_all);
            }
            if (NULL != x->e_dischg_all) {
                cJSON_AddNumberToObject(j, "e_dischg_all", *x->e_dischg_all);
            }
            if (NULL != x->e_eps_all) {
                cJSON_AddNumberToObject(j, "e_eps_all", *x->e_eps_all);
            }
            if (NULL != x->e_to_grid_all) {
                cJSON_AddNumberToObject(j, "e_to_grid_all", *x->e_to_grid_all);
            }
            if (NULL != x->e_to_user_all) {
                cJSON_AddNumberToObject(j, "e_to_user_all", *x->e_to_user_all);
            }
            if (NULL != x->t_inner) {
                cJSON_AddNumberToObject(j, "t_inner", *x->t_inner);
            }
            if (NULL != x->t_rad_1) {
                cJSON_AddNumberToObject(j, "t_rad_1", *x->t_rad_1);
            }
            if (NULL != x->t_rad_2) {
                cJSON_AddNumberToObject(j, "t_rad_2", *x->t_rad_2);
            }
            if (NULL != x->t_bat) {
                cJSON_AddNumberToObject(j, "t_bat", *x->t_bat);
            }
            if (NULL != x->runtime) {
                cJSON_AddNumberToObject(j, "runtime", *x->runtime);
            }
            if (NULL != x->max_chg_curr) {
                cJSON_AddNumberToObject(j, "max_chg_curr", *x->max_chg_curr);
            }
            if (NULL != x->max_dischg_curr) {
                cJSON_AddNumberToObject(j, "max_dischg_curr", *x->max_dischg_curr);
            }
            if (NULL != x->charge_volt_ref) {
                cJSON_AddNumberToObject(j, "charge_volt_ref", *x->charge_volt_ref);
            }
            if (NULL != x->dischg_cut_volt) {
                cJSON_AddNumberToObject(j, "dischg_cut_volt", *x->dischg_cut_volt);
            }
            if (NULL != x->bat_status_0) {
                cJSON_AddNumberToObject(j, "bat_status_0", *x->bat_status_0);
            }
            if (NULL != x->bat_status_1) {
                cJSON_AddNumberToObject(j, "bat_status_1", *x->bat_status_1);
            }
            if (NULL != x->bat_status_2) {
                cJSON_AddNumberToObject(j, "bat_status_2", *x->bat_status_2);
            }
            if (NULL != x->bat_status_3) {
                cJSON_AddNumberToObject(j, "bat_status_3", *x->bat_status_3);
            }
            if (NULL != x->bat_status_4) {
                cJSON_AddNumberToObject(j, "bat_status_4", *x->bat_status_4);
            }
            if (NULL != x->bat_status_5) {
                cJSON_AddNumberToObject(j, "bat_status_5", *x->bat_status_5);
            }
            if (NULL != x->bat_status_6) {
                cJSON_AddNumberToObject(j, "bat_status_6", *x->bat_status_6);
            }
            if (NULL != x->bat_status_7) {
                cJSON_AddNumberToObject(j, "bat_status_7", *x->bat_status_7);
            }
            if (NULL != x->bat_status_8) {
                cJSON_AddNumberToObject(j, "bat_status_8", *x->bat_status_8);
            }
            if (NULL != x->bat_status_9) {
                cJSON_AddNumberToObject(j, "bat_status_9", *x->bat_status_9);
            }
            if (NULL != x->bat_status_inv) {
                cJSON_AddNumberToObject(j, "bat_status_inv", *x->bat_status_inv);
            }
            if (NULL != x->bat_count) {
                cJSON_AddNumberToObject(j, "bat_count", *x->bat_count);
            }
            if (NULL != x->bat_capacity) {
                cJSON_AddNumberToObject(j, "bat_capacity", *x->bat_capacity);
            }
            if (NULL != x->bat_current) {
                cJSON_AddNumberToObject(j, "bat_current", *x->bat_current);
            }
            if (NULL != x->bms_event_1) {
                cJSON_AddNumberToObject(j, "bms_event_1", *x->bms_event_1);
            }
            if (NULL != x->bms_event_2) {
                cJSON_AddNumberToObject(j, "bms_event_2", *x->bms_event_2);
            }
            if (NULL != x->max_cell_voltage) {
                cJSON_AddNumberToObject(j, "max_cell_voltage", *x->max_cell_voltage);
            }
            if (NULL != x->min_cell_voltage) {
                cJSON_AddNumberToObject(j, "min_cell_voltage", *x->min_cell_voltage);
            }
            if (NULL != x->max_cell_temp) {
                cJSON_AddNumberToObject(j, "max_cell_temp", *x->max_cell_temp);
            }
            if (NULL != x->min_cell_temp) {
                cJSON_AddNumberToObject(j, "min_cell_temp", *x->min_cell_temp);
            }
            if (NULL != x->bms_fw_update_state) {
                cJSON_AddNumberToObject(j, "bms_fw_update_state", *x->bms_fw_update_state);
            }
            if (NULL != x->cycle_count) {
                cJSON_AddNumberToObject(j, "cycle_count", *x->cycle_count);
            }
            if (NULL != x->vbat_inv) {
                cJSON_AddNumberToObject(j, "vbat_inv", *x->vbat_inv);
            }
            if (NULL != x->time) {
                cJSON_AddNumberToObject(j, "time", *x->time);
            }
            if (NULL != x->datalog) {
                // cJSON_AddStringToObject(j, "datalog", x->datalog);
            }
        }
    }
    return j;
}

char * cJSON_PrintDataLog(const struct DataLog * x) {
    char * s = NULL;
    if (NULL != x) {
        cJSON * j = cJSON_CreateDataLog(x);
        if (NULL != j) {
            s = cJSON_Print(j);
            cJSON_Delete(j);
        }
    }
    return s;
}

void cJSON_DeleteDataLog(struct DataLog * x) {
    if (NULL != x) {
        if (NULL != x->status) {
            cJSON_free(x->status);
        }
        if (NULL != x->v_pv_1) {
            cJSON_free(x->v_pv_1);
        }
        if (NULL != x->v_pv_2) {
            cJSON_free(x->v_pv_2);
        }
        if (NULL != x->v_pv_3) {
            cJSON_free(x->v_pv_3);
        }
        if (NULL != x->v_bat) {
            cJSON_free(x->v_bat);
        }
        if (NULL != x->soc) {
            cJSON_free(x->soc);
        }
        if (NULL != x->soh) {
            cJSON_free(x->soh);
        }
        if (NULL != x->p_pv) {
            cJSON_free(x->p_pv);
        }
        if (NULL != x->p_pv_1) {
            cJSON_free(x->p_pv_1);
        }
        if (NULL != x->p_pv_2) {
            cJSON_free(x->p_pv_2);
        }
        if (NULL != x->p_pv_3) {
            cJSON_free(x->p_pv_3);
        }
        if (NULL != x->p_charge) {
            cJSON_free(x->p_charge);
        }
        if (NULL != x->p_discharge) {
            cJSON_free(x->p_discharge);
        }
        if (NULL != x->v_ac_r) {
            cJSON_free(x->v_ac_r);
        }
        if (NULL != x->v_ac_s) {
            cJSON_free(x->v_ac_s);
        }
        if (NULL != x->v_ac_t) {
            cJSON_free(x->v_ac_t);
        }
        if (NULL != x->f_ac) {
            cJSON_free(x->f_ac);
        }
        if (NULL != x->p_inv) {
            cJSON_free(x->p_inv);
        }
        if (NULL != x->p_rec) {
            cJSON_free(x->p_rec);
        }
        if (NULL != x->pf) {
            cJSON_free(x->pf);
        }
        if (NULL != x->v_eps_r) {
            cJSON_free(x->v_eps_r);
        }
        if (NULL != x->v_eps_s) {
            cJSON_free(x->v_eps_s);
        }
        if (NULL != x->v_eps_t) {
            cJSON_free(x->v_eps_t);
        }
        if (NULL != x->f_eps) {
            cJSON_free(x->f_eps);
        }
        if (NULL != x->p_eps) {
            cJSON_free(x->p_eps);
        }
        if (NULL != x->s_eps) {
            cJSON_free(x->s_eps);
        }
        if (NULL != x->p_to_grid) {
            cJSON_free(x->p_to_grid);
        }
        if (NULL != x->p_to_user) {
            cJSON_free(x->p_to_user);
        }
        if (NULL != x->e_pv_day) {
            cJSON_free(x->e_pv_day);
        }
        if (NULL != x->e_pv_day_1) {
            cJSON_free(x->e_pv_day_1);
        }
        if (NULL != x->e_pv_day_2) {
            cJSON_free(x->e_pv_day_2);
        }
        if (NULL != x->e_pv_day_3) {
            cJSON_free(x->e_pv_day_3);
        }
        if (NULL != x->e_inv_day) {
            cJSON_free(x->e_inv_day);
        }
        if (NULL != x->e_rec_day) {
            cJSON_free(x->e_rec_day);
        }
        if (NULL != x->e_chg_day) {
            cJSON_free(x->e_chg_day);
        }
        if (NULL != x->e_dischg_day) {
            cJSON_free(x->e_dischg_day);
        }
        if (NULL != x->e_eps_day) {
            cJSON_free(x->e_eps_day);
        }
        if (NULL != x->e_to_grid_day) {
            cJSON_free(x->e_to_grid_day);
        }
        if (NULL != x->e_to_user_day) {
            cJSON_free(x->e_to_user_day);
        }
        if (NULL != x->v_bus_1) {
            cJSON_free(x->v_bus_1);
        }
        if (NULL != x->v_bus_2) {
            cJSON_free(x->v_bus_2);
        }
        if (NULL != x->e_pv_all) {
            cJSON_free(x->e_pv_all);
        }
        if (NULL != x->e_pv_all_1) {
            cJSON_free(x->e_pv_all_1);
        }
        if (NULL != x->e_pv_all_2) {
            cJSON_free(x->e_pv_all_2);
        }
        if (NULL != x->e_pv_all_3) {
            cJSON_free(x->e_pv_all_3);
        }
        if (NULL != x->e_inv_all) {
            cJSON_free(x->e_inv_all);
        }
        if (NULL != x->e_rec_all) {
            cJSON_free(x->e_rec_all);
        }
        if (NULL != x->e_chg_all) {
            cJSON_free(x->e_chg_all);
        }
        if (NULL != x->e_dischg_all) {
            cJSON_free(x->e_dischg_all);
        }
        if (NULL != x->e_eps_all) {
            cJSON_free(x->e_eps_all);
        }
        if (NULL != x->e_to_grid_all) {
            cJSON_free(x->e_to_grid_all);
        }
        if (NULL != x->e_to_user_all) {
            cJSON_free(x->e_to_user_all);
        }
        if (NULL != x->t_inner) {
            cJSON_free(x->t_inner);
        }
        if (NULL != x->t_rad_1) {
            cJSON_free(x->t_rad_1);
        }
        if (NULL != x->t_rad_2) {
            cJSON_free(x->t_rad_2);
        }
        if (NULL != x->t_bat) {
            cJSON_free(x->t_bat);
        }
        if (NULL != x->runtime) {
            cJSON_free(x->runtime);
        }
        if (NULL != x->max_chg_curr) {
            cJSON_free(x->max_chg_curr);
        }
        if (NULL != x->max_dischg_curr) {
            cJSON_free(x->max_dischg_curr);
        }
        if (NULL != x->charge_volt_ref) {
            cJSON_free(x->charge_volt_ref);
        }
        if (NULL != x->dischg_cut_volt) {
            cJSON_free(x->dischg_cut_volt);
        }
        if (NULL != x->bat_status_0) {
            cJSON_free(x->bat_status_0);
        }
        if (NULL != x->bat_status_1) {
            cJSON_free(x->bat_status_1);
        }
        if (NULL != x->bat_status_2) {
            cJSON_free(x->bat_status_2);
        }
        if (NULL != x->bat_status_3) {
            cJSON_free(x->bat_status_3);
        }
        if (NULL != x->bat_status_4) {
            cJSON_free(x->bat_status_4);
        }
        if (NULL != x->bat_status_5) {
            cJSON_free(x->bat_status_5);
        }
        if (NULL != x->bat_status_6) {
            cJSON_free(x->bat_status_6);
        }
        if (NULL != x->bat_status_7) {
            cJSON_free(x->bat_status_7);
        }
        if (NULL != x->bat_status_8) {
            cJSON_free(x->bat_status_8);
        }
        if (NULL != x->bat_status_9) {
            cJSON_free(x->bat_status_9);
        }
        if (NULL != x->bat_status_inv) {
            cJSON_free(x->bat_status_inv);
        }
        if (NULL != x->bat_count) {
            cJSON_free(x->bat_count);
        }
        if (NULL != x->bat_capacity) {
            cJSON_free(x->bat_capacity);
        }
        if (NULL != x->bat_current) {
            cJSON_free(x->bat_current);
        }
        if (NULL != x->bms_event_1) {
            cJSON_free(x->bms_event_1);
        }
        if (NULL != x->bms_event_2) {
            cJSON_free(x->bms_event_2);
        }
        if (NULL != x->max_cell_voltage) {
            cJSON_free(x->max_cell_voltage);
        }
        if (NULL != x->min_cell_voltage) {
            cJSON_free(x->min_cell_voltage);
        }
        if (NULL != x->max_cell_temp) {
            cJSON_free(x->max_cell_temp);
        }
        if (NULL != x->min_cell_temp) {
            cJSON_free(x->min_cell_temp);
        }
        if (NULL != x->bms_fw_update_state) {
            cJSON_free(x->bms_fw_update_state);
        }
        if (NULL != x->cycle_count) {
            cJSON_free(x->cycle_count);
        }
        if (NULL != x->vbat_inv) {
            cJSON_free(x->vbat_inv);
        }
        if (NULL != x->time) {
            cJSON_free(x->time);
        }
        if (NULL != x->datalog) {
            //FIXME    
            cJSON_free(x->datalog);
            // free(x->datalog);
        }
        cJSON_free(x);
    }
}

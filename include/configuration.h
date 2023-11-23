#ifndef __CONFIG_MODULE__
#define __CONFIG_MODULE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libconfig.h>

char* read_string_setting(config_setting_t* cfg, const char* setting_name, const char* default_value);
int read_int_setting(config_setting_t* cfg, const char* setting_name, int default_value);

#endif // !__CONFIG_MODULE__


#include "configuration.h"
#include "error.h"

//FIXME
extern char* strdup(const char*);

// Function to read a string value from a config setting with a default value
char* read_string_setting(config_setting_t* cfg, const char* setting_name, const char* default_value) 
{
    const char* value = NULL;
    if (config_setting_lookup_string(cfg, setting_name, &value)) 
    { 
        return strdup(value);
    } 
    else 
    {
        return strdup(default_value);
    }

}

// Function to read an integer value from a config setting with a default value
int read_int_setting(config_setting_t* cfg, const char* setting_name, int default_value) 
{
    int value = default_value;
    config_setting_lookup_int(cfg, setting_name, &value);
    return value;
}
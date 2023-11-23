#ifndef DATA_FORWARDER_H__
#define DATA_FORWARDER_H__

#include <libconfig.h>

int data_forwarder_task_init(config_t* cfg);
int data_forwarder_task_cleanup();

#endif
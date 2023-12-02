#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct Message {
    char source_topic[128];
    char target_topic[128];
    
    char data[2048];
    int16_t datalen;

    int64_t time_arrived;
};

struct Message* create_message(const char* source_topic, const char* target_topic, void* data, int64_t datalen);
void free_message(struct Message* msg);



#ifdef __cplusplus
}
#endif

#endif
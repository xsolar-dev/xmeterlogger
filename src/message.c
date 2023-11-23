#include "message.h"
#include "error.h"


//FIXME
extern char* strdup(const char*);

/**
 * @brief Create a message object
 * 
 * @param source_topic 
 * @param target_topic 
 * @param data 
 * @param datalen 
 * @return struct Message* 
 */
struct Message* create_message(const char* source_topic, const char* target_topic, void* data, int64_t datalen)
{
    struct Message* msg = malloc(sizeof(struct Message));

    if (msg == NULL)
    {
        exit(ESYSERR);
    }

    memset(msg, 0, sizeof(struct Message));

    if (source_topic)
        strncpy(msg->source_topic, source_topic, sizeof (msg->source_topic));

    if (target_topic)
        strncpy(msg->target_topic, target_topic, sizeof(msg->target_topic));

    if (data && (datalen > 0))
    {        
        memcpy(msg->data, data, datalen);

        msg->datalen = datalen;
    }

    return msg;
}

/**
 * @brief 
 * 
 * @param msg 
 */
void free_message(struct Message* msg)
{
    if (msg)
    {
        // free pointer
        free(msg);
    }
}
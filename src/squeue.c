/**
 * @file squeue.c
 * @author longdh
 * @brief 
 * @version 0.1
 * @date 2023-10-18
 * 
 * @copyright Copyright (c) 2023
 * 
 * use nng as internal queue-system
 */
#include "squeue.h"
#include "datalog.h"
#include "logger.h"
#include "error.h"

#include <nng/nng.h>
#include <nng/protocol/bus0/bus.h>
#include <nng/protocol/reqrep0/req.h>
#include <nng/protocol/reqrep0/rep.h>

/**
 * @brief Internal Queue Data
 * 
 */
typedef struct {
    nng_socket queue_pub_socket; 
    nng_socket queue_sub_socket;   

} QueuePrivateData;

/**
 * @brief Init Queue, default URL as queue ID
 * 
 * @param q 
 */
void init_queue(Queue* q, const char* q_name) 
{
    QueuePrivateData* priv = (QueuePrivateData*) malloc (sizeof (QueuePrivateData));

    q->bus_address = strdup(q_name);

    if (nng_bus0_open(&priv->queue_pub_socket) != 0) 
    {
        log_message(LOG_ERR, "Error: nng_req0_open\n");
        exit(EQUERR);
    }

    if (nng_listen(priv->queue_pub_socket, q->bus_address, NULL, 0) != 0) 
    {
        log_message(LOG_ERR, "Error: nng_listen\n");
        nng_close(priv->queue_pub_socket);

        exit(EQUERR);
    }

    if (nng_bus0_open(&priv->queue_sub_socket) != 0) 
    {
        log_message(LOG_ERR, "Error: nng_rep0_open\n");

        exit(EQUERR);
    }

    if (nng_dial(priv->queue_sub_socket, q->bus_address, NULL, 0) != 0) 
    {
        log_message(LOG_ERR, "Error: nng_dial\n");
        nng_close(priv->queue_sub_socket);
        
        exit(EQUERR);
    }

    // set data to bus
    q->data = (void*) priv;
}

/**
 * @brief backward compat
 * 
 * @param q 
 * @return int 
 */
int is_empty(Queue* q) 
{
    return 0;
}

/**
 * @brief backward compat
 * 
 * @param q 
 * @return int 
 */
int is_full(Queue* q) {
    return 0;
}

/**
 * @brief Queue an Item
 * 
 * @param q 
 * @param data 
 */
void enqueue(Queue* q, void* data, int datalen) 
{
    nng_msg *msg = NULL;
    if (nng_msg_alloc(&msg, datalen) != 0) 
    {
        log_message(LOG_ERR, "Error: nng_msg_alloc\n");
        return;
    }

    memcpy(nng_msg_body(msg), data, datalen);

    QueuePrivateData* priv = (QueuePrivateData*) q->data;

    if (nng_sendmsg(priv->queue_pub_socket, msg, 0) != 0) 
    {
        log_message(LOG_ERR, "Error: nng_sendmsg\n");
        return;
    }

}

/**
 * @brief dequeue data
 * 
 * @param q 
 * @param data 
 * @return int 
 */
int dequeue(Queue* q, void** data, int* datalen) 
{
    nng_msg *msg = NULL;

    QueuePrivateData* priv = (QueuePrivateData*) q->data;

    if (nng_recvmsg(priv->queue_sub_socket, &msg, 0) != 0) 
    {
        log_message(LOG_ERR, "Error: nng_recvmsg\n");
        return 0;
    }

    if (nng_msg_len(msg) > 0) 
    {
        *datalen = nng_msg_len(msg);
        *data = (void*)nng_msg_body(msg);

        return 1;
    } 
    else 
    {
        log_message(LOG_ERR, "Received message with incorrect size\n");
        return 0;
    }
}

/**
 * @brief wait new message
 * 
 * @param q 
 * @param data 
 * @return int 
 */
int wait_dequeue(Queue* q, void** data, int* datalen) 
{
    return dequeue(q,data, datalen);
}

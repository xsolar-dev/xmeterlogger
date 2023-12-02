/**
 * @file sbus.c
 * @author longdh
 * @brief 
 * @version 0.1
 * @date 2023-10-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "utils/sbus.h"
#include "meter/datalog.h"
#include "utils/logger.h"
#include "utils/error.h"

#include <nng/nng.h>
#include <nng/protocol/bus0/bus.h>

//FIXME
extern char* strdup(const char*);

typedef struct {
    nng_socket bus_socket; 

} BusPrivateData;

int init_bus(Bus* b, const char* b_name)
{
    BusPrivateData* priv = (BusPrivateData*) malloc (sizeof (BusPrivateData));

    b->bus_address = strdup(b_name);

    if (nng_bus0_open(&priv->bus_socket) != 0) 
    {
        log_message(LOG_ERR, "Error: nng_req0_open\n");
        exit(EQUERR);
    }

    if (nng_listen(priv->bus_socket, b->bus_address, NULL, 0) != 0) 
    {
        log_message(LOG_ERR, "Error: nng_listen\n");
        nng_close(priv->bus_socket);

        exit(EQUERR);
    }
    b->data = (void*) priv;

    return ENOERR;
}

int create_bus_writer(BusWriter** bw, Bus* b)
{
    *bw = (BusWriter*) nng_alloc(sizeof(BusWriter));

    (*bw)->bus_address = nng_strdup(b->bus_address);
    (*bw)->data = b->data;

    return ENOERR;
}

int create_bus_reader(BusReader** br, Bus* b)
{
    *br = (BusReader*) nng_alloc(sizeof(BusReader));
    BusPrivateData* priv = (BusPrivateData*) malloc (sizeof (BusPrivateData));
    (*br)->bus_address = nng_strdup(b->bus_address);

    if (nng_bus0_open(&priv->bus_socket) != 0) 
    {
        log_message(LOG_ERR, "Error: nng_rep0_open\n");

        exit(EQUERR);
    }

    if (nng_dial(priv->bus_socket, (*br)->bus_address, NULL, 0) != 0) 
    {
        log_message(LOG_ERR, "Error: nng_dial\n");
        nng_close(priv->bus_socket);
        
        exit(EQUERR);
    }

    (*br)->data = (void*) priv;


    return ENOERR;
}

/**
 * @brief write data to bus
 * 
 * @param bw 
 * @param data 
 * @param datalen 
 * @return int, 0 => no error, 1-x: error code
 */
int bus_write(BusWriter* bw, void* data, int datalen)
{
    nng_msg *msg = NULL;
    if (nng_msg_alloc(&msg, datalen) != 0) 
    {
        log_message(LOG_ERR, "Error: nng_msg_alloc\n");
        return EQUERR;
    }

    memcpy(nng_msg_body(msg), data, datalen);

    BusPrivateData* priv = (BusPrivateData*) bw->data;

    if (nng_sendmsg(priv->bus_socket, msg, 0) != 0) 
    {
        log_message(LOG_ERR, "Error: nng_sendmsg\n");

        nng_msg_free(msg);

        return EQUERR;
    }

    //sendok: nng_msg_free(msg);
    
    return 0;
}

/**
 * @brief Read data from bus
 * 
 * @param br 
 * @param data 
 * @param datalen 
 * @return int 0=> no error, 1-x => msg error
 */
int bus_read(BusReader* br, void** data, int* datalen)
{
    nng_msg *msg = NULL;

    BusPrivateData* priv = (BusPrivateData*) br->data;

    if (nng_recvmsg(priv->bus_socket, &msg, 0) != 0) 
    {
        log_message(LOG_ERR, "Error: nng_recvmsg\n");
        return EQUERR;
    }

    int len = nng_msg_len(msg);
    if (len >= 0) 
    {
        *datalen = nng_msg_len(msg);
        *data = (void*) malloc(nng_msg_len(msg));
        
        if (*data == NULL)
        {
            log_message(LOG_ERR, "Error: nng_recvmsg\n");
            return EQUERR;
        }

        memcpy(*data, nng_msg_body(msg), nng_msg_len(msg));

        nng_msg_free(msg);

        return 0;
    } 
    else 
    {
        log_message(LOG_ERR, "Received message with incorrect size\n");

        nng_msg_free(msg);

        return EQUERR;
    }

}
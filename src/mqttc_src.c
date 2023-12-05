/**
 * @file mqttc_src.c
 * @author longdh (longdh@xsolar.vn)
 * @brief 
 * @version 0.1
 * @date 2023-09-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/syslog.h>

#include "mqttc_src.h"
#include "meter/datalog.h"
#include "utils/squeue.h"
#include "utils/error.h"
#include "utils/logger.h"

#include "nng/mqtt/mqtt_client.h"
#include "nng/nng.h"
#include "nng/supplemental/util/platform.h"

#define DEBUG

//FIXME
extern char* strdup(const char*);

static volatile int connected = 0;
static int keepRunning = 1;

static void
fatal(const char *msg, int rv)
{
#ifdef DEBUG
	fprintf(stderr, "%s: %s\n", msg, nng_strerror(rv));
#endif // DEBUG	

	log_message(LOG_ERR, "%s: %s\n", msg, nng_strerror(rv));
}

/**
 * @brief 
 * 
 * @param p 
 * @param ev 
 * @param arg 
 */
static void
disconnect_cb(nng_pipe p, nng_pipe_ev ev, void *arg)
{
#ifdef DEBUG
	int reason = 0;
	// get connect reason
	nng_pipe_get_int(p, NNG_OPT_MQTT_DISCONNECT_REASON, &reason);
	// property *prop;
	// nng_pipe_get_ptr(p, NNG_OPT_MQTT_DISCONNECT_PROPERTY, &prop);
	// nng_socket_get?
	printf("%s: disconnected!\n", __FUNCTION__);
#endif // DEBUG	

    connected = 0;
}

/**
 * @brief 
 * 
 * @param p 
 * @param ev 
 * @param arg 
 */
static void
connect_cb(nng_pipe p, nng_pipe_ev ev, void *arg)
{
#ifdef DEBUG
	int reason;
	// get connect reason
	nng_pipe_get_int(p, NNG_OPT_MQTT_CONNECT_REASON, &reason);
	// get property for MQTT V5
	// property *prop;
	// nng_pipe_get_ptr(p, NNG_OPT_MQTT_CONNECT_PROPERTY, &prop);
	printf("%s: connected!\n", __FUNCTION__);
#endif // DEBUG	


    connected = 1;
}

// Connect to the given address.
static int
client_connect(
    nng_socket *sock, nng_dialer *dialer, const char *url, const char* username, const char* password, bool verbose)
{
	int        rv;

	if ((rv = nng_mqtt_client_open(sock)) != 0) {
		fatal("nng_socket", rv);
	}

	if ((rv = nng_dialer_create(dialer, *sock, url)) != 0) {
		fatal("nng_dialer_create", rv);
	}

	// create a CONNECT message
	/* CONNECT */
	nng_msg *connmsg;
	nng_mqtt_msg_alloc(&connmsg, 0);
	nng_mqtt_msg_set_packet_type(connmsg, NNG_MQTT_CONNECT);
	nng_mqtt_msg_set_connect_proto_version(connmsg, 4);
	nng_mqtt_msg_set_connect_keep_alive(connmsg, 60);

    if (username)
    {
        nng_mqtt_msg_set_connect_user_name(connmsg, username);
        nng_mqtt_msg_set_connect_password(connmsg, password);
    }

	nng_mqtt_msg_set_connect_will_msg(
	    connmsg, (uint8_t *) "bye-bye", strlen("bye-bye"));
	nng_mqtt_msg_set_connect_will_topic(connmsg, "will_topic");
	nng_mqtt_msg_set_connect_clean_session(connmsg, true);

	nng_mqtt_set_connect_cb(*sock, connect_cb, sock);
	nng_mqtt_set_disconnect_cb(*sock, disconnect_cb, connmsg);

	uint8_t buff[1024] = { 0 };

#ifdef DEBUG
	if (verbose) {
		nng_mqtt_msg_dump(connmsg, buff, sizeof(buff), true);
		printf("%s\n", buff);
	}

	printf("Connecting to server ...\n");
#endif // DEBUG

	nng_dialer_set_ptr(*dialer, NNG_OPT_MQTT_CONNMSG, connmsg);
	nng_dialer_start(*dialer, NNG_FLAG_NONBLOCK);

	return (0);
}

/**
 * @brief Publish a message to the given topic and with the given QoS. 
 * 
 * @param sock 
 * @param topic 
 * @param payload 
 * @param payload_len 
 * @param qos 
 * @param verbose 
 * @return int 
 */
static int
client_publish(nng_socket sock, const char *topic, uint8_t *payload,
    uint32_t payload_len, uint8_t qos, bool verbose)
{
	int rv;

	// create a PUBLISH message
	nng_msg *pubmsg;
	nng_mqtt_msg_alloc(&pubmsg, 0);
	nng_mqtt_msg_set_packet_type(pubmsg, NNG_MQTT_PUBLISH);
	nng_mqtt_msg_set_publish_dup(pubmsg, 0);
	nng_mqtt_msg_set_publish_qos(pubmsg, qos);
	nng_mqtt_msg_set_publish_retain(pubmsg, 0);
	nng_mqtt_msg_set_publish_payload(
	    pubmsg, (uint8_t *) payload, payload_len);
	nng_mqtt_msg_set_publish_topic(pubmsg, topic);

#ifdef DEBUG
	if (verbose) {
		uint8_t print[1024] = { 0 };
		nng_mqtt_msg_dump(pubmsg, print, 1024, true);
		printf("%s\n", print);
	}
	printf("Publishing to '%s' ...\n", topic);
#endif // DEBUG

	if ((rv = nng_sendmsg(sock, pubmsg, NNG_FLAG_NONBLOCK)) != 0) {
		fatal("nng_sendmsg", rv);
	}

	return rv;
}

/**
 * @brief send callback
 * 
 * @param client 
 * @param msg 
 * @param arg 
 */
static void
send_callback (nng_mqtt_client *client, nng_msg *msg, void *arg) {
	nng_aio *        aio    = client->send_aio;
	uint32_t         count;
	uint8_t *        code;
	uint8_t          type;

	if (msg == NULL)
		return;
	
    switch (nng_mqtt_msg_get_packet_type(msg)) 
    {
	case NNG_MQTT_SUBACK:
		code = (uint8_t*)(reason_code *) nng_mqtt_msg_get_suback_return_codes(
		    msg, &count);
#ifdef DEBUG
		printf("SUBACK reason codes are");
		for (int i = 0; i < count; ++i)
			printf("%d ", code[i]);
		printf("\n");
#endif // DEBUG			
		break;

	case NNG_MQTT_UNSUBACK:
		code = (uint8_t*)(reason_code *) nng_mqtt_msg_get_unsuback_return_codes(
		    msg, &count);
#ifdef DEBUG
		printf("UNSUBACK reason codes are");
		for (int i = 0; i < count; ++i)
			printf("%d ", code[i]);
		printf("\n");
#endif // DEBUG
		break;

	case NNG_MQTT_PUBACK:
#ifdef DEBUG
		printf("PUBACK");
#endif // DEBUG
		break;

	default:
#ifdef DEBUG
		printf("Sending in async way is done.\n");
#endif // DEBUG
		break;
	}

	printf("aio mqtt result %d \n", nng_aio_result(aio));
	// printf("suback %d \n", *code);

	nng_msg_free(msg);
}


/**
 * @brief Main thread
 * 
 * @param arg 
 * @return void* 
 */
static void* mqttc_source_reader_task(void* arg) 
{
    mqttc_source_config* cfg = (mqttc_source_config*) arg;

    int rc;
    int rv;
    
	// get queue 
    BusReader* br = cfg->br;

    char mqttc_addr[256];
    sprintf(mqttc_addr, "mqtt-tcp://%s:%d", cfg->host, cfg->port);
    #ifdef DEBUG
    log_message(LOG_INFO, "connect to %s, client_id = %s\n", mqttc_addr, cfg->client_id); 
    #endif // DEBUG
 
    while (1) 
    {
        nng_socket sock;
        nng_dialer dailer;

        client_connect(&sock, &dailer, mqttc_addr, cfg->username, cfg->password, false);
        while (!connected) 
        {            
            nng_msleep(100); // Sleep for a short period to avoid busy-waiting
        }

        nng_mqtt_topic_qos subscriptions[] = {
            {
                .qos   = 0,
                .topic = { 
                    .buf    = (uint8_t *) cfg->topic,
                    .length = strlen(cfg->topic), 
                },
            },
		};

		nng_mqtt_topic unsubscriptions[] = {
			{
			    .buf    = (uint8_t *) cfg->topic,
			    .length = strlen(cfg->topic),
			},
		};

		// Sync subscription
		// rv = nng_mqtt_subscribe(&sock, subscriptions, 1, NULL);

		// Asynchronous subscription
		nng_mqtt_client *client = nng_mqtt_client_alloc(sock, &send_callback, true);
		nng_mqtt_subscribe_async(client, subscriptions, 1, NULL);


		uint8_t buff[1024*2] = { 0 };

		while (true) 
        {
			nng_msg *msg;
			uint8_t *payload;
			uint32_t payload_len;            
            const char* topic;
            uint32_t topic_len;
			
            if ((rv = nng_recvmsg(sock, &msg, 0)) != 0) 
            {
				fatal("nng_recvmsg", rv);
				continue;
			}

            topic = nng_mqtt_msg_get_publish_topic(msg, &topic_len);
			
			if (topic && (topic_len > 0))
            {
#ifdef DEBUG
                printf("topic: %s\n", topic);
#endif // DEBUG				
            }

			payload = nng_mqtt_msg_get_publish_payload(msg, &payload_len);

			//  check payload
			if (!payload)
				continue;

			#ifdef DEBUG 
            if (payload)
            {
                printf("recv: %s\n", (char*) payload);

            }			
            #endif // DEBUG     

			if (topic && payload)
			{
				// create packet
				// struct Message* msgp = create_message(topic, NULL, payload, payload_len);
				// if (msg) 
				// {
				// 	enqueue(q, (void*) msgp);
				// 	free((void*) msgp);
				// }
			}
            
			nng_msg_free(msg);
        }

    }

    return NULL;
}

/**
 * @brief Init Source task
 * 
 * @param cfg 
 * @param host 
 * @param port 
 * @param username 
 * @param password 
 * @return int 
 */
int mqttc_source_init(mqttc_source_config* cfg, Bus* b, const char* host, int port, const char* username, const char* password, const char* client_id, const char* topic)
{
    memset(cfg, 0, sizeof (mqttc_source_config));

    cfg->b = b;
	create_bus_reader(&cfg->br, cfg->b);

    if (host != NULL)
        cfg->host = strdup(host);

    cfg->port = port;
    if (username != NULL)
        cfg->username = strdup(username);

    if (password != NULL)
        cfg->password = strdup(password);

    if (topic != NULL)
        cfg->topic = strdup(topic);

    if (client_id != NULL)
        cfg->client_id = strdup(client_id);
    else {
        char id[128];
        memset(id, 0, sizeof(id));
        sprintf(id, "src_%lu\n", (unsigned long)time(NULL));

        cfg->client_id = strdup(id);
    } 

    return 0;
}

/**
 * @brief Free task's data
 * 
 * @param cfg 
 * @return int 
 */
int mqttc_source_term(mqttc_source_config* cfg)
{
    if (cfg->host != NULL)
        free(cfg->host);

    if (cfg->username != NULL)
        free(cfg->username);

    if (cfg->password != NULL)
        free(cfg->password);
    
    if (cfg->client_id != NULL)
        free(cfg->client_id);

    if (cfg->topic != NULL)
        free(cfg->topic);

    return 0;
}

/**
 * @brief Do the task
 * 
 * @param cfg 
 * @return int 
 */
int mqttc_source_run(mqttc_source_config* cfg)
{   
    return 
        pthread_create(&cfg->task_thread, NULL, mqttc_source_reader_task, cfg);
    
}

/**
 * @brief Wait until end
 * 
 * @param cfg 
 * @return int 
 */
int mqttc_source_wait(mqttc_source_config* cfg)
{
    return 
        pthread_join(cfg->task_thread, NULL);    
}
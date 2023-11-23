#ifndef __SQUEUE_H__
#define __SQUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_QUEUE_SIZE          100
#define MAX_QUEUE_DATA_SIZE     2048

typedef struct {
    char* bus_address; // in-memory address 
    void* data;
} Queue;

void init_queue(Queue* q, const char* q_name);
int is_empty(Queue* q);
int is_full(Queue* q);
void enqueue(Queue* q, void* data, int datalen);
int dequeue(Queue* q, void** data, int *datalen);
int wait_dequeue(Queue* q, void** data, int *datalen);

#endif

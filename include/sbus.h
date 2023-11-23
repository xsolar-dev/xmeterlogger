#ifndef __SBUS_H__
#define __SBUS_H__

typedef struct {
    char* bus_address; // in-memory address 
    void* data;
} Bus;

typedef struct {
    char* bus_address; // in-memory address 
    void* data;
} BusReader;

typedef struct {
    char* bus_address; // in-memory address 
    void* data;
} BusWriter;

int init_bus(Bus* b, const char* b_name);

int create_bus_writer(BusWriter** bw, Bus* b);
int create_bus_reader(BusReader** br, Bus* b);

int bus_write(BusWriter* bw, void* data, int datalen);
int bus_read(BusReader* bw, void** data, int* datalen);

#endif

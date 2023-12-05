# Minimum required version of CMake
cmake_minimum_required(VERSION 3.0)

option(CROSS_COMPILE_ARM64 "Cross compile for ARM64" OFF)
option(KAFKA "Kafka sink support" OFF)
option(REDIS "Redis sink support" OFF)
option(NATS "NATS sink support" OFF)
option(MOSQUITTO "mosquitto src-sink support" OFF)
option(PAHO "paho.mqtt.c src-sink support" OFF)
option(MQTTC "mqtt.c src-sink support" OFF)
option(INFLUXDB "influxdb sink support" ON)
option(MODBUSMS "modbus src support" ON)


if(CROSS_COMPILE_ARM64)
    message("Cross-compiling for ARM64")
    set(CMAKE_TOOLCHAIN_FILE "cmake/toolchain-arm64.cmake")
endif()


# Project name
project(xmeterlogger)

# Add the source and include directories
#add_subdirectory(src)
include_directories(include)

# List of source files
set(SOURCES 
    src/main.c
    src/datalog.c
    src/message.c
    src/logger.c
    src/squeue.c    
    src/sbus.c
    src/configuration.c
    src/list.c
    src/hashtable.c
    src/data_forwarder.c
    src/ng_http.c
    src/ng_mqtt.c
    src/meter_reader.c
)

# List of header files
set(HEADERS
    # include/datalog.h
    # include/logger.h
    # include/squeue.h
)

set(LINK_LIBRARIES
    cjson config pthread nng)

if(KAFKA)
    message("KAFKA flag is defined.")
    add_definitions(-DKAFKA)
    list(APPEND SOURCES src/kafka_sink.c)
    list(APPEND LINK_LIBRARIES rdkafka)
else()
    message("KAFKA flag is not defined!")
endif()

if(REDIS)
    message("REDIS flag is defined.")
    add_definitions(-DREDIS)
    list(APPEND SOURCES src/redis_sink.c)
    list(APPEND LINK_LIBRARIES hiredis)
else()
    message("REDIS flag is not defined!")
endif()

if(NATS)
    message("NATS flag is defined.")
    add_definitions(-DNATS)
    list(APPEND SOURCES src/nats_sink.c)
    list(APPEND LINK_LIBRARIES nats ssl crypto)
else()
    message("NATS flag is not defined!")
endif()

if(INFLUXDB)
    message("INFLUXDB flag is defined.")
    add_definitions(-DINFLUXDB)
    list(APPEND SOURCES src/influxdb_sink.c)
    list(APPEND LINK_LIBRARIES curl)
else()
    message("INFLUXDB flag is not defined!")
endif()

if(MOSQUITTO)
    message("MOSQUITTO flag is defined.")
    add_definitions(-DMOSQUITTO)
    list(APPEND SOURCES src/mosq_sink.c)
    list(APPEND SOURCES src/mosq_src.c)
    list(APPEND LINK_LIBRARIES mosquitto)
else()
    message("MOSQUITTO flag is not defined!")
endif()

if(PAHO)
    message("PAHO-mqtt flag is defined.")
    add_definitions(-DPAHO)
    list(APPEND SOURCES src/mqtt_sink.c)
    list(APPEND SOURCES src/mqtt_src.c)
    list(APPEND LINK_LIBRARIES paho-mqtt3c)
else()
    message("PAHO-mqtt flag is not defined!")
endif()

if(MQTTC)
    message("NNG-mqtt flag is defined.")
    add_definitions(-DMQTTC)
    list(APPEND SOURCES src/mqttc_sink.c)
    list(APPEND SOURCES src/mqttc_src.c)
#    list(APPEND SOURCES src/mqttc_bridge.c)
#    list(APPEND LINK_LIBRARIES mqttc)
else()
    message("NNG-MQTT flag is not defined!")
endif()


if(MODBUSMS)
    message("MODBUSMS flag is defined.")
    add_definitions(-DMODBUSMS)
    list(APPEND SOURCES src/modbus_src.c)
    list(APPEND LINK_LIBRARIES modbus)
else()
    message("MODBUSMS flag is not defined!")
endif()

add_subdirectory(NanoNNG)

# Create an executable from the sources
add_executable(xmeterlogger ${SOURCES} ${HEADERS})
target_link_libraries(xmeterlogger PUBLIC ${LINK_LIBRARIES})

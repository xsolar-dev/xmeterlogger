// logger.h

#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>
#include <syslog.h>

// typedef enum {
//     // LOG_INFO,
//     // LOG_WARNING,
//     // LOG_ERROR
// } LogLevel;

void init_logger(const char* logFilePath, int8_t use_syslog);
void cleanup_logger();
void log_message(int level, const char* message, ...);

#endif // LOGGER_H

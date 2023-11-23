// logger.c

#include "logger.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syslog.h>
#include <syslog.h>

static int8_t have_syslog = 0;

static FILE* log_file = NULL;
static pthread_mutex_t logLock = PTHREAD_MUTEX_INITIALIZER;

void init_logger(const char* logFilePath, int8_t use_syslog) 
{
    log_file = fopen(logFilePath, "w");
    if (!log_file) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }

    have_syslog = use_syslog;

    if (have_syslog)
    {
        openlog("xsolar", LOG_PID, LOG_USER);
    }
}

void cleanup_logger() 
{
    fclose(log_file);

    if (have_syslog)
    {
        closelog();
    }
}

void log_message(int level, const char* message, ...) 
{
    va_list args;
    va_start(args, message);

    pthread_mutex_lock(&logLock);

    switch (level) {
        case LOG_INFO:
            fprintf(stdout, "[INFO] ");
            vfprintf(stdout, message, args);

            if (have_syslog)
            {
                syslog(LOG_INFO, message, args);
            }

            break;

        case LOG_WARNING:
            fprintf(stdout, "[WARNING] ");
            vfprintf(stdout, message, args);
            if (have_syslog)
            {
                syslog(LOG_WARNING, message, args);
            }            
            break;

        case LOG_PERROR:
        case LOG_ERR:
            fprintf(stdout, "[ERROR] ");
            vfprintf(stdout, message, args);

            if (have_syslog)
            {
                syslog(LOG_PERROR, message, args);
            }
            break;
    }

    // FIXME
    // vfprintf(log_file, message, args);

    pthread_mutex_unlock(&logLock);

    va_end(args);
}

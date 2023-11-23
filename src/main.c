/**
 * @file main.c
 * @author longdh (longdh@xsolar.vn)
 * @brief 
 * @version 0.1
 * @date 2023-09-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>

#include <libconfig.h>

#include "error.h"
#include "configuration.h"

#include "cjson/cJSON.h"
#include "logger.h"
#include "squeue.h"
#include "datalog.h"
#include "data_forwarder.h"


static void daemonize();

// libconfig
config_t cfg;
 

int main(int argc, char* argv[]) {
    int daemonize_flag = 0;
    char *config_file = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "dc:")) != -1) {
        switch (opt) {
            case 'd':
                daemonize_flag = 1;
                break;
            case 'c':
                config_file = optarg;
                break;
            default:
                printf("Usage: %s [-d] -c config_file\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (config_file == NULL) {
        printf("Error: Missing config file. Use -c option to specify a config file.\n");
        config_file = "../etc/dfconfig.cfg";
    }

    config_init(&cfg);

    if (!config_read_file(&cfg, config_file)) {
        fprintf(stderr, "Error reading configuration file: %s\n", config_error_text(&cfg));
        config_destroy(&cfg);

        return 1;
    }

    if (daemonize_flag) {
        daemonize();
    }

    init_logger("logfile.log", 0);
    data_forwarder_task_init(&cfg);


    while (1) {
        sleep(1);
    }

    cleanup_logger();
    config_destroy(&cfg);
    data_forwarder_task_cleanup();

    return 0;
}

// go background
void daemonize() {
    pid_t pid, sid;

    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <unistd.h>

typedef struct {
    int trigger_time;
    int func_id;
    void **params;
} DecodedRPC;

#define INIT_I2C 0
#define DEINIT_I2C 1
#define RECEIVE_I2C 2
#define TRANSMIT_I2C 3

typedef struct _scheduler {
    DecodedRPC **list_of_tasks;
    int nr_tasks;
    void (*check)(scheduler *s);
    int (*add)(scheduler *s, DecodedRPC task);
    void (*_run_task)(scheduler *s, DecodedRPC *task);

} scheduler;

#endif
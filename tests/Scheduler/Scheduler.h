#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <unistd.h>
#include <stdint.h>

#define MAX_SIZE 100

typedef struct {
    int trigger_time;
    void(*func)(void);
    void *params[5];
}DecodedRPC;

typedef struct  Scheduler Scheduler;

struct Scheduler{
    int start;
    int end;
    DecodedRPC list_of_tasks[MAX_SIZE];

    void (*init)(Scheduler *s);
    int (*enqueue)(Scheduler *queue, DecodedRPC *task);
    DecodedRPC* (*dequeue)(Scheduler *s);
    void (*check)(Scheduler *s);
    void (*_run_task)(Scheduler *s, DecodedRPC *task);
};

extern Scheduler default_scheduler;

#endif
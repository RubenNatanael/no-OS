#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Scheduler.h"
#include "capi_i2c.h"

static int _enqueue(Scheduler *s, DecodedRPC* task);
static DecodedRPC* _dequeue(Scheduler *s);
static void _init_queue(Scheduler *s);
static void run_task(Scheduler *s, DecodedRPC *task);
static void check_trigger(Scheduler *s);

Scheduler default_scheduler = {
    .init = _init_queue
};

static void _init_queue(Scheduler *s) {
    s->start = 0;
    s->end = 0;
    s->init = &_init_queue;
    s->enqueue = &_enqueue;
    s->dequeue = &_dequeue;
    s->_run_task = &run_task;
    s->check = &check_trigger;
}

static int isEmpty(Scheduler *s)
{
    return (s->start == s->end);
}

static int isFull(Scheduler *s)
{
    return ((s->end + 1) % MAX_SIZE == s->start);
}

static void increase_index(int *index) {
    if (*index == MAX_SIZE - 1) {
        *index = 0;
        return;
    }
    (*index)++;
}

static int _enqueue(Scheduler *s, DecodedRPC* task) {
    if (s == NULL)
        return -1;
    if (isFull(s))
        return -1;

    s->list_of_tasks[s->end] = *task;

    increase_index(&s->end);
    return 0;
}

static DecodedRPC* _dequeue(Scheduler *s) {
    if (s == NULL || isEmpty(s))
        return NULL;

    DecodedRPC *result = &s->list_of_tasks[s->start];
    increase_index(&s->start);

    return result;
}

static void run_task(Scheduler *s, DecodedRPC *task) {
    printf("Runing task ");
    if (task->func == (void(*)(void))capi_i2c_init) {
        printf("init\n");
        // TODO
    } else if (task->func == (void(*)(void))capi_i2c_deinit) {
        printf("deinit\n");
        // TODO
    } else if (task->func == (void(*)(void))capi_i2c_receive) {
        printf("receive\n");
        capi_i2c_receive((struct capi_i2c_device*)(task->params[0]), (struct capi_i2c_transfer*)(task->params[1]));
        free(task->params[0]);
        free(task->params[1]);
    } else if (task->func == (void(*)(void))capi_i2c_transmit) {
        printf("transmit\n");
        capi_i2c_transmit((struct capi_i2c_device*)(task->params[0]), (struct capi_i2c_transfer*)(task->params[1]));
        free(task->params[0]);
        free(task->params[1]);
    }
}

int current_time = 100;
static void check_trigger(Scheduler *s) {
    int index = s->start;
    while(index != s->end) {
        if (s->list_of_tasks[index].trigger_time == current_time) {
            s->_run_task(s, s->dequeue(s));
        }
        index++;
    }
}
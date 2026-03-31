#include "Scheduler.h"

int current_time = 100;

int add_task (scheduler *s, DecodedRPC task) {
    DecodedRPC *new_task = malloc(sizeof(DecodedRPC));
    if (!new_task) {
        return -1;
    }
    *new_task = task;
    int index_found = -1;
    for (int i = 0; i < s->nr_tasks; i++) {
        if (new_task->trigger_time < s->list_of_tasks[i]->trigger_time) {
            index_found = i;
            break;
        }
    }
    index_found = (index_found != -1) ? index_found : s->nr_tasks;

    for (int i = s->nr_tasks; i > index_found; i--) {
        s->list_of_tasks[i] = s->list_of_tasks[i - 1];
    }
    s->list_of_tasks[index_found] = new_task;
    s->nr_tasks++;
}


void check_trigger(scheduler *s) {
    int index = 0;
    while (s->list_of_tasks[index] >= current_time)
        s->_run_task(s, s->list_of_tasks[0]);
}

void run_task(DecodedRPC rpc) {
    switch(rpc.func_id) {
        case INIT_I2C:
            // TODO
            break;
        case DEINIT_I2C:
            // TODO
            break;
        case RECEIVE_I2C:
            capi_i2c_receive((struct capi_i2c_device*)(&rpc.params[0]),
             (struct capi_i2c_transfer*)(&rpc.params[1]));
            break;
        case TRANSMIT_I2C:
            capi_i2c_transmit((struct capi_i2c_device*)(&rpc.params[0]),
             (struct capi_i2c_transfer*)(&rpc.params[1]));
            break;
        default:
            break;
    }
}
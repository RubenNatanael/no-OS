#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "capi_scheduler.h"
#include "capi_i2c.h"

static int add_params(void (*fun)(void), void *params[], va_list args) {
    if (fun == (void (*)(void))capi_i2c_init) {
        // TODO
    } else if (fun == (void (*)(void))capi_i2c_deinit) {
        struct capi_i2c_controller_handle *handle = va_arg(args, struct capi_i2c_controller_handle *);

        params[0] = malloc(sizeof(struct capi_i2c_controller_handle));
        memcpy(params[0], handle, sizeof(struct capi_i2c_controller_handle));
    } else if (fun == (void (*)(void))capi_i2c_receive || fun == (void (*)(void))capi_i2c_transmit) {
        struct capi_i2c_device *device = va_arg(args, struct capi_i2c_device *);
        struct capi_i2c_transfer *transfer = va_arg(args, struct capi_i2c_transfer *);

        params[0] = malloc(sizeof(struct capi_i2c_device));
        params[1] = malloc(sizeof(struct capi_i2c_transfer));
        memcpy(params[0], device, sizeof(struct capi_i2c_device));
        memcpy(params[1], transfer, sizeof(struct capi_i2c_transfer));
    } else {
        return -1;
    }

    return 0;
}


void capi_enqueue(Scheduler *scheduler, uint64_t trigger, void (*fun)(void), ...) {
    DecodedRPC d = {
        .trigger_time = trigger,
        .func = (void*)fun,
        .params = {NULL}
    };
    
    va_list args;
    va_start(args, fun);
    add_params(fun, d.params, args);
    va_end(args);
    scheduler->enqueue(scheduler, &d);
}

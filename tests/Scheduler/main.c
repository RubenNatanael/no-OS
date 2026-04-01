#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "capi_i2c.h"
#include "capi_scheduler.h"

int print_status(int status, const char *text) {
    if (status == 0) {
        printf("%s successful!\n", text);
        return 0;
    } else if (status == -EINVAL) {
        printf("%s failed, invalid address\n", text);
        return 1;
    } else {
        printf("%s failed with status: %d\n", text, status);
        return 1;
    }
}

int main() {
    Scheduler scheduler = default_scheduler;
    scheduler.init(&scheduler);

    // 1. Client create params from function
    uint8_t sub_addr = 0x00;
    uint8_t rx_buf[16] = {0};

    struct capi_i2c_transfer transfer = {
        .repeated_start = false,
        .sub_address = &sub_addr,
        .sub_address_len = sizeof(sub_addr),
        .buf = rx_buf,
        .len = 2,
        .no_stop = false,
    };

    struct capi_i2c_device device = {
        .controller = NULL,
        .address = 0x48,
        .b10addr = false,
        .speed = CAPI_I2C_SPEED_STANDARD,
        .duty_cycle = 0,
        .clk_stretch = 0,
        .extra = NULL,
    };

    // 2. Capi enqueue a function and its parameters
    capi_enqueue(&scheduler, 100, (void (*)(void))capi_i2c_transmit, &device, &transfer);

    // 3.Data transfer
    // TODO
    int once = 1;
    while(once) {
        scheduler.check(&scheduler);
        once--;
    }

}

#ifndef CAPI_I2C_PLATFORM_H
#define CAPI_I2C_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "capi_i2c.h"

// Platform-specific I2C ops (implemented by linux, dummy, or other drivers)
extern struct capi_i2c_ops i2c_ops;

#ifdef __cplusplus
}
#endif

#endif // CAPI_I2C_PLATFORM_H

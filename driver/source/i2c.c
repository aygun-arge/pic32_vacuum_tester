
#include "driver/i2c.h"

void initI2cDriver(void) {
    
}

void i2cOpen(
    struct i2cHandle *          handle,
    const struct i2cConfig *    config) {

    handle->id    = config->id;
    handle->flags = config->flags;
    handle->id->open(config, handle);
}

void i2cClose(
    struct i2cHandle *  handle) {

    handle->id->close(handle);
}

void i2cAck(
    struct i2cHandle *          handle) {

    handle->id->ack(handle);
}

void i2cNack(
    struct i2cHandle *          handle) {

    handle->id->nack(handle);
}

bool i2cWriteArray(
    struct i2cHandle *          handle,
    const uint8_t *             data,
    size_t                      size) {

    while (size != 0) {

        if (handle->id->write(handle, *data) != true) {

            return (false);
        }
        data++;
        size--;
    }

    return (true);
}

bool i2cWrite(
    struct i2cHandle *          handle,
    uint8_t                     data) {

    return (handle->id->write(handle, data));
}

void i2cReadArray(
    struct i2cHandle *          handle,
    uint8_t *                   data,
    size_t                      size) {

    while (size != 0) {

        *data = handle->id->read(handle);
        handle->id->ack(handle);
        data++;
        size--;
    }
}

uint8_t i2cRead(
    struct i2cHandle *          handle) {

    return (handle->id->read(handle));
}

void i2cStart(
    struct i2cHandle *          handle) {

    handle->id->start(handle);
}

void i2cRestart(
    struct i2cHandle *          handle) {

    handle->id->restart(handle);
}

void i2cStop(
    struct i2cHandle *          handle) {

    handle->id->stop(handle);
}
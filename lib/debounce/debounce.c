/*
 * File:   debounce.h
 * Author: nenad
 *
 * Created on July 15, 2014, 1:45 PM
 */

#include "debounce.h"
#include "eds/epa.h"

static void gpio_a_handler(void)
{
    esEvent
    gpioChangeDisableHandler(&GpioA);

}

static void gpio_b_handler(void)
{

}

static void gpio_c_handler(void)
{

}

void debounce_init(struct debounce_def * def, struct debounce_handle * handle)
{
    handle->def.gpio     = def->gpio;
    handle->def.gpio_pin = def->gpio_pin;
    gpioSetAsInput(handle->def.gpio, handle->def.gpio_pin);

    if (def->gpio == &GpioA) {
        gpioChangeSetHandler(def->gpio, def->gpio_pin, gpio_a_handler);
    } else if (def->gpio == &GpioB) {
        gpioChangeSetHandler(def->gpio, def->gpio_pin, gpio_b_handler);
    } else {
        gpioChangeSetHandler(def->gpio, def->gpio_pin, gpio_c_handler);
    }
}


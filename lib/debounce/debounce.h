/* 
 * File:   debounce.h
 * Author: nenad
 *
 * Created on July 15, 2014, 1:41 PM
 */

#ifndef DEBOUNCE_H
#define	DEBOUNCE_H

#include <stdint.h>

#include "driver/gpio.h"

#ifdef	__cplusplus
extern "C" {
#endif

struct debounce_def
{
    struct gpio *               gpio;
    uint32_t                    gpio_pin;
};

struct debounce_handle
{
    struct debounce_def         def;
};

void debounce_init(struct debounce_def * def, struct debounce_handle * handle);


#ifdef	__cplusplus
}
#endif

#endif	/* DEBOUNCE_H */


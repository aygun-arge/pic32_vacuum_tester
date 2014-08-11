/* 
 * File:    gpio.h
 * Author:  nenad
 * Details: GPIO
 *
 * Created on February 16, 2014, 8:47 PM
 */

#ifndef GPIO_H
#define	GPIO_H

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

/*===============================================================  MACRO's  ==*/

#define CONFIG_MAX_CHANGE_HANDLERS          3
#define GPIO_NUM_OF_PORTS                   3

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct change_slot;

struct gpio {
    volatile unsigned int * port;
    volatile unsigned int * tris;
    volatile unsigned int * lat;
    volatile unsigned int * set;
    volatile unsigned int * clr;
    volatile unsigned int * invert;
    volatile unsigned int * od;
    volatile unsigned int * change;
    volatile unsigned int * status;
    volatile unsigned int * pullup;
    volatile unsigned int * pulldown;
    volatile unsigned int * ansel;
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern const struct gpio GpioA;
extern const struct gpio GpioB;
extern const struct gpio GpioC;

/*===================================================  FUNCTION PROTOTYPES  ==*/

void initGpioDriver(void);

struct change_slot * gpio_request_slot(const struct gpio * gpio, uint32_t pin, void (* handler)(void));
void gpio_release_slot(struct change_slot * slot);
void gpio_change_enable(struct change_slot * slot);
void gpio_change_disable(struct change_slot * slot);

static inline void gpioSetAsInput(const struct gpio * gpio, uint32_t pin)
{
    *gpio->tris |= ((uint32_t)0x1u << pin);
}

static inline void gpioSetPullDown(const struct gpio * gpio, uint32_t pin)
{
    *gpio->pulldown |= ((uint32_t)0x1u << pin);
}

static inline void gpioSetAsOutput(const struct gpio * gpio, uint32_t pin)
{
    *gpio->tris &= ~((uint32_t)0x1u << pin);
}

static inline void gpioSetAsOutputPullUp(const struct gpio * gpio, uint32_t pin)
{
    *gpio->tris   &= ~((uint32_t)0x1u << pin);
    *gpio->od     |=  ((uint32_t)0x1u << pin);
    *gpio->pullup |=  ((uint32_t)0x1u << pin);
}

static inline uint32_t gpioRead(const struct gpio * gpio)
{
    return (*gpio->port);
}

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of gpio.h
 ******************************************************************************/
#endif /* GPIO_H */

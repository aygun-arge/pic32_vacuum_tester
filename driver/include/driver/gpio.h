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

#define GPIO_NUM_OF_PORTS                   3

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

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

void initGpioDriver(
    void);

void gpioChangeSetHandler(const struct gpio * gpio, uint32_t pin, void (* handler)(void));
void gpioChangeEnableHandler(const struct gpio * gpio);
void gpioChangeDisableHandler(const struct gpio * gpio);

static inline void gpioSetAsInput(const struct gpio * gpio, uint32_t pin)
{
    *gpio->tris |= ((uint32_t)0x1u << pin);
}

static inline void gpioSetAsOutput(const struct gpio * gpio, uint32_t pin)
{
    *gpio->tris &= ~((uint32_t)0x1u << pin);
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

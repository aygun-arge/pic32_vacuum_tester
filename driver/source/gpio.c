/*
 * File:    spi.c
 * Author:  nenad
 * Details: GPIO
 *
 * Created on February 17, 2014, 1:41 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <xc.h>

#include "driver/gpio.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

const struct gpio GpioA = {
    .port     = &PORTA,
    .tris     = &TRISA,
    .lat      = &LATA,
    .set      = &LATASET,
    .clr      = &LATACLR,
    .invert   = &LATAINV,
    .od       = &ODCA,
    .change   = &CNENA,
    .status   = &CNSTATA,
    .pullup   = &CNPUA,
    .pulldown = &CNPDA,
    .ansel    = &ANSELA
};

const struct gpio GpioB = {
    .port     = &PORTB,
    .tris     = &TRISB,
    .lat      = &LATB,
    .set      = &LATBSET,
    .clr      = &LATBCLR,
    .invert   = &LATBINV,
    .od       = &ODCB,
    .change   = &CNENB,
    .status   = &CNSTATB,
    .pullup   = &CNPUB,
    .pulldown = &CNPDB,
    .ansel    = &ANSELB
};

const struct gpio GpioC = {
    .port     = &PORTC,
    .tris     = &TRISC,
    .lat      = &LATC,
    .set      = &LATCSET,
    .clr      = &LATCCLR,
    .invert   = &LATCINV,
    .od       = &ODCC,
    .change   = &CNENC,
    .status   = &CNSTATC,
    .pullup   = &CNPUC,
    .pulldown = &CNPDC,
    .ansel    = &ANSELC
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void initPort(struct gpio * gpio) {
    *(gpio->port)     = 0u;
    *(gpio->tris)     = 0xffffffffu;
    *(gpio->lat)      = 0u;
    *(gpio->od)       = 0u;
    *(gpio->change)   = 0u;
    *(gpio->pullup)   = 0u;
    *(gpio->pulldown) = 0u;
    *(gpio->ansel)    = 0u;
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initGpioDriver(
    void) {
#if (((__PIC32_FEATURE_SET__ >= 100) && (__PIC32_FEATURE_SET__ <= 299)))
    /*
     * Setup all pins as digital IO with push-pull mode
     */
    initPort(&GpioA);
    initPort(&GpioB);
    initPort(&GpioC);
#endif
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of uart.c
 ******************************************************************************/


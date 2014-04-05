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
    &PORTA, &TRISA, &LATA, &LATASET, &LATACLR, &LATAINV, &CNPUA, &CNPDA, &ANSELA
};

const struct gpio GpioB = {
    &PORTB, &TRISB, &LATB, &LATBSET, &LATBCLR, &LATBINV, &CNPUB, &CNPDB, &ANSELB
};

const struct gpio GpioC = {
    &PORTC, &TRISC, &LATC, &LATCSET, &LATCCLR, &LATCINV, &CNPUC, &CNPDC, &ANSELC
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initGpioDriver(
    void) {
#if (((__PIC32_FEATURE_SET__ >= 100) && (__PIC32_FEATURE_SET__ <= 299)))
    /*
     * Setup all pins as digital IO with push-pull mode
     */
    ANSELA = 0u;
    ODCA   = 0u;
    CNPUA  = 0u;
    CNPDA  = 0u;
    ANSELB = 0u;
    CNPUB  = 0u;
    CNPDB  = 0u;
    ODCB   = 0u;
    ANSELC = 0u;
    ODCC   = 0u;
    CNPUC  = 0u;
    CNPDC  = 0u;
#endif
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of uart.c
 ******************************************************************************/


/*
 * File:    spi.c
 * Author:  nenad
 * Details: GPIO
 *
 * Created on February 17, 2014, 1:41 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "arch/intr.h"
#include <xc.h>
#include <sys/attribs.h>

#include "driver/gpio.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CNCON_ON                        (0x1u << 15)

#define CHANGE_INT_GPIOA                (0x1u << 13)
#define CHANGE_PRIO_GPIOA               (0x7u << 18)
#define CHANGE_SUBPRIO_GPIOA            (0x3u << 16)

#define CHANGE_INT_GPIOB                (0x1u << 14)
#define CHANGE_PRIO_GPIOB               (0x7u << 18)
#define CHANGE_SUBPRIO_GPIOB            (0x3u << 16)

#define CHANGE_INT_GPIOC                (0x1u << 15)
#define CHANGE_PRIO_GPIOC               (0x7u << 18)
#define CHANGE_SUBPRIO_GPIOC            (0x3u << 16)

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static void (* Handler[3])(void);

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

static void initPort(const struct gpio * gpio) {
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

void gpioChangeSetHandler(const struct gpio * gpio, uint32_t pin, void (* handler)(void)) {
    *(gpio->change) |= (0x1u << pin);

    if (gpio == &GpioA) {
        Handler[0]    = handler;
        CNCONA       |= CNCON_ON;
        IEC1CLR       = CHANGE_INT_GPIOA;
        IFS1CLR       = CHANGE_INT_GPIOA;
        IPC8bits.CNIP = CONFIG_INTR_MAX_ISR_PRIO;
        IPC8bits.CNIS = 0;
        IEC1SET       = CHANGE_INT_GPIOA;
    } else if (gpio == &GpioB) {
        Handler[1]    = handler;
        CNCONB       |= CNCON_ON;
        IEC1CLR       = CHANGE_INT_GPIOB;
        IFS1CLR       = CHANGE_INT_GPIOB;
        IPC8bits.CNIP = CONFIG_INTR_MAX_ISR_PRIO;
        IPC8bits.CNIS = 0;
        IEC1SET       = CHANGE_INT_GPIOB;
    } else if (gpio == &GpioC) {
        Handler[2]    = handler;
        CNCONC       |= CNCON_ON;
        IEC1CLR       = CHANGE_INT_GPIOC;
        IFS1CLR       = CHANGE_INT_GPIOC;
        IPC8bits.CNIP = CONFIG_INTR_MAX_ISR_PRIO;
        IPC8bits.CNIS = 0;
        IEC1SET       = CHANGE_INT_GPIOC;
    }
}

void gpioChangeEnableHandler(const struct gpio * gpio) {

    if (gpio == &GpioA) {
        IEC1SET = CHANGE_INT_GPIOA;
    } else if (gpio == &GpioB) {
        IEC1SET = CHANGE_INT_GPIOB;
    } else if (gpio == &GpioC) {
        IEC1SET = CHANGE_INT_GPIOC;
    }
}

void gpioChangeDisableHandler(const struct gpio * gpio) {

    if (gpio == &GpioA) {
        IEC1CLR = CHANGE_INT_GPIOA;
    } else if (gpio == &GpioB) {
        IEC1CLR = CHANGE_INT_GPIOB;
    } else if (gpio == &GpioC) {
        IEC1CLR = CHANGE_INT_GPIOC;
    }
}

void __ISR(_CHANGE_NOTICE_VECTOR) changeNotice(void) {
    volatile uint32_t   port;
    uint32_t            intFlag;

    intFlag = IEC1 & IFS1;
    port = *GpioA.port;

    if (intFlag & CHANGE_INT_GPIOA) {

        if (Handler[0] != NULL) {
            Handler[0]();
        }
        IFS1CLR = CHANGE_INT_GPIOA;
    }
    port = *GpioB.port;

    if (intFlag & CHANGE_INT_GPIOB) {

        if (Handler[1] != NULL) {
            Handler[1]();
        }
        IFS1CLR = CHANGE_INT_GPIOB;
    }
    port = *GpioC.port;

    if (intFlag & CHANGE_INT_GPIOC) {

        if (Handler[2] != NULL) {
            Handler[2]();
        }
        IFS1CLR = CHANGE_INT_GPIOC;
    }
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of gpio.c
 ******************************************************************************/


/*
 * File:    clock.c
 * Author:  nenad
 * Details: Clock management
 *
 * Created on February 17, 2014, 1:13 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <peripheral/system.h>
#include <xc.h>

#include "driver/clock.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define REFOCON_ROSEL_SYSCLK            (0x0u << 0)
#define REFOCON_ROSEL_PBCLK             (0x1u << 0)
#define REFOCON_ROSEL_FRC               (0x3u << 0)
#define REFOCON_ACTIVE                  (0x1u << 8)
#define REFOCON_OE                      (0x1u << 12)
#define REFOCON_ON                      (0x1u << 15)

#define REFOCON_RODIV_Pos               16
#define REFOCON_RODIV_Msk               (0x7fu << REFOCON_RODIV_Pos)

#define CONFIG_SYSTEM_CLOCK             48000000ul

/*======================================================  LOCAL DATA TYPES  ==*/

struct clocks {
    uint32_t            system;
    uint32_t            peripheralBus;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static struct clocks GlobalClocks;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initClockDriver(
    void) {

    GlobalClocks.system        = CONFIG_SYSTEM_CLOCK;
    GlobalClocks.peripheralBus = SYSTEMConfigPerformance(CONFIG_SYSTEM_CLOCK);
}

uint32_t clockGetSystemClock(
    void) {

    return (GlobalClocks.system);
}

uint32_t clockGetPeripheralClock (
    void) {

    return (GlobalClocks.peripheralBus);
}

void clockSetOutput(
    enum clockOutNum    outputNum,
    enum clockOutSource source,
    enum clockOutDivider divider) {

    REFOCON = 0u;

    switch (outputNum) {
        case CLOCK_OUT_1: {
            RPA2R = 0x7u;
            break;
        }
        case CLOCK_OUT_2 : {
            RPA3R = 0x0u;
            break;
        }
        default : {
            return;
        }
    }
    while ((REFOCON & REFOCON_ACTIVE) != 0u);

    switch (source) {
        case CLOCK_OUT_SOURCE_SYSCLK : {
            REFOCONSET = REFOCON_ROSEL_SYSCLK;
            break;
        }
        case CLOCK_OUT_SOURCE_PBCLK : {
            REFOCONSET = REFOCON_ROSEL_PBCLK;
            break;
        }
        case CLOCK_OUT_SOURCE_FRC : {
            REFOCONSET = REFOCON_ROSEL_FRC;
            break;
        }
    }

    switch (divider) {
        case CLOCK_OUT_DIV_1 : {
            REFOTRIM   = 0x0u;
            REFOCONCLR = REFOCON_RODIV_Msk;
            break;
        }
        case CLOCK_OUT_DIV_2 : {
            REFOTRIM   = 0x0u;
            REFOCONCLR = REFOCON_RODIV_Msk;
            REFOCONSET = (0x1u << REFOCON_RODIV_Pos);
            break;
        }
        case CLOCK_OUT_DIV_4 : {
            REFOTRIM   = 0x0u;
            REFOCONCLR = REFOCON_RODIV_Msk;
            REFOCONSET = (0x2u << REFOCON_RODIV_Pos);
            break;
        }
        case CLOCK_OUT_DIV_6 : {
            REFOTRIM   = 0x0u;
            REFOCONCLR = REFOCON_RODIV_Msk;
            REFOCONSET = (0x3u << REFOCON_RODIV_Pos);
            break;
        }
        default : {
            REFOTRIM = 0x0u;
            REFOCONCLR = REFOCON_RODIV_Msk;
            break;
        }
    }
    REFOCONSET = REFOCON_ON | REFOCON_OE;
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.c
 ******************************************************************************/

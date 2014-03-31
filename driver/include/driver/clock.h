/* 
 * File:    clock.h
 * Author:  nenad
 * Details: Clock management
 *
 * Created on February 17, 2014, 1:18 AM
 */

#ifndef CLOCK_H
#define	CLOCK_H

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum clockOutNum {
    CLOCK_OUT_DISABLE,
    CLOCK_OUT_1,
    CLOCK_OUT_2
};

enum clockOutSource {
    CLOCK_OUT_SOURCE_SYSCLK = (0x0u << 0),
    CLOCK_OUT_SOURCE_PBCLK  = (0x1u << 0),
    CLOCK_OUT_SOURCE_FRC    = (0x3u << 0)
};

enum clockOutDivider {
    CLOCK_OUT_DIV_1,
    CLOCK_OUT_DIV_2,
    CLOCK_OUT_DIV_4,
    CLOCK_OUT_DIV_6
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void initClockDriver(
    void);

uint32_t clockGetSystemClock(
    void);

uint32_t clockGetPeripheralClock (
    void);

void clockSetOutput(
    enum clockOutNum    outputNum,
    enum clockOutSource source,
    enum clockOutDivider divider);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of clock.h
 ******************************************************************************/
#endif /* CLOCK_H */


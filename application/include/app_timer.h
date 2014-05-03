/* 
 * File:   app_timer.h
 * Author: nenad
 *
 * Created on April 30, 2014, 12:48 AM
 */

#ifndef APP_TIMER_H
#define	APP_TIMER_H

#include "vtimer/vtimer.h"
#include "eds/epa.h"

#ifdef	__cplusplus
extern "C" {
#endif

struct appTimer {
    esVTimer            timer;
    uint16_t            eventId;
    esEpa *             epa;
};

void appTimerInit(
    struct appTimer *   timer);

void appTimerStart(
    struct appTimer *   timer,
    esSysTimerTick      tick,
    uint16_t            eventId);

void appTimerCancel(
    struct appTimer *   timer);


#ifdef	__cplusplus
}
#endif

#endif	/* APP_TIMER_H */


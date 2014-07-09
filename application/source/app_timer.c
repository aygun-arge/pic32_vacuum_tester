
/*
 * File:    app_timer.c
 * Author:  nenad
 * Details: Various support functions
 *
 * Created on April 30, 2014, 12:48 AM
 */

#include <string.h>

#include "app_timer.h"
#include "base/debug.h"

static const ES_MODULE_INFO_CREATE("AppTimer", "Application Event Timers", "Nenad Radulovic");

static void timeout(void * arg) {

    struct appTimer *   timer;
    esEvent *           event;
    esError             error;

    timer = (struct appTimer *)arg;
    ES_ENSURE(error = esEventCreateI(sizeof(*event), timer->eventId, &event));

    if (error == ES_ERROR_NONE) {
        ES_ENSURE(esEpaSendAheadEventI(timer->epa, event));
    }
}

void appTimerInit(
    struct appTimer *   timer) {

    esVTimerInit(&timer->timer);
    timer->epa = esEdsGetCurrent();
}

void appTimerStart(
    struct appTimer *   timer,
    esSysTimerTick      tick,
    uint16_t            eventId) {

    esVTimerCancel(&timer->timer);
    timer->eventId = eventId;
    esVTimerStart(&timer->timer, tick, timeout, timer);
}

void appTimerCancel(
    struct appTimer *   timer) {

    esVTimerCancel(&timer->timer);
}

esSysTimerTick appTimerGetRemaining(
    const struct appTimer * timer) {
    
    return (esVTimerGetRemaining(&timer->timer));
}

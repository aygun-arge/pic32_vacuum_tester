/* 
 * File:   epa_gobject.h
 * Author: nenad
 *
 * Created on May 1, 2014, 9:27 PM
 */

#ifndef EPA_GOBJECT_H
#define	EPA_GOBJECT_H

#include "eds/epa.h"
#include "gobject.h"

#define CONFIG_EPA_GOBJECT_PRIORITY     28
#define CONFIG_EPA_GOBJECT_QUEUE_SIZE   10
#define CONFIG_EPA_GOBJECT_EVENT_BASE   1100
#define CONFIG_EPA_GOBJECT_NAME         "GObject EPA"

#ifdef	__cplusplus
extern "C" {
#endif

enum gObjectEvents {
    GO_READY            = CONFIG_EPA_GOBJECT_EVENT_BASE,
    GO_TOUCH_START,
    GO_TOUCH_STOP,
    GO_SET_INIT,
    GO_GET_INIT,
    GO_CLICKED,
    GO_PRESSED,
    GO_RELEASED
};

struct gInitDataEvent {
    esEvent             event;
    struct gInitData    data;
};

struct gEvent {
    esEvent             event;
    struct gPage *      page;
    uint16_t            objectId;
};

extern const esEpaDefine GobjectEpa;
extern const esSmDefine  GobjectSm;
extern esEpa *           Gobject;

#ifdef	__cplusplus
}
#endif

#endif	/* EPA_GOBJECT_H */


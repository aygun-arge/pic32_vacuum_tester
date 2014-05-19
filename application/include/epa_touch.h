/* 
 * File:   epa_touch.h
 * Author: nenad
 *
 * Created on May 14, 2014, 10:33 PM
 */

#ifndef EPA_TOUCH_H
#define	EPA_TOUCH_H

#include "events.h"
#include "eds/epa.h"

#define CONFIG_EPA_TOUCH_PRIORITY       28
#define CONFIG_EPA_TOUCH_QUEUE_SIZE     10
#define CONFIG_EPA_TOUCH_EVENT_BASE     2000
#define CONFIG_EPA_TOUCH_NAME           "Graphical Touch Interface"

#ifdef	__cplusplus
extern "C" {
#endif

enum touchEventsId {
    EVT_TOUCH_ENABLE        = CONFIG_EPA_TOUCH_EVENT_BASE,
    EVT_TOUCH_DISABLE,
    EVT_TOUCH_STATUS,
    EVT_TOUCH_INITIALIZE,
    EVT_TOUCH_CALIBRATE,
    EVT_TOUCH_TAG
};

enum touchStatus {
    TOUCH_NOT_INITIALIZED,
    TOUCH_NOT_CALIBRATED,
    TOUCH_INITIALIZED,
    TOUCH_CALIBRATED
};

struct touchStatusEvent {
    esEvent             event;
    enum touchStatus    status;
};

struct touchEvent {
    esEvent             event;
    uint8_t             tag;
};

extern const struct esEpaDefine TouchEpa;
extern const struct esSmDefine  TouchSm;
extern struct esEpa *           Touch;

#ifdef	__cplusplus
}
#endif

#endif	/* EPA_TOUCH_H */


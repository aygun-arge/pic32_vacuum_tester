
/*=========================================================  INCLUDE FILES  ==*/

#include "epa_gobject.h"
#include "gobject.h"
#include "app_timer.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CONFIG_GOBJECT_REFRESH_MS       20

#define GOBJECT_TABLE(entry)                                                    \
    entry(stateInit,                TOP)                                        \
    entry(stateWakeUp,              TOP)                                        \
    entry(stateIdle,                TOP)                                        \
    entry(stateTouchEnabled,        stateIdle)                                  \
    entry(stateTouchDisabled,       stateIdle)                                  \

/*======================================================  LOCAL DATA TYPES  ==*/

enum gObjectStateId {
    ES_STATE_ID_INIT(GOBJECT_TABLE)
};

enum gObjectLocalEvents {
    EVT_WAKEUP_RETRY        = ES_EVENT_LOCAL_ID,
    EVT_TOUCH_POLL
};

struct wspace {
    struct appTimer     refresh;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit               (struct wspace *, const esEvent *);
static esAction stateWakeUp             (struct wspace *, const esEvent *);
static esAction stateIdle               (struct wspace *, const esEvent *);
static esAction stateTouchEnabled       (struct wspace *, const esEvent *);
static esAction stateTouchDisabled      (struct wspace *, const esEvent *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("Gobject", "Graphics object EPA", "Nenad Radulovic");

static const esSmTable  GobjectTable[] = ES_STATE_TABLE_INIT(GOBJECT_TABLE);

/*======================================================  GLOBAL VARIABLES  ==*/

const esEpaDefine       GobjectEpa = ES_EPA_DEFINE(
    CONFIG_EPA_GOBJECT_NAME,
    CONFIG_EPA_GOBJECT_PRIORITY,
    CONFIG_EPA_GOBJECT_QUEUE_SIZE);
const esSmDefine        GobjectSm = ES_SM_DEFINE(
    GobjectTable,
    sizeof(struct wspace),
    stateInit);
esEpa *                 Gobject;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static esAction stateInit(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_INIT : {
            appTimerInit(&wspace->refresh);
            gHalGpuInit();

            return (ES_STATE_TRANSITION(stateWakeUp));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateWakeUp(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_INIT : {

            if (gHalIsGpuReady()) {
                esError   error;
                esEvent * notify;

                gHalGpuEnable();
                ES_ENSURE(error = esEventCreate(sizeof(esEvent), GO_READY, &notify));

                if (error == ES_ERROR_NONE) {
                    ES_ENSURE(esEpaSendEvent(gObjectModuleHandler(), notify));
                }
                
                return (ES_STATE_TRANSITION(stateIdle));
            } else {
                appTimerStart(
                    &wspace->refresh,
                    ES_VTMR_TIME_TO_TICK_MS(100),
                    EVT_WAKEUP_RETRY);

                return (ES_STATE_HANDLED());
            }
        }
        case EVT_WAKEUP_RETRY : {
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(100),
                EVT_WAKEUP_RETRY);

            return (ES_STATE_TRANSITION(stateWakeUp));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateIdle(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_INIT : {

            return (ES_STATE_TRANSITION(stateTouchDisabled));
        }
        case GO_SET_INIT : {
            const struct gInitDataEvent * init;
            
            init = (const struct gInitDataEvent *)event;
            gHalTouchSetCalibration(&init->data.calibration);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateTouchDisabled(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY : {
            appTimerCancel(&wspace->refresh);

            return (ES_STATE_HANDLED());
        }
        case GO_TOUCH_START : {

            return (ES_STATE_TRANSITION(stateTouchEnabled));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateTouchEnabled(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY : {
            appTimerCancel(&wspace->refresh);
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_GOBJECT_REFRESH_MS),
                EVT_TOUCH_POLL);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_POLL : {
            struct gHalPage * halPage;
            esError             error;
            struct gEvent *     notify;
            uint16_t            objectId;

            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_GOBJECT_REFRESH_MS),
                EVT_TOUCH_POLL);
            halPage = gPageGetHalPage();

            if (halPage == NULL) {

                return (ES_STATE_HANDLED());
            }
            objectId = gHalGetHotSpot(halPage);

            if (objectId == 0) {

                return (ES_STATE_HANDLED());
            }
            /* NOTE: This is still not used (WORK IN PROGRESS) */
#if 0
            if (gHalIsPressed(halPage)) {
                ES_ENSURE(error = esEventCreate(sizeof(*notify), GO_PRESSED, (esEvent **)&notify));

                if (error == ES_ERROR_NONE) {
                    notify->objectId = objectId;
                    ES_ENSURE(esEpaSendEvent(gObjectModuleHandler(), (esEvent *)notify));
                }
            }

            if (gHalIsReleased(halPage)) {
                ES_ENSURE(error = esEventCreate(sizeof(*notify), GO_RELEASED, (esEvent **)&notify));

                if (error == ES_ERROR_NONE) {
                    notify->objectId = objectId;
                    ES_ENSURE(esEpaSendEvent(gObjectModuleHandler(), (esEvent *)notify));
                }
            }
#endif
            if (gHalIsClicked(halPage)) {
                ES_ENSURE(error = esEventCreate(sizeof(*notify), GO_CLICKED, (esEvent **)&notify));

                if (error == ES_ERROR_NONE) {
                    notify->objectId = objectId;
                    ES_ENSURE(esEpaSendEvent(gObjectModuleHandler(), (esEvent *)notify));
                }
            }

            return (ES_STATE_HANDLED());
        }
        case GO_TOUCH_STOP : {

            return (ES_STATE_TRANSITION(stateTouchDisabled));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa_gui.c
 ******************************************************************************/

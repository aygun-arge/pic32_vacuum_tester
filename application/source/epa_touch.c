
/*=========================================================  INCLUDE FILES  ==*/

#include "epa_touch.h"
#include "app_gpu.h"
#include "app_storage.h"
#include "checksum/checksum.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define TOUCH_SIGNATURE                 0xdead5051u

#define TOUCH_TABLE(entry)                                                      \
    entry(stateInit,                TOP)                                        \
    entry(stateIdle,                TOP)                                        \
    entry(stateCalibrate,           TOP)                                        \
    entry(stateTouch,               TOP)                                        \

/*======================================================  LOCAL DATA TYPES  ==*/

enum touchStateId {
    ES_STATE_ID_INIT(TOUCH_TABLE)
};

enum touchLocalEventId {
    TOUCH_DETECTED_ = ES_EVENT_LOCAL_ID
};

struct wspace {
    struct storageSpace * nvHandle;
    esEpa *             client;
    uint8_t             prevTag;
};

struct nvStorageData {
    struct gpuTouchData gpuTouchData;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit               (struct wspace *, const esEvent *);
static esAction stateIdle               (struct wspace *, const esEvent *);
static esAction stateCalibrate          (struct wspace *, const esEvent *);
static esAction stateTouch              (struct wspace *, const esEvent *);

static void touchHandler(void);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("Touch interface", CONFIG_EPA_TOUCH_NAME, "Nenad Radulovic");

static const esSmTable  TouchTable[] = ES_STATE_TABLE_INIT(TOUCH_TABLE);

static struct storageSpace * Storage;

/*======================================================  GLOBAL VARIABLES  ==*/

const struct esEpaDefine TouchEpa = ES_EPA_DEFINE(
    CONFIG_EPA_TOUCH_NAME,
    CONFIG_EPA_TOUCH_PRIORITY,
    CONFIG_EPA_TOUCH_QUEUE_SIZE);
const struct esSmDefine  TouchSm = ES_SM_DEFINE(
    TouchTable,
    sizeof(struct wspace),
    stateInit);
struct esEpa *           Touch;

const struct storageEntry TouchStorage = {
    TOUCH_SIGNATURE,
    sizeof(struct nvStorageData),
    &Storage
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static esAction stateInit(struct wspace * space, const esEvent * event) {
    
    switch (event->id) {
        case ES_INIT: {

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateIdle(struct wspace * space, const esEvent * event) {

    switch (event->id) {

        case EVT_TOUCH_INITIALIZE : {
            struct nvStorageData storageData;
            esEvent *   response;
            esError     error;

            if (storageRead(Storage, &storageData)) {
                goto SPACE_FAILURE;
            }
            ES_ENSURE(error = esEventCreate(
                sizeof(struct touchStatusEvent),
                EVT_TOUCH_STATUS,
                &response));
            ((struct touchStatusEvent *)response)->status = TOUCH_INITIALIZED;

            if (error == ES_ERROR_NONE) {
                ES_ENSURE(esEpaSendEvent(event->producer, response));
            }
            gpuSetTouchCalibration(&storageData.gpuTouchData);

            return (ES_STATE_HANDLED());
SPACE_FAILURE:
            ES_ENSURE(error = esEventCreate(
                sizeof(struct touchStatusEvent),
                EVT_TOUCH_STATUS,
                &response));
            ((struct touchStatusEvent *)response)->status = TOUCH_NOT_INITIALIZED;

            if (error == ES_ERROR_NONE) {
                ES_ENSURE(esEpaSendEvent(event->producer, response));
            }
            gpuGetDefaultTouch(&storageData.gpuTouchData);
            gpuSetTouchCalibration(&storageData.gpuTouchData);
            storageWrite(Storage, &storageData);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_CALIBRATE : {
            space->client = event->producer;
            
            return (ES_STATE_TRANSITION(stateCalibrate));
        }
        case EVT_TOUCH_ENABLE : {
            space->client = event->producer;

            return (ES_STATE_TRANSITION(stateTouch));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCalibrate(struct wspace * space, const esEvent * event) {
    switch (event->id) {
        case ES_INIT : {
            struct nvStorageData storageData;
            esEvent *   response;
            esError     error;

            gpuGetTouchCalibration(&storageData.gpuTouchData);
            error = storageWrite(Storage, &storageData);
            
            if (error != ES_ERROR_NONE) {
                goto SPACE_FAILURE;
            }
            ES_ENSURE(error = esEventCreate(
                sizeof(struct touchStatusEvent),
                EVT_TOUCH_STATUS,
                &response));
            ((struct touchStatusEvent *)response)->status = TOUCH_CALIBRATED;

            if (error == ES_ERROR_NONE) {
                ES_ENSURE(esEpaSendEvent(space->client, response));
            }

            return (ES_STATE_TRANSITION(stateIdle));
SPACE_FAILURE:
            ES_ENSURE(error = esEventCreate(
                sizeof(struct touchStatusEvent),
                EVT_TOUCH_STATUS,
                &response));
            ((struct touchStatusEvent *)response)->status = TOUCH_NOT_CALIBRATED;

            if (error == ES_ERROR_NONE) {
                ES_ENSURE(esEpaSendEvent(event->producer, response));
            }

            return (ES_STATE_TRANSITION(stateIdle));

        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateTouch(struct wspace * space, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY : {
            gpuTouchEnable(touchHandler);
            space->prevTag = 0;

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT : {
            gpuTouchDisable();

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_DISABLE : {

            return (ES_STATE_TRANSITION(stateIdle));
        }
        case EVT_TOUCH_CALIBRATE : {

            return (ES_STATE_TRANSITION(stateCalibrate));
        }
        case TOUCH_DETECTED_ : {
            uint8_t     tag;

            tag = gpuGetTouchTag();

            if ((space->prevTag != 0u) && (tag == 0u)) {
                esError             error;
                struct touchEvent * touchEvent;

                ES_ENSURE(error = esEventCreate(
                    sizeof(struct touchEvent),
                    EVT_TOUCH_TAG,
                    (esEvent **)&touchEvent));

                if (error == ES_ERROR_NONE) {
                    touchEvent->tag = space->prevTag;
                    ES_ENSURE(esEpaSendEvent(space->client, (esEvent *)touchEvent));
                }
            }
            space->prevTag = tag;

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static void touchHandler(void) {
    esEvent *           event;
    esError             error;

    ES_ENSURE(error = esEventCreate(sizeof(esEvent), TOUCH_DETECTED_, &event));

    if (error == ES_ERROR_NONE) {
        ES_ENSURE(esEpaSendEvent(Touch, event));
    }
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa_touch.c
 ******************************************************************************/
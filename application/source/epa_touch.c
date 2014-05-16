
/*=========================================================  INCLUDE FILES  ==*/

#include "epa_touch.h"
#include "app_gpu.h"
#include "app_storage.h"
#include "config/nv_storage.h"
#include "checksum/checksum.h"

/*=========================================================  LOCAL MACRO's  ==*/

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
};

struct nvStorageData {
    struct gpuTouchData gpuTouchData;
    uint8_t             checksum;
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

        case TOUCH_INITIALIZE : {
            struct nvStorageData storageData;
            esEvent *   response;
            esError     error;
            size_t      read;

            error = storageOpenSpace(NV_STORAGE_UI_ID, &space->nvHandle);

            if (error != ES_ERROR_NONE) {
                goto SPACE_FAILURE;
            }
            error = storageRead(
                space->nvHandle,
                &storageData,
                sizeof(storageData),
                &read);

            if ((error != ES_ERROR_NONE)        ||
                (read != sizeof(storageData))   ||
                (checksumParity8(&storageData, sizeof(storageData)) != 0u)) {
                goto SPACE_FAILURE;
            }
            ES_ENSURE(error = esEventCreate(
                sizeof(struct touchStatusEvent),
                TOUCH_STATUS,
                &response));
            ((struct touchStatusEvent *)response)->status = TOUCH_INITIALIZED;

            if (error == ES_ERROR_NONE) {
                ES_ENSURE(esEpaSendEvent(event->producer, response));
            }
            gpuSetTouch(&storageData.gpuTouchData);

            return (ES_STATE_HANDLED());
SPACE_FAILURE:
            ES_ENSURE(error = esEventCreate(
                sizeof(struct touchStatusEvent),
                TOUCH_STATUS,
                &response));
            ((struct touchStatusEvent *)response)->status = TOUCH_NOT_INITIALIZED;

            if (error == ES_ERROR_NONE) {
                ES_ENSURE(esEpaSendEvent(event->producer, response));
            }
            gpuGetDefaultTouch(&storageData.gpuTouchData);
            gpuSetTouch(&storageData.gpuTouchData);

            return (ES_STATE_HANDLED());
        }
        case TOUCH_CALIBRATE : {
            
            return (ES_STATE_TRANSITION(stateCalibrate));
        }
        case TOUCH_ENABLE : {

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
            size_t      written;

            gpuGetTouch(&storageData.gpuTouchData);
            storageData.checksum = 0;
            storageData.checksum = checksumParity8(
                &storageData,
                sizeof(storageData));
            storageSetPos(space->nvHandle, 0);
            error = storageWrite(
                space->nvHandle, 
                &storageData, 
                sizeof(storageData), 
                &written);
            
            if (error != ES_ERROR_NONE) {
                goto SPACE_FAILURE;
            }
            ES_ENSURE(error = esEventCreate(
                sizeof(struct touchStatusEvent),
                TOUCH_STATUS,
                &response));
            ((struct touchStatusEvent *)response)->status = TOUCH_CALIBRATED;

            if (error == ES_ERROR_NONE) {
                ES_ENSURE(esEpaSendEvent(event->producer, response));
            }

            return (ES_STATE_TRANSITION(stateIdle));
SPACE_FAILURE:
            ES_ENSURE(error = esEventCreate(
                sizeof(struct touchStatusEvent),
                TOUCH_STATUS,
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

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT : {

            return (ES_STATE_HANDLED());
        }
        case TOUCH_DISABLE : {

            return (ES_STATE_TRANSITION(stateIdle));
        }
        case TOUCH_CALIBRATE : {

            return (ES_STATE_TRANSITION(stateCalibrate));
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
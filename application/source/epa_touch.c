
/*=========================================================  INCLUDE FILES  ==*/

#include "epa_touch.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define TOUCH_TABLE(entry)                                                      \
    entry(stateInit,                TOP)                                        \

/*======================================================  LOCAL DATA TYPES  ==*/

enum touchStateId {
    ES_STATE_ID_INIT(TOUCH_TABLE)
};

struct wspace {

};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit               (struct wspace *, const esEvent *);

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
    
    return (ES_STATE_IGNORED());
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa_touch.c
 ******************************************************************************/
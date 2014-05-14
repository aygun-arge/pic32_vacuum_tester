
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

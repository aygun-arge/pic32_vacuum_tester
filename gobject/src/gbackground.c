

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "ghal.h"
#include "gobject.h"
#include "gtheme.h"
#include "gbackground.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define EXPAND_BACKGROUND_COLOR(a, b, c)                                        \
    [a][0].rgb = b, [a][1].rgb = c,

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static const struct gColor * backgroundThemedColor(enum gThemeColors color);
static void     backgroundDraw(struct gHalPage *, const struct gObject *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const struct gColor BackgroundColor[][2] = {
    BACKGROUND_COLOR_TABLE(EXPAND_BACKGROUND_COLOR)
};

/*======================================================  GLOBAL VARIABLES  ==*/

const struct gMethod BackgroundMethods = {
    .themedColor      = backgroundThemedColor,
    .propertySetLabel = NULL,
    .draw             = backgroundDraw
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static const struct gColor * backgroundThemedColor(enum gThemeColors color) {

    return (&BackgroundColor[color][0]);
}

static void backgroundDraw(struct gHalPage * halPage, const struct gObject * object) {
    gHalBackground(halPage, object->color);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of gtext.c
 ******************************************************************************/


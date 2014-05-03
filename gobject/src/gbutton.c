
/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "ghal.h"
#include "gobject.h"
#include "gtheme.h"
#include "gbutton.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define EXPAND_BUTTON_SIZE(a, b, c)                                             \
    [a].width = b, [a].height = c,

#define EXPAND_BUTTON_COLOR(a, b, c, d, e)                                      \
    [a][0].rgb = b, [a][1].rgb = c, [a][2].rgb = d, [a][3].rgb = e,

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void     buttonInit(struct gObject *);
static const struct gColor * buttonThemedColor(enum gThemeColors color);
static uint16_t buttonThemedWidth(enum gThemeSizes size);
static uint16_t buttonThemedHeight(enum gThemeSizes size);
static void     buttonPropertySetLabel(struct gObject *, const char *);
static void     buttonDraw(struct gHalPage *, const struct gObject *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const struct gSize ButtonSize[] = {
    BUTTON_SIZE_TABLE(EXPAND_BUTTON_SIZE)
};

static const struct gColor ButtonColor[][4] = {
    BUTTON_COLOR_TABLE(EXPAND_BUTTON_COLOR)
};

/*======================================================  GLOBAL VARIABLES  ==*/

const struct gMethod ButtonMethods = {
    .init             = buttonInit,
    .themedColor      = buttonThemedColor,
    .themedWidth      = buttonThemedWidth,
    .themedHeight     = buttonThemedHeight,
    .propertySetLabel = buttonPropertySetLabel,
    .draw             = buttonDraw
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void buttonInit(struct gObject * object) {
    buttonPropertySetLabel(object, "Button");
}

static const struct gColor * buttonThemedColor(enum gThemeColors color) {

    return (&ButtonColor[color][0]);
}

static uint16_t buttonThemedWidth(enum gThemeSizes size) {

    return (ButtonSize[size].width);
}

static uint16_t buttonThemedHeight(enum gThemeSizes size) {

    return (ButtonSize[size].height);
}

static void buttonPropertySetLabel(struct gObject * object, const char * label) {
    ((struct gButtonProperty *)object->property)->label = label;

    if (object->geometry.size.width > 100) {
        ((struct gButtonProperty *)object->property)->font = 30;
    } else {
        ((struct gButtonProperty *)object->property)->font = 27;
    }
}

static void buttonDraw(struct gHalPage * halPage, const struct gObject * object) {
    gHalButton(
        halPage,
        &object->geometry,
        object->color,
        ((struct gButtonProperty *)object->property)->label,
        ((struct gButtonProperty *)object->property)->font);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of gbutton.c
 ******************************************************************************/

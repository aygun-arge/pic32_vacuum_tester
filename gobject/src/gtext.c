

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "ghal.h"
#include "gobject.h"
#include "gtheme.h"
#include "gtext.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define EXPAND_TEXT_SIZE(a, b, c)                                               \
    [a].width = b, [a].height = c,

#define EXPAND_TEXT_COLOR(a, b)                                                 \
    [a].rgb = b,

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void     textInit(struct gObject *);
static const struct gColor * textThemedColor(enum gThemeColors color);
static uint16_t textThemedWidth(enum gThemeSizes size);
static uint16_t textThemedHeight(enum gThemeSizes size);
static void     textPropertySetLabel(struct gObject *, const char *);
static void     textPropertySetFont(struct gObject *, uint16_t);
static void     textDraw(struct gHalPage *, const struct gObject *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const struct gSize TextSize[] = {
    TEXT_SIZE_TABLE(EXPAND_TEXT_SIZE)
};

static const struct gColor TextColor[] = {
    TEXT_COLOR_TABLE(EXPAND_TEXT_COLOR)
};

/*======================================================  GLOBAL VARIABLES  ==*/

const struct gMethod TextMethods = {
    .init             = textInit,
    .themedColor      = textThemedColor,
    .themedWidth      = textThemedWidth,
    .themedHeight     = textThemedHeight,
    .propertySetLabel = textPropertySetLabel,
    .propertySetFont  = textPropertySetFont,
    .draw             = textDraw
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void textInit(struct gObject * object) {
    textPropertySetLabel(object, "Text");
    textPropertySetFont(object, 0);
}

static const struct gColor * textThemedColor(enum gThemeColors color) {

    return (&TextColor[color]);
}

static uint16_t textThemedWidth(enum gThemeSizes size) {

    return (TextSize[size].width);
}

static uint16_t textThemedHeight(enum gThemeSizes size) {

    return (TextSize[size].height);
}

static void textPropertySetLabel(struct gObject * object, const char * label) {
    ((struct gTextProperty *)object->property)->label = label;
}

static void textPropertySetFont(struct gObject * object, uint16_t font) {
    ((struct gTextProperty *)object->property)->font = font;
}

static void textDraw(struct gHalPage * halPage, const struct gObject * object) {
    gHalText(
        halPage,
        &object->geometry,
        object->color,
        ((struct gTextProperty *)object->property)->label,
        ((struct gTextProperty *)object->property)->font);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of gtext.c
 ******************************************************************************/

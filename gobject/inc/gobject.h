/* 
 * File:   gobject.h
 * Author: nenad
 *
 * Created on May 2, 2014, 4:34 PM
 */

#ifndef GOBJECT_H
#define	GOBJECT_H

#include <stdbool.h>
#include <stdint.h>

#include "eds/epa.h"

#include "gprimitives.h"
#include "ghal.h"
#include "gbackground.h"
#include "gbutton.h"
#include "gtext.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define EXPAND_PAGE_TABLE(type, x, y, hAlign, vAlign, size, color, property)    \
    {type, x, y, hAlign, vAlign, size, color, property, sizeof(*property)},

struct gObjectDescriptor {
    enum gThemeTypes   type;
    uint16_t            id;
    uint16_t            x;
    uint16_t            y;
    enum gHAlignments   hAlign;
    enum gVAlignments   vAlign;
    enum gThemeSizes    size;
    enum gThemeColors   color;
    void *              property;
    size_t              propertySize;
};

/*==  gObject module data  ===================================================*/
struct gInitData {
    struct gHalTouchCalibration calibration;
};

struct gPage;

void initGobjectModule(esMem * memory, esEpa * epa);
esEpa * gObjectModuleHandler(void);

struct gPage * gPageCreate(struct gObjectDescriptor * objects);
void gPageDestroy(struct gPage * page);
void gPageSetActive(struct gPage * page);
struct gHalPage * gPageGetHalPage(void);
void gObjectSetX(struct gPage * page, uint16_t id, uint16_t x);
void gObjectSetY(struct gPage * page, uint16_t id, uint16_t y);
void gObjectSetSize(struct gPage * page, uint16_t id, enum gThemeSizes size);
void gObjectSetSizeWidth(struct gPage * page, uint16_t id, uint16_t width);
void gObjectSetSizeHeight(struct gPage * page, uint16_t id, uint16_t height);
void gObjectSetColor(struct gPage * page, uint16_t id, enum gThemeColors color);
void gObjectAttributeSetVisible(struct gPage * page, uint16_t id, bool isVisible);
void gObjectAttributeSetEventId(struct gPage * page, uint16_t id, uint16_t eventId);
void gObjectPropertySetLabel(struct gPage * page, uint16_t id, const char * label);
void gObjectPropertySetFont(struct gPage * page, uint16_t id, uint16_t font);

#ifdef	__cplusplus
}
#endif

#endif	/* GOBJECT_H */


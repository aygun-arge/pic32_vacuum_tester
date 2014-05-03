/* 
 * File:   gprimitives.h
 * Author: nenad
 *
 * Created on May 3, 2014, 11:46 AM
 */

#ifndef GPRIMITIVES_H
#define	GPRIMITIVES_H

#include <stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

enum gThemeTypes {
    GO_BACKGROUND,
    GO_BUTTON,
    GO_TEXT,
    GO_PICTURE,
    GO_PAGE_END
};

enum gThemeSizes {
    GO_SIZE_XSMALL,
    GO_SIZE_SMALL,
    GO_SIZE_MEDIUM,
    GO_SIZE_BIG,
    GO_SIZE_XBIG
};

enum gThemeColors {
    GO_COLOR_LRED,
    GO_COLOR_LGREEN,
    GO_COLOR_LBLUE,
    GO_COLOR_LORANGE,
    GO_COLOR_LVIOLET,
    GO_COLOR_DRED,
    GO_COLOR_DGREEN,
    GO_COLOR_DBLUE,
    GO_COLOR_DORANGE,
    GO_COLOR_DVIOLET,
    GO_COLOR_WHITE,
    GO_COLOR_BLACK
};

enum gHAlignments {
    GO_H_ALIGN_LEFT   = (0x0 << 16),
    GO_H_ALIGN_CENTER = (0x1 << 16),
    GO_H_ALIGN_RIGHT  = (0x2 << 16)
};

enum gVAlignments {
    GO_V_ALIGN_TOP    = (0x0 << 0),
    GO_V_ALIGN_CENTER = (0x1 << 1),
    GO_V_ALIGN_BOTTOM = (0x2 << 2)
};

struct gPosition {
    uint16_t            x;
    uint16_t            y;
};

struct gSize {
    uint16_t            width;
    uint16_t            height;
};

struct gAlignment {
    enum gHAlignments   hAlign;
    enum gVAlignments   vAlign;
};

struct gGeometry {
    struct gPosition    position;
    struct gSize        size;
    struct gAlignment   alignment;
};

struct gAttribute {
    bool                isVisible;
    uint16_t            eventGenerator;
};

struct gColor {
    uint32_t            rgb;
};

struct gObject;
struct gHalPage;

struct gMethod {
    void                  (* init)(struct gObject *);
    const struct gColor * (* themedColor)(enum gThemeColors);
    uint16_t              (* themedWidth)(enum gThemeSizes);
    uint16_t              (* themedHeight)(enum gThemeSizes);
    void                  (* propertySetLabel)(struct gObject *, const char *);
    void                  (* propertySetFont)(struct gObject *, uint16_t);
    void                  (* draw)(struct gHalPage *, const struct gObject *);
};

struct gObject {
    uint16_t                id;
    const struct gColor *   color;
    const struct gMethod *  method;
    struct gGeometry        geometry;
    struct gAttribute       attribute;
    void *                  property;
};

#ifdef	__cplusplus
}
#endif

#endif	/* GPRIMITIVES_H */


/* 
 * File:   ghal.h
 * Author: nenad
 *
 * Created on May 2, 2014, 6:23 PM
 */

#ifndef GHAL_H
#define	GHAL_H

#include <stdint.h>
#include <stdbool.h>

#include "gprimitives.h"

#ifdef	__cplusplus
extern "C" {
#endif

struct gHalTouchCalibration {
    uint16_t            threshold;
    uint16_t            a;
    uint16_t            b;
    uint16_t            c;
    uint16_t            d;
    uint16_t            e;
    uint16_t            f;
};

struct gHalPage;

void gHalInit(void);
void gHalGpuInit(void);
void gHalGpuEnable(void);
bool gHalIsGpuReady(void);
void gHalTouchSetCalibration(const struct gHalTouchCalibration * calibration);
void gHalTouchGetCalibration(struct gHalTouchCalibration * calibration);

struct gHalPage * gHalPageCreate(void);
void gHalPageFlush(struct gHalPage *  halPage);
void gHalSetHotspot(struct gHalPage * halPage, const struct gGeometry * geometry, uint16_t id);
uint16_t gHalGetHotSpot(struct gHalPage * halPage);
bool gHalIsPressed(struct gHalPage *  halPage);
bool gHalIsReleased(struct gHalPage * halPage);
bool gHalIsClicked(struct gHalPage *  halPage);
void gHalButton(struct gHalPage *     halPage, const struct gGeometry * geometry, const struct gColor * color, const char * label, uint16_t font);
void gHalText(struct gHalPage *       halPage, const struct gGeometry * geometry, const struct gColor * color, const char * label, uint16_t font);
void gHalBackground(struct gHalPage * halPage, const struct gColor * color);

#ifdef	__cplusplus
}
#endif

#endif	/* GHAL_H */


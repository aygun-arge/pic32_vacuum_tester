/* 
 * File:   logo.h
 * Author: nenad
 *
 * Created on April 16, 2014, 6:40 PM
 */

#ifndef LOGO_H
#define	LOGO_H

#include <stdint.h>
#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif

struct bitmapInfo {
    uint32_t            format;
    uint32_t            linestride;
    uint32_t            height;
    uint32_t            pixelsX;
    uint32_t            pixelsY;
    size_t              size;
};

extern const char ManufacturerLogo[];
extern const struct bitmapInfo ManufacturerLogoInfo;

#ifdef	__cplusplus
}
#endif

#endif	/* LOGO_H */


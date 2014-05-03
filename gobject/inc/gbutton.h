/* 
 * File:   gbutton.h
 * Author: nenad
 *
 * Created on May 2, 2014, 11:46 PM
 */

#ifndef GBUTTON_H
#define	GBUTTON_H

#include "gprimitives.h"

#ifdef	__cplusplus
extern "C" {
#endif

struct gButtonProperty {
    const char *        label;
    uint16_t            font;
};


extern const struct gMethod ButtonMethods;

#ifdef	__cplusplus
}
#endif

#endif	/* GBUTTON_H */


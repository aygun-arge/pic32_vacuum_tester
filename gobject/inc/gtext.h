/* 
 * File:   gtext.h
 * Author: nenad
 *
 * Created on May 3, 2014, 11:54 AM
 */

#ifndef GTEXT_H
#define	GTEXT_H

#include "gprimitives.h"

#ifdef	__cplusplus
extern "C" {
#endif

struct gTextProperty {
    const char *        label;
    uint16_t            font;
};

extern const struct gMethod TextMethods;

#ifdef	__cplusplus
}
#endif

#endif	/* GTEXT_H */


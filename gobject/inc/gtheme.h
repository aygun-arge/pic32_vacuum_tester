/* 
 * File:   gtheme.h
 * Author: nenad
 *
 * Created on May 2, 2014, 7:19 PM
 */

#ifndef GTHEME_H
#define	GTHEME_H

#include "gprimitives.h"


#ifdef	__cplusplus
extern "C" {
#endif

/*==  PREDEFINED COLORS  =====================================================*/
#define COLOR_LRED                      0xff4444
#define COLOR_LGREEN                    0x99cc00
#define COLOR_LBLUE                     0x33b5e5
#define COLOR_LVIOLET                   0xaa66cc
#define COLOR_LORANGE                   0xffbb33

#define COLOR_DRED                      0xcc0000
#define COLOR_DGREEN                    0x669900
#define COLOR_DBLUE                     0x0099cc
#define COLOR_DVIOLET                   0x9933cc
#define COLOR_DORANGE                   0xff8800

#define COLOR_WHITE                     0xffffff
#define COLOR_BLACK                     0x000000

/*==  BACKGROUND  ============================================================*/
/*--  Colors: top color, bottom color  ---------------------------------------*/
#define BACKGROUND_COLOR_TABLE(entry)                                           \
    entry(GO_COLOR_LRED,    COLOR_DRED,     COLOR_LRED)                         \
    entry(GO_COLOR_LGREEN,  COLOR_DGREEN,   COLOR_LGREEN)                       \
    entry(GO_COLOR_LBLUE,   COLOR_DBLUE,    COLOR_LBLUE)                        \
    entry(GO_COLOR_WHITE,   0xfefefe,       COLOR_WHITE)                        \

/*==  BUTTON  ================================================================*/

/*--  Dimensions: X dp, Y dp  ------------------------------------------------*/
#define BUTTON_SIZE_TABLE(entry)                                                \
    entry(GO_SIZE_XSMALL,   40,     40)                                         \
    entry(GO_SIZE_SMALL,    80,     40)                                         \
    entry(GO_SIZE_MEDIUM,   130,    40)                                         \
    entry(GO_SIZE_BIG,      130,    60)                                         \
    entry(GO_SIZE_XBIG,     160,    80)

/*--  Colors: text, gradient, foreground, background  ------------------------*/
#define BUTTON_COLOR_TABLE(entry)                                                           \
    entry(GO_COLOR_LRED,    COLOR_WHITE,    COLOR_WHITE,    COLOR_LRED,     COLOR_DRED)     \
    entry(GO_COLOR_LGREEN,  COLOR_WHITE,    COLOR_WHITE,    COLOR_LGREEN,   COLOR_DGREEN)   \
    entry(GO_COLOR_LBLUE,   COLOR_WHITE,    COLOR_WHITE,    COLOR_LBLUE,    COLOR_DBLUE)    \

#define BUTTON_COLOR_LIGHT_MODIFIER     0x20
#define BUTTON_COLOR_DARK_MODIFIER      0x20

/*==  TEXT  ==================================================================*/

/*--  Text size: font, dp  ---------------------------------------------------*/
#define TEXT_SIZE_TABLE(entry)                                                  \
    entry(GO_SIZE_XSMALL,   26,     12)                                         \
    entry(GO_SIZE_SMALL,    27,     13)                                         \
    entry(GO_SIZE_MEDIUM,   28,     14)                                         \
    entry(GO_SIZE_BIG,      29,     15)                                         \
    entry(GO_SIZE_XBIG,     30,     16)

#define TEXT_COLOR_TABLE(entry)                                                 \
    entry(GO_COLOR_LRED,    COLOR_LRED)                                         \
    entry(GO_COLOR_LGREEN,  COLOR_LGREEN)                                       \
    entry(GO_COLOR_LBLUE,   COLOR_LBLUE)    


#ifdef	__cplusplus
}
#endif

#endif	/* GTHEME_H */


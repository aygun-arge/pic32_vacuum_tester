/* 
 * File:   epa_gui.h
 * Author: nenad
 *
 * Created on April 15, 2014, 7:42 PM
 */

#ifndef EPA_GUI_H
#define	EPA_GUI_H

#include "events.h"
#include "eds/epa.h"

#define CONFIG_EPA_GUI_PRIORITY         29
#define CONFIG_EPA_GUI_QUEUE_SIZE       10
#define CONFIG_EPA_GUI_EVENT_BASE       1000
#define CONFIG_EPA_GUI_NAME             "Graphical User Interface manager"

#define CONFIG_TIME_WELCOME             2000

#ifdef	__cplusplus
extern "C" {
#endif

extern const struct esEpaDefine GuiEpa;
extern const struct esSmDefine  GuiSm;
extern struct esEpa *           Gui;

#ifdef	__cplusplus
}
#endif

#endif	/* EPA_GUI_H */


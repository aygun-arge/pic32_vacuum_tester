/* 
 * File:   app_battery.h
 * Author: nenad
 *
 * Created on April 5, 2014, 11:11 PM
 */

#ifndef APP_BATTERY_H
#define	APP_BATTERY_H

#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

void initBatteryModule(void);
bool batteryIsPgHigh(void);
bool batteryIsStat1High(void);
bool batteryIsStat2High(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_BATTERY_H */


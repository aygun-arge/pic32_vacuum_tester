/* 
 * File:   app_battery.h
 * Author: nenad
 *
 * Created on April 5, 2014, 11:11 PM
 */

#ifndef APP_BATTERY_H
#define	APP_BATTERY_H

#include <stdint.h>
#include <stdbool.h>

#define CONFIG_TEXT_OPTIMIZING          "optimize"
#define CONFIG_TEXT_CHARGING            "charging"
#define CONFIG_TEXT_DISCHARGING         "discharging"
#define CONFIG_TEXT_MISSING             "missing"
#define CONFIG_TEXT_EMPTY               "empty"
#define CONFIG_TEXT_FULL                "full"
#define CONFIG_TEXT_FAULT               "fault"

#ifdef	__cplusplus
extern "C" {
#endif

enum batteryStatus {
    BATT_OPTIMIZING,
    BATT_CHARGING,
    BATT_DISCHARING,
    BATT_MISSING,
    BATT_EMPTY,
    BATT_FULL,
    BATT_FAULT
};

void initBatteryModule(void);
bool batteryIsPgHigh(void);
bool batteryIsStat1High(void);
bool batteryIsStat2High(void);
enum batteryStatus getBatteryStatus(void);
uint32_t snprintBatteryStatus(char * buffer);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_BATTERY_H */


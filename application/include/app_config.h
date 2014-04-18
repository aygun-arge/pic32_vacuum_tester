/* 
 * File:   app_config.h
 * Author: nenad
 *
 * Created on April 3, 2014, 11:43 PM
 */

#ifndef APP_CONFIG_H
#define	APP_CONFIG_H

#include <stdint.h>

#define CONFIG_HARDWARE_VERSION         "Rev. 02"
#define CONFIG_SOFTWARE_VERSION         "1.0 Beta"

#define PATCH_LEVEL_POWER_STATUS_1      1

#ifdef	__cplusplus
extern "C" {
#endif

void configGetGpuCalibrate();

uint32_t getFirstThTimeout(void);
uint32_t getFirstThRawVacuum(void);
uint32_t getSecondThTimeout(void);
uint32_t getSecondThValue(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_CONFIG_H */


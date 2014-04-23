/* 
 * File:   app_config.h
 * Author: nenad
 *
 * Created on April 3, 2014, 11:43 PM
 */

#ifndef APP_CONFIG_H
#define	APP_CONFIG_H

#include <stdint.h>



#define PATCH_LEVEL_POWER_STATUS_1      1

#ifdef	__cplusplus
extern "C" {
#endif

void configGetGpuCalibrate();

uint32_t configGetFirstThTimeout(void);
uint32_t configGetFirstThRawVacuum(void);
uint32_t configGetSecondThTimeout(void);
uint32_t configGetSecondThValue(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_CONFIG_H */


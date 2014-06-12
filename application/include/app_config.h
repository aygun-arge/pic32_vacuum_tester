/* 
 * File:   app_config.h
 * Author: nenad
 *
 * Created on April 3, 2014, 11:43 PM
 */

#ifndef APP_CONFIG_H
#define	APP_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#include "app_storage.h"

#define PATCH_LEVEL_POWER_STATUS_1      1

#ifdef	__cplusplus
extern "C" {
#endif

extern const struct storageEntry ConfigStorage;

void configGetGpuCalibrate();

void initAppConfig(void);
bool configSetRawIdleVacuum(uint32_t rawVacuum);
bool configSetFirstThTimeout(uint32_t timeoutMs);
bool configSetFirstThRawVacuum(uint32_t rawVacuum);
bool configSetSecondThTimeout(uint32_t timeoutMs);
bool configSetSecondThRawVacuum(uint32_t rawVacuum);

uint32_t configGetRawIdleVacuum(void);
uint32_t configGetFirstThTimeout(void);
uint32_t configGetFirstThRawVacuum(void);
uint32_t configGetSecondThTimeout(void);
uint32_t configGetSecondThRawVacuum(void);
bool configIsPasswordCharValid(char character, uint8_t position);
uint32_t configPasswordLength(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_CONFIG_H */


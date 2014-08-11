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
bool configSetTh0Timeout(uint32_t timeoutMs);
bool configSetTh0RawVacuum(uint32_t rawVacuum);
bool configSetTh1Timeout(uint32_t timeoutMs);
bool configSetTh1RawVacuum(uint32_t rawVacuum);

uint32_t configGetTh0Timeout(void);
uint32_t configGetTh0RawVacuum(void);
uint32_t configGetTh1Timeout(void);
uint32_t configGetTh1RawVacuum(void);
uint32_t configGetTh0DefaultTimeout(void);
uint32_t configGetTh0DefaultRawVacuum(void);
uint32_t configGetTh1DefaultTimeout(void);
uint32_t configGetTh1DefaultRawVacuum(void);
uint32_t configGetRetryCount(void);
bool configIsPasswordCharValid(char character, uint8_t position);
uint32_t configPasswordLength(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_CONFIG_H */


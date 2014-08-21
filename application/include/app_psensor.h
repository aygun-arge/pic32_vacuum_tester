/* 
 * File:   app_psensor.h
 * Author: nenad
 *
 * Created on April 5, 2014, 12:32 PM
 */

#ifndef APP_PSENSOR_H
#define	APP_PSENSOR_H

#include <stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

void initPSensorModule(void);
uint32_t getDutRawValue(void);
bool isDutFirstThresholdValid(void);
bool isDutSecondhTresholdValid(void);
void newDut(uint32_t firstTreshold, uint32_t secondTreshold);
void updateDutFirstTh(void);
void updateDutSecondTh(void);

uint32_t dutRawToMm(uint32_t rawValue);
uint32_t dutMmToRaw(uint32_t mmValue);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_PSENSOR_H */


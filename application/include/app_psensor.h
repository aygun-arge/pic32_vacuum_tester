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
void setDutFirstTreshold(uint32_t treshold);
uint32_t getDutRawValue(void);
bool isDutDetected(void);
bool isDutFirstTresholdValid(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_PSENSOR_H */


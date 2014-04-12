/* 
 * File:   app_psensor.h
 * Author: nenad
 *
 * Created on April 5, 2014, 12:32 PM
 */

#ifndef APP_PSENSOR_H
#define	APP_PSENSOR_H

#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

void initPSensorModule(void);
bool isDutDetected(void);
bool isDutVacuumValid(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_PSENSOR_H */


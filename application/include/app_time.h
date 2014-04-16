/* 
 * File:   app_time.h
 * Author: nenad
 *
 * Created on April 15, 2014, 9:50 PM
 */

#ifndef APP_TIME_H
#define	APP_TIME_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

uint32_t snprintRtcTime(char * buffer);
uint32_t snprintRtcDate(char * buffer);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_TIME_H */


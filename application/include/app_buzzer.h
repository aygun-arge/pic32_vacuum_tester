/* 
 * File:   app_buzzer.h
 * Author: nenad
 *
 * Created on April 5, 2014, 11:25 PM
 */

#ifndef APP_BUZZER_H
#define	APP_BUZZER_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

void initBuzzerModule(void);
void buzzerTone(uint32_t duration);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_BUZZER_H */


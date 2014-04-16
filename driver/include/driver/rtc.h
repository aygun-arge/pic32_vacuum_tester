/* 
 * File:   rtc.h
 * Author: nenad
 *
 * Created on April 15, 2014, 12:13 AM
 */

#ifndef RTC_H
#define	RTC_H

#include <stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

struct rtcTime {
    uint16_t            year;
    uint8_t             month;
    uint8_t             day;
    uint8_t             hour;
    uint8_t             minute;
    uint8_t             second;
};

void initRtcDriver(void);
void termRtcDriver(void);
void rtcGetTime(struct rtcTime * time);
bool isRtcActive(void);

#ifdef	__cplusplus
}
#endif

#endif	/* RTC_H */


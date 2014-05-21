/* 
 * File:   nv_config.h
 * Author: nenad
 *
 * Created on May 21, 2014, 9:49 AM
 */

#ifndef NV_CONFIG_H
#define	NV_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#define NV_ID_TOUCH_CALIBRATION         0
#define NV_ID_USER_ACCOUNTS             1
#define NV_ID_USER_INTERFACE            2
#define NV_ID_MEAS_CALIBRATION          3
#define NV_ID_LOG_STATISTICS            4

#define FLASH_LAYOUT(entry)                                                     \
    entry(NV_ID_TOUCH_CALIBRATION,      0x1000)                                 \
    entry(NV_ID_USER_ACCOUNTS,          0x1000)                                 \
    entry(NV_ID_USER_INTERFACE,         0x1000)                                 \
    entry(NV_ID_MEAS_CALIBRATION,       0x1000)                                 \
    entry(NV_ID_LOG_STATISTICS,         0x1000)


#ifdef	__cplusplus
}
#endif

#endif	/* NV_CONFIG_H */


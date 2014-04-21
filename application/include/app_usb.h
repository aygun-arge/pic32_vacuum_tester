/* 
 * File:   app_usb.h
 * Author: nenad
 *
 * Created on April 3, 2014, 11:49 PM
 */

#ifndef APP_USB_H
#define	APP_USB_H

#include <stdint.h>

#define CONFIG_TEXT_USB_DETECTED        "detected"
#define CONFIG_TEXT_USB_NOT_DETECTED    "not detected"

#ifdef	__cplusplus
extern "C" {
#endif

void initUsbModule(void);
void appUsb(void);
bool isUsbDetected(void);
bool isUsbMounted(void);
uint32_t snprintUsbStatus(char * buffer);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_USB_H */


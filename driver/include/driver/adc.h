/* 
 * File:   adc.h
 * Author: nenad
 *
 * Created on April 5, 2014, 2:25 PM
 */

#ifndef ADC_H
#define	ADC_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

void initAdcDriver(void);
void adcEnableChannel(uint32_t id, void (* callback)(int32_t));
void adcDisableChannel(uint32_t id);
int32_t adcReadChannel(uint32_t id);

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */


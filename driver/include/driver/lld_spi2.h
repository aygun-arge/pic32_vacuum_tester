/* 
 * File:    lld_spi1.h
 * Author:  nenad
 * Details: Low Level Driver for SPI2
 *
 * Created on March 31, 2014, 9:38 PM
 */

#ifndef LLD_SPI2_H_
#define	LLD_SPI2_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "driver/spi.h"

/*===============================================================  MACRO's  ==*/

#if   (((__PIC32_FEATURE_SET__ >= 100) && (__PIC32_FEATURE_SET__ <= 299)))
#define SPI2_PIN_TABLE(entry)                                                   \
    entry(SPI2_SDI_RPB2,            0x04,           NULL)                       \
    entry(SPI2_SDI_RPB6,            0x01,           NULL)                       \
    entry(SPI2_SDI_RPC4,            0x01,           NULL)                       \
    entry(SPI2_SDO_RPB5,            0,              &RPB5R)                     \
    entry(SPI2_SDO_RPC3,            0,              &RPC3R)                     \
    entry(SPI2_SS_RPC2,             0,              &RPC2R)                     \
    entry(SPI2_SS_RPC2_GPIO,        2,              &GpioC)
#elif (((__PIC32_FEATURE_SET__ >= 300) && (__PIC32_FEATURE_SET__ <= 499)))
#elif (((__PIC32_FEATURE_SET__ >= 500) && (__PIC32_FEATURE_SET__ <= 799)))
#endif

#define SPI2_PIN_ID(id, value, address)                                         \
    id,

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum spi2PinId {
    SPI2_PIN_TABLE(SPI2_PIN_ID)
    SPI2_LAST_PIN_ID
};

/*======================================================  GLOBAL VARIABLES  ==*/

/**@brief       Hardware SPI2 identificator
 */
extern const struct spiId GlobalSpi2;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of lld_spi1.h
 ******************************************************************************/
#endif /* LLD_SPI2_H_ */

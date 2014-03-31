/* 
 * File:    lld_spi1.h
 * Author:  nenad
 * Details: Low Level Driver for SPI1
 *
 * Created on February 11, 2014, 10:28 PM
 */

#ifndef LLD_SPI1_H_
#define	LLD_SPI1_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "driver/spi.h"

/*===============================================================  MACRO's  ==*/

#if   (((__PIC32_FEATURE_SET__ >= 100) && (__PIC32_FEATURE_SET__ <= 299)))
#define SPI1_PIN_TABLE(entry)                                                   \
    entry(SPI1_SDI_RPA1,            0x00,           NULL)                       \
    entry(SPI1_SDI_RPB5,            0x01,           NULL)                       \
    entry(SPI1_SDO_RPA2,            0,              &RPA2R)                     \
    entry(SPI1_SS_EXAMPLE,          0,              &RPA2R)
#elif (((__PIC32_FEATURE_SET__ >= 300) && (__PIC32_FEATURE_SET__ <= 499)))
#elif (((__PIC32_FEATURE_SET__ >= 500) && (__PIC32_FEATURE_SET__ <= 799)))
#define SPI1_PIN_TABLE(entry)                                                   \
    entry(SPI1_SCK,                 0,              &PORTC)
#endif

#define SPI1_PIN_ID(id, value, address)                                         \
    id,

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum spi1PinId {
    SPI1_PIN_TABLE(SPI1_PIN_ID)
    SPI1_LAST_PIN_ID
};

/*======================================================  GLOBAL VARIABLES  ==*/

/**@brief       Hardware SPI1 identificator
 */
extern const struct spiId GlobalSpi1;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of lld_spi1.h
 ******************************************************************************/
#endif /* LLD_SPI1_H_ */

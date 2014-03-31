/*
 * File:    lld_spis.h
 * Author:  nenad
 * Details: Low Level Driver for Software SPI
 *
 * Created on February 14, 2014, 8:27 PM
 */

#ifndef LLD_SPIS_H_
#define	LLD_SPIS_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "driver/spi.h"

/*===============================================================  MACRO's  ==*/

#if   (((__PIC32_FEATURE_SET__ >= 100) && (__PIC32_FEATURE_SET__ <= 299)))
#define SPIS_PIN_TABLE(entry)                                                   \
    entry(SPIS_SDI_C6,      6,  &PORTC,     &TRISC,     &PORTCCLR,  &PORTCSET)  \
    entry(SPIS_SDI_C7,      7,  &PORTC,     &TRISC,     &PORTCCLR,  &PORTCSET)  \
    entry(SPIS_SDO_C8,      8,  &PORTC,     &TRISC,     &PORTCCLR,  &PORTCSET)  \
    entry(SPIS_SDO_B8,      8,  &PORTB,     &TRISB,     &PORTBCLR,  &PORTBSET)  \
    entry(SPIS_SCK_B9,      9,  &PORTB,     &TRISB,     &PORTBCLR,  &PORTBSET)  \
    entry(SPIS_SS_B10,      10, &PORTB,     &TRISB,     &PORTBCLR,  &PORTBSET)
#elif (((__PIC32_FEATURE_SET__ >= 300) && (__PIC32_FEATURE_SET__ <= 499)))
#elif (((__PIC32_FEATURE_SET__ >= 500) && (__PIC32_FEATURE_SET__ <= 799)))
#define SPIS_PIN_TABLE(entry)                                                   \
    entry(SPIS_SCK_A9,      9,  &PORTA,     &TRISA,     &PORTACLR,  &PORTASET)  \
    entry(SPIS_SCK_B5,      5,  &PORTB,     &TRISB,     &PORTBCLR,  &PORTBSET)  \
    entry(SPIS_SCK_D9,      9,  &PORTD,     &TRISD,     &PORTDCLR,  &PORTDSET)  \
    entry(SPIS_SDO_A0,      0,  &PORTA,     &TRISA,     &PORTACLR,  &PORTASET)  \
    entry(SPIS_SDO_A1,      1,  &PORTA,     &TRISA,     &PORTACLR,  &PORTASET)  \
    entry(SPIS_SDO_B4,      4,  &PORTB,     &TRISB,     &PORTBCLR,  &PORTBSET)  \
    entry(SPIS_SDO_B5,      5,  &PORTB,     &TRISB,     &PORTBCLR,  &PORTBSET)  \
    entry(SPIS_SDI_A0,      0,  &PORTA,     &TRISA,     &PORTACLR,  &PORTASET)  \
    entry(SPIS_SDI_A1,      1,  &PORTA,     &TRISA,     &PORTACLR,  &PORTASET)  \
    entry(SPIS_SDI_A9,      9,  &PORTA,     &TRISA,     &PORTACLR,  &PORTASET)  \
    entry(SPIS_SDI_B4,      4,  &PORTB,     &TRISB,     &PORTBCLR,  &PORTBSET)  \
    entry(SPIS_SDI_B5,      5,  &PORTB,     &TRISB,     &PORTBCLR,  &PORTBSET)  \
    entry(SPIS_SS_A1,       1,  &PORTA,     &TRISA,     &PORTACLR,  &PORTASET)  \
    entry(SPIS_SS_A10,      10,  &PORTA,    &TRISA,     &PORTACLR,  &PORTASET)
#endif

#define SPIS_PIN_ID(id, bit, port, tris, clr, set)                              \
    id,

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum spisPinId {
    SPIS_PIN_TABLE(SPIS_PIN_ID)
    SPIS_LAST_PIN_ID
};

/*======================================================  GLOBAL VARIABLES  ==*/

/**@brief       Software SPI identifier
 */
extern const struct spiId SpiSoft;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of lld_spis.h
 ******************************************************************************/
#endif /* LLD_SPIS_H_ */

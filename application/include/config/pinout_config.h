/* 
 * File:   pinout_config.h
 * Author: nenad
 *
 * Created on August 10, 2014, 7:34 PM
 */

#ifndef PINOUT_CONFIG_H
#define	PINOUT_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#define CONFIG_STAT1_PORT               &GpioC
#define CONFIG_STAT1_PIN                8

#define CONFIG_STAT2_PORT               &GpioC
#define CONFIG_STAT2_PIN                9

#define CONFIG_PG_PORT                  &GpioB
#define CONFIG_PG_PIN                   13

#define CONFIG_BUZZER_GPIO_PORT         &GpioB
#define CONFIG_BUZZER_GPIO_PIN          1

#define CONFIG_FT800_INT_PORT           &GpioB
#define CONFIG_FT800_INT_PIN            7
#define CONFIG_FT800_PD_N_PORT          &GpioB
#define CONFIG_FT800_PD_N_PIN           3

#define CONFIG_FT800_SPI                &GlobalSpi2
#define CONFIG_FT800_SPI_SDI            SPI2_SDI_RPB2
#define CONFIG_FT800_SPI_SDO            SPI2_SDO_RPB5
#define CONFIG_FT800_SPI_CS             0
#define CONFIG_FT800_SPI_SS             SPI2_SS_RPC2_GPIO

#define CONFIG_MSENSOR_GPIO_PORT        &GpioC
#define CONFIG_MSENSOR_GPIO_PIN         0
#define CONFIG_MSENSOR_AD_CHANNEL       6

#define CONFIG_MDRIVE_GPIO_PORT         &GpioA
#define CONFIG_MDRIVE_GPIO_PIN          4

#define CONFIG_MDRIVE_POWER_PORT        &GpioA
#define CONFIG_MDRIVE_POWER_PIN         10

#define CONFIG_PDETECTOR_PORT           &GpioA
#define CONFIG_PDETECTOR_PIN            9

#define CONFIG_PSENSOR_GPIO_PORT        &GpioB
#define CONFIG_PSENSOR_GPIO_PIN         0
#define CONFIG_PSENSOR_ADC_CHANNEL      2

#define CONFIG_S25_SPI_MODULE           &SpiSoft
#define CONFIG_S25FL_SDI                SPIS_SDI_C4
#define CONFIG_S25FL_SDO                SPIS_SDO_C3
#define CONFIG_S25FL_SCK                SPIS_SCK_C6
#define CONFIG_S25FL_SS                 SPIS_SS_C5


#ifdef	__cplusplus
}
#endif

#endif	/* PINOUT_CONFIG_H */


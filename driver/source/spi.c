/*
 * File:    spi.c
 * Author:  nenad
 * Details: Generic SPI driver
 *
 * Created on February 6, 2014, 7:12 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "driver/spi.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define SPI_DATA_Msk                    (0x3 << 10)

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initSpiDriver(
    void) {
    /*
     * NOTE: This is the place to do general and system wide initializaion
     */
}

void spiOpen(
    struct spiHandle *        handle,
    const struct spiConfig *  config) {

    handle->id    = config->id;
    handle->flags = config->flags;
    handle->id->open(config, handle);
}

void spiClose(
    struct spiHandle * handle) {

    handle->id->close(handle);
}

void spiExchange(
    struct spiHandle *  handle,
    void *              buffer,
    size_t              nElements) {

    size_t              transmitted;

    transmitted = 0u;

    switch (handle->flags & SPI_DATA_Msk) {
        case SPI_DATA_8: {
            uint8_t *  buffer_ = (uint8_t *)buffer;

            while (transmitted < nElements) {
                if (handle->id->isBuffFull(handle) != true) {
                    buffer_[transmitted] = handle->id->exchange(handle, buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
        case SPI_DATA_16 : {
            uint16_t *  buffer_ = (uint16_t *)buffer;

            while (transmitted < nElements) {
                if (handle->id->isBuffFull(handle) != true) {
                    buffer_[transmitted] = handle->id->exchange(handle, buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
        default : {
            uint32_t *  buffer_ = (uint32_t *)buffer;

            while (transmitted < nElements) {
                if (handle->id->isBuffFull(handle) != true) {
                    buffer_[transmitted] = handle->id->exchange(handle, buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
    }
}

void spiWrite(
    struct spiHandle *  handle,
    const void *        buffer,
    size_t              nElements) {

    size_t              transmitted;

    transmitted = 0u;

    switch (handle->flags & SPI_DATA_Msk) {
        case SPI_DATA_8: {
            const uint8_t *  buffer_ = (const uint8_t *)buffer;

            while (transmitted < nElements) {
                if (handle->id->isBuffFull(handle) != true) {
                    (void)handle->id->exchange(handle, buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
        case SPI_DATA_16 : {
            uint16_t *  buffer_ = (uint16_t *)buffer;

            while (transmitted < nElements) {
                if (handle->id->isBuffFull(handle) != true) {
                    (void)handle->id->exchange(handle, buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
        default : {
            uint32_t *  buffer_ = (uint32_t *)buffer;

            while (transmitted < nElements) {
                if (handle->id->isBuffFull(handle) != true) {
                    (void)handle->id->exchange(handle, buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
    }
}

void spiSSActivate(
    struct spiHandle *  handle) {

    handle->id->ssActivate(handle);
}

void spiSSDeactivate(
    struct spiHandle *  handle) {
    
    handle->id->ssDeactivate(handle);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of spi.c
 ******************************************************************************/


#include "driver/spi.h"
#include <stddef.h>

#define CONFIG_S25_SPI_MODULE           &SpiSoft
#define CONFIG_S25FL_SDI                SPIS_SDI_C4
#define CONFIG_S25FL_SDO                SPIS_SDO_C3
#define CONFIG_S25FL_SCK                SPIS_SCK_C6
#define CONFIG_S25FL_SS                 SPIS_SS_C5

#define CMD_RDID                        0x9fu

static struct spiHandle FlashSpi;

static void flashExchange(void * buffer, size_t size) {
    spiSSActivate(&FlashSpi);
    spiExchange(&FlashSpi, buffer, size);
    spiSSDeactivate(&FlashSpi);
}

int32_t initFlash(void) {
    char                data[6];

    struct spiConfig spiConfig = {
        CONFIG_S25_SPI_MODULE,
        SPI_MASTER_MODE             | SPI_MASTER_SS_ACTIVE_LOW   |
        SPI_SLAVE_MODE              |
        SPI_CLOCK_POLARITY_IDLE_LOW | SPI_CLOCK_PHASE_FIRST_EDGE |
        SPI_DATA_8,
        1000000u,
        4,
        {
            CONFIG_S25FL_SDI,
            CONFIG_S25FL_SDO,
            CONFIG_S25FL_SCK,
            CONFIG_S25FL_SS
        }
    };
    spiOpen(&FlashSpi, &spiConfig);

    data[0] = CMD_RDID;
    flashExchange(&data, sizeof(data));

    if (data[1] != 0x1u) {
        spiClose(&FlashSpi);

        return (-1);
    } else {

        return (0);
    }
}




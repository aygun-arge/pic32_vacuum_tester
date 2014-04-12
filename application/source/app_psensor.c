
#include <stddef.h>

#include "main.h"

#include "app_psensor.h"
#include "driver/gpio.h"
#include "driver/adc.h"

#define CONFIG_PSENSOR_GPIO_PORT        &GpioB
#define CONFIG_PSENSOR_GPIO_PIN         0
#define CONFIG_PDETECT_GPIO_PORT        &GpioA
#define CONFIG_PDETECT_GPIO_PIN         9
#define CONFIG_PSENSOR_ADC_CHANNEL      2

void initPSensorModule(void) {
    *(CONFIG_PSENSOR_GPIO_PORT)->tris     |= (0x1u << CONFIG_PSENSOR_GPIO_PIN);
    *(CONFIG_PSENSOR_GPIO_PORT)->ansel    |= (0x1u << CONFIG_PSENSOR_GPIO_PIN);
    *(CONFIG_PDETECT_GPIO_PORT)->tris     |= (0x1u << CONFIG_PDETECT_GPIO_PIN);
    *(CONFIG_PDETECT_GPIO_PORT)->pulldown |= (0x1u << CONFIG_PDETECT_GPIO_PIN);

    adcEnableChannel(CONFIG_PSENSOR_ADC_CHANNEL, NULL);
}

bool isDutDetected(void) {

    if ((*(CONFIG_PDETECT_GPIO_PORT)->port & (0x1u << CONFIG_PDETECT_GPIO_PIN)) != 0u) {

        return (true);
    } else {

        return (false);
    }
}

bool isDutVacuumValid(void) {

    //int32_t             sensor;

    g_sensor = adcReadChannel(CONFIG_PSENSOR_ADC_CHANNEL);

    return (false);
}

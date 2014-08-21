
#include <stddef.h>

#include "main.h"

#include "app_psensor.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "config/pinout_config.h"


static uint32_t FirstTreshold;
static uint32_t SecondTreshold;
static uint32_t IdleVacuum;
static uint32_t MaxFirstVacuum;
static uint32_t MaxSecondVacuum;

void initPSensorModule(void) {
    *(CONFIG_PSENSOR_GPIO_PORT)->tris     |= (0x1u << CONFIG_PSENSOR_GPIO_PIN);
    *(CONFIG_PSENSOR_GPIO_PORT)->ansel    |= (0x1u << CONFIG_PSENSOR_GPIO_PIN);

    adcEnableChannel(CONFIG_PSENSOR_ADC_CHANNEL, NULL);
}

uint32_t getDutRawValue(void) {

    return (adcReadChannel(CONFIG_PSENSOR_ADC_CHANNEL));
}

bool isDutFirstThresholdValid(void) {

    if (MaxFirstVacuum > FirstTreshold) {

        return (true);
    } else {

        return (false);
    }
}

bool isDutSecondhTresholdValid(void) {

    if (MaxSecondVacuum > SecondTreshold) {

        return (true);
    } else {

        return (false);
    }
}

void newDut(uint32_t firstTreshold, uint32_t secondTreshold) {
    uint32_t            idle;

    idle = getDutRawValue();
    IdleVacuum      = idle;
    MaxFirstVacuum  = 0;
    MaxSecondVacuum = 0;
    FirstTreshold   = firstTreshold;
    SecondTreshold  = secondTreshold;
}

void updateDutFirstTh(void) {
    uint32_t current;

    current = IdleVacuum - getDutRawValue();

    if (MaxFirstVacuum < current ) {
        MaxFirstVacuum = current;
    }
}

void updateDutSecondTh(void) {
    uint32_t current;

    current = IdleVacuum - getDutRawValue();

    if (MaxSecondVacuum < current ) {
        MaxSecondVacuum = current;
    }
}

uint32_t dutRawToMm(uint32_t rawValue) {

    return (rawValue);
}

uint32_t dutMmToRaw(uint32_t mmValue) {

    return (mmValue);
}


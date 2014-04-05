
#include <xc.h>
#include <stddef.h>

#include "app_motor.h"
#include "driver/gpio.h"
#include "driver/adc.h"

#define CONFIG_MSENSOR_GPIO_PORT        &GpioC
#define CONFIG_MSENSOR_GPIO_PIN         0
#define CONFIG_MSENSOR_AD_CHANNEL       6

#define CONFIG_MDRIVE_GPIO_PORT         &GpioA
#define CONFIG_MDRIVE_GPIO_PIN          4

void initMotorModule(void) {
    *(CONFIG_MSENSOR_GPIO_PORT)->tris  |=  (0x1u << CONFIG_MSENSOR_GPIO_PIN);
    *(CONFIG_MSENSOR_GPIO_PORT)->ansel |=  (0x1u << CONFIG_MSENSOR_GPIO_PIN);
    *(CONFIG_MDRIVE_GPIO_PORT)->tris   &= ~(0x1u << CONFIG_MDRIVE_GPIO_PIN);
    *(CONFIG_MDRIVE_GPIO_PORT)->clr     =  (0x1u << CONFIG_MDRIVE_GPIO_PIN);

    adcEnableChannel(CONFIG_MSENSOR_AD_CHANNEL, NULL);
}

void motorEnable(void) {
    *(CONFIG_MDRIVE_GPIO_PORT)->set = (0x1u << CONFIG_MDRIVE_GPIO_PIN);
}

void motorDisable(void) {
    *(CONFIG_MDRIVE_GPIO_PORT)->clr = (0x1u << CONFIG_MDRIVE_GPIO_PIN);
}

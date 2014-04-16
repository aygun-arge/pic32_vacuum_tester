
#include "vtimer/vtimer.h"

#include "app_buzzer.h"
#include "driver/gpio.h"

#define CONFIG_BUZZER_GPIO_PORT         &GpioB
#define CONFIG_BUZZER_GPIO_PIN          1

static uint32_t BuzzerDuration;
static esVTimer BuzzerTimeout;

static void buzzerOff(void * arg) {
    *(CONFIG_BUZZER_GPIO_PORT)->clr = (0x1 << CONFIG_BUZZER_GPIO_PIN);
}

void initBuzzerModule(void) {
    esVTimerInit(&BuzzerTimeout);
    *(CONFIG_BUZZER_GPIO_PORT)->tris = ~(0x1 << CONFIG_BUZZER_GPIO_PIN);
    *(CONFIG_BUZZER_GPIO_PORT)->clr  =  (0x1 << CONFIG_BUZZER_GPIO_PIN);
}

void buzzerTone(uint32_t duration) {
    esVTimerCancel(&BuzzerTimeout);
    esVTimerStart(&BuzzerTimeout, ES_VTMR_TIME_TO_TICK_MS(duration), buzzerOff, NULL);
    *(CONFIG_BUZZER_GPIO_PORT)->set = (0x1 << CONFIG_BUZZER_GPIO_PIN);
}


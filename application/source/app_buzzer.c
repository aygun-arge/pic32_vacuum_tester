
#include "vtimer/vtimer.h"

#include "app_buzzer.h"
#include "driver/gpio.h"

#define CONFIG_BUZZER_GPIO_PORT         &GpioB
#define CONFIG_BUZZER_GPIO_PIN          1

static esVTimer BuzzerTimeout;

static void buzzerOff(void * arg) {
    *(CONFIG_BUZZER_GPIO_PORT)->clr = (0x1 << CONFIG_BUZZER_GPIO_PIN);
}

static void buzzerNext(void * arg) {
    uint8_t *           melody;

    melody = (uint8_t *)arg;
    melody++;
    *(CONFIG_BUZZER_GPIO_PORT)->invert = (0x1 << CONFIG_BUZZER_GPIO_PIN);

    if (*melody != 0) {
        esVTimerStart(
            &BuzzerTimeout,
            ES_VTMR_TIME_TO_TICK_MS(*melody),
            buzzerNext,
            (void *)melody);
    } else {
        *(CONFIG_BUZZER_GPIO_PORT)->clr = (0x1 << CONFIG_BUZZER_GPIO_PIN);
    }
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

void buzzerMelody(const uint8_t * melody) {
    esVTimerCancel(&BuzzerTimeout);
    esVTimerStart(&BuzzerTimeout, ES_VTMR_TIME_TO_TICK_MS(*melody), buzzerNext, (void *)melody);
    *(CONFIG_BUZZER_GPIO_PORT)->set = (0x1 << CONFIG_BUZZER_GPIO_PIN);
}


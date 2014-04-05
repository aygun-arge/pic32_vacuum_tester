
#include <xc.h>
#include <sys/attribs.h>

#include "app_buzzer.h"
#include "driver/gpio.h"
#include "driver/clock.h"

#define CONFIG_BUZZER_GPIO_PORT         &GpioB
#define CONFIG_BUZZER_GPIO_PIN          1

#define CONFIG_BUZZER_TIME_QUANTUM      100

#define T_CON_ON                        (0x1u << 15)
#define T_CON_TCKPS(x)                  ((x) << 4)

#define IFS0_T2IF                       (0x1u << 9)
#define IEC0_T2IE                       (0x1u << 9)
#define IPC2_T2_PRIORITY_Msk            (0x7u << 2)
#define IPC2_T2_PRIORITY(x)             ((x) << 2)
#define IPC2_T2_SUBPRIORITY_Msk         (0x3u << 0)
#define IPC2_T2_SUBPRIORITY(x)          ((x) << 0)

static uint32_t BuzzerDuration;

void initBuzzerModule(void) {
    T2CON   = 0u;
    IFS0CLR = IFS0_T2IF;
     *(CONFIG_BUZZER_GPIO_PORT)->tris &= ~(0x1u << CONFIG_BUZZER_GPIO_PIN);
     *(CONFIG_BUZZER_GPIO_PORT)->clr   =  (0x1u << CONFIG_BUZZER_GPIO_PIN);

     IPC2CLR = IPC2_T2_PRIORITY_Msk | IPC2_T2_SUBPRIORITY_Msk;
     IPC2SET = IPC2_T2_PRIORITY(1)  | IPC2_T2_SUBPRIORITY(0);
}

void buzzerTone(uint32_t duration) {
    T2CON = 0u;

    uint32_t prescaler;
    uint32_t reload;

    BuzzerDuration = duration * CONFIG_BUZZER_TIME_QUANTUM / 1000u;
    reload    = clockGetPeripheralClock() / CONFIG_BUZZER_TIME_QUANTUM;
    prescaler = 0u;

    while (reload > UINT16_MAX) {
        reload = clockGetPeripheralClock() / (CONFIG_BUZZER_TIME_QUANTUM * (0x1u << prescaler++));
    }
    T2CON     = T_CON_TCKPS(prescaler);
    TMR2      = 0u;
    PR2       = reload;
    IFS0CLR   = IFS0_T2IF;
    IEC0SET   = IEC0_T2IE;
    T2CONSET  = T_CON_ON;
    *(CONFIG_BUZZER_GPIO_PORT)->set = (0x1 << CONFIG_BUZZER_GPIO_PIN);
}

void __ISR(_TIMER_2_VECTOR) buzzerHandler(void) {
    BuzzerDuration--;

    if (BuzzerDuration == 0) {
        T2CON   = 0u;
        IEC0CLR = IEC0_T2IE;
        *(CONFIG_BUZZER_GPIO_PORT)->clr = (0x1 << CONFIG_BUZZER_GPIO_PIN);
    }
    IFS0CLR = IFS0_T2IF;
}

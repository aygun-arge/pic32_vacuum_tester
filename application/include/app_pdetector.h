
#ifndef APP_PDETECTOR_H
#define APP_PDETECTOR_H

#include "eds/event.h"

#define CONFIG_DEBOUNCE_EVENT_BASE          1800
#define CONFIG_CONSUMER                     Gui
#define CONFIG_PDETECTOR_PORT               &GpioA
#define CONFIG_PDETECTOR_PIN                9


enum event_debounce_id
{
    EVT_PDETECT_PRESS = CONFIG_DEBOUNCE_EVENT_BASE,
    EVT_PDETECT_RELEASE
};

void initPdetectorModule(void);

#endif /* APP_PDETECTOR_H */

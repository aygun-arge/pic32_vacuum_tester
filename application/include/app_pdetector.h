
#ifndef APP_PDETECTOR_H
#define APP_PDETECTOR_H

#include "eds/event.h"

#define CONFIG_DEBOUNCE_EVENT_BASE          1800

enum pdetect_state
{
    PDETECT_PRESENT,
    PDETECT_NOT_PRESENT
};

enum event_debounce_id
{
    EVENT_PDETECT
};

struct event_debounce
{
    esEvent             super;
    enum pdetect_state  state;
};

void initPdetectorModule(void);

#endif /* APP_PDETECTOR_H */

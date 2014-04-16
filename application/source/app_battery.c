
#include "app_battery.h"
#include "driver/gpio.h"
#include "Compiler.h"

#define CONFIG_STAT1_PORT               &GpioC
#define CONFIG_STAT1_PIN                8

#define CONFIG_STAT2_PORT               &GpioC
#define CONFIG_STAT2_PIN                9

#define CONFIG_PG_PORT                  &GpioB
#define CONFIG_PG_PIN                   13

void initBatteryModule(void) {
    *(CONFIG_STAT1_PORT)->tris   |= (0x1u << CONFIG_STAT1_PIN);
    *(CONFIG_STAT1_PORT)->pullup |= (0x1u << CONFIG_STAT1_PIN);
    *(CONFIG_STAT2_PORT)->tris   |= (0x1u << CONFIG_STAT2_PIN);
    *(CONFIG_STAT2_PORT)->pullup |= (0x1u << CONFIG_STAT2_PIN);
    *(CONFIG_PG_PORT)->tris      |= (0x1u << CONFIG_PG_PIN);
    *(CONFIG_PG_PORT)->pullup    |= (0x1u << CONFIG_PG_PIN);
}

bool batteryIsPgHigh(void) {
    
    if ((*(CONFIG_PG_PORT)->port & (0x1u << CONFIG_PG_PIN)) != 0u) {
        
        return (true);
    } else {

        return (false);
    }
}

bool batteryIsStat1High(void) {
    
    if ((*(CONFIG_STAT1_PORT)->port & (0x1u << CONFIG_STAT1_PIN)) != 0u) {
        
        return (true);
    } else {

        return (false);
    }
}

bool batteryIsStat2High(void) {
    
    if ((*(CONFIG_STAT2_PORT)->port & (0x1u << CONFIG_STAT2_PIN)) != 0u) {
        
        return (true);
    } else {

        return (false);
    }
}

enum batteryStatus getBatteryStatus(void) {
    uint32_t            pins;

    pins = 0u;

    if (batteryIsPgHigh()) {
        pins |= 0x1u << 0;
    }

    if (batteryIsStat2High()) {
        pins |= 0x1u << 1;
    }

    if (batteryIsStat1High()) {
        pins |= 0x1u << 2;
    }

    switch (pins) {
        case 0x7: {

            return (BATT_DISCHARING);
        }
        case 0x6: {

            return (BATT_MISSING);
        }
        case 0x3: {

            return (BATT_EMPTY);
        }
        case 0x0: {

            return (BATT_FAULT);
        }
        case 0x4: {

            return (BATT_FULL);
        }
        case 0x2: {

            return (BATT_CHARGING);
        }
        default : {

            return (BATT_OPTIMIZING);
        }
    }
}

uint32_t snprintBatteryStatus(char * buffer) {
    enum batteryStatus status;
    
    status = getBatteryStatus();
    
    switch (status) {
        case BATT_OPTIMIZING: {
            memcpy(buffer, CONFIG_TEXT_OPTIMIZING, sizeof(CONFIG_TEXT_OPTIMIZING));

            return (sizeof(CONFIG_TEXT_OPTIMIZING));
        }
        case BATT_CHARGING: {
            memcpy(buffer, CONFIG_TEXT_CHARGING, sizeof(CONFIG_TEXT_CHARGING));

            return (sizeof(CONFIG_TEXT_CHARGING));
        }
        case BATT_DISCHARING: {
            memcpy(buffer, CONFIG_TEXT_DISCHARGING, sizeof(CONFIG_TEXT_DISCHARGING));

            return (sizeof(CONFIG_TEXT_DISCHARGING));
        }
        case BATT_MISSING: {
            memcpy(buffer, CONFIG_TEXT_MISSING, sizeof(CONFIG_TEXT_MISSING));

            return (sizeof(CONFIG_TEXT_MISSING));
        }
        case BATT_EMPTY: {
            memcpy(buffer, CONFIG_TEXT_EMPTY, sizeof(CONFIG_TEXT_EMPTY));

            return (sizeof(CONFIG_TEXT_EMPTY));
        }
        case BATT_FULL: {
            memcpy(buffer, CONFIG_TEXT_FULL, sizeof(CONFIG_TEXT_FULL));

            return (sizeof(CONFIG_TEXT_FULL));
        }
        default: {
            memcpy(buffer, CONFIG_TEXT_FAULT, sizeof(CONFIG_TEXT_FAULT));

            return (sizeof(CONFIG_TEXT_FAULT));
        }
    }
}

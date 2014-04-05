
#include "app_battery.h"
#include "driver/gpio.h"

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
    
    if ((*(CONFIG_PG_PORT)->port & CONFIG_PG_PIN) != 0u) {
        
        return (true);
    } else {

        return (false);
    }
}

bool batteryIsStat1High(void) {
    
    if ((*(CONFIG_STAT1_PORT)->port & CONFIG_STAT1_PIN) != 0u) {
        
        return (true);
    } else {

        return (false);
    }
}

bool batteryIsStat2High(void) {
    
    if ((*(CONFIG_STAT2_PORT)->port & CONFIG_STAT2_PIN) != 0u) {
        
        return (true);
    } else {

        return (false);
    }
}



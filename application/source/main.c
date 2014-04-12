
#include <xc.h>

#include "main.h"

#include "config/mcu_config.h"

#include "driver/clock.h"
#include "driver/gpio.h"
#include "driver/intr.h"
#include "driver/spi.h"
#include "driver/adc.h"
#include "driver/s25fl.h"

#include "app_gui.h"
#include "app_usb.h"
#include "app_psensor.h"
#include "app_motor.h"
#include "app_battery.h"
#include "app_buzzer.h"
#include "app_config.h"
#include "app_control.h"

#define CONFIG_INIT_RETRY               5

int32_t g_sensor;

int main(void) {
    uint32_t            retry;
    int32_t             status;

    initClockDriver();
    initIntrDriver();
    initGpioDriver();
    initSpiDriver();
    initAdcDriver();

    for (retry = 0; (retry < CONFIG_INIT_RETRY) && ((status = initFlash()) != 0u); retry++);

    if (status != 0) {
        /*--  NOTE: Failed to wake up the flash  -----------------------------*/
    }
    
    initBatteryModule();
    initBuzzerModule();
    initGuiModule();
    initUsbModule();
    initPSensorModule();
    initMotorModule();
    initControlModule();

    /*--  Start up tone  -----------------------------------------------------*/
    buzzerTone(20);

    while (true) {
        isDutVacuumValid();
        appUsb();
        appGui();
        appControl();
    }

    return (0);
}



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

int main(void) {
    initClockDriver();
    initIntrDriver();
    initGpioDriver();
    initSpiDriver();
    initAdcDriver();
    initFlashDriver();
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
        appUsb();
        appGui();
        appControl();
    }

    return (0);
}


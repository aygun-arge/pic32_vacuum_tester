
/*=========================================================  INCLUDE FILES  ==*/

#include <xc.h>

#include "base/base.h"
#include "vtimer/vtimer.h"
#include "mem/mem_class.h"
#include "eds/epa.h"

#include "config/mcu_config.h"

#include "driver/clock.h"
#include "driver/gpio.h"
#include "driver/intr.h"
#include "driver/spi.h"
#include "driver/adc.h"
#include "driver/s25fl.h"
#include "driver/rtc.h"
#include "driver/systick.h"

#include "app_gui.h"
#include "app_usb.h"
#include "app_psensor.h"
#include "app_pdetector.h"
#include "app_motor.h"
#include "app_battery.h"
#include "app_buzzer.h"
#include "app_config.h"
#include "app_storage.h"
#include "app_gpu.h"
#include "app_data_log.h"
#include "app_user.h"

#include "events.h"
#include "epa_touch.h"
#include "epa_gui.h"

#include "main.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CONFIG_EDS_STATIC_SIZE          16384
#define CONFIG_EVENT_HEAP_SIZE          8192

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void nativeFsm(void);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("main", "main loop", "Nenad Radulovic");

static uint8_t          StaticMemBuff[CONFIG_EDS_STATIC_SIZE];



/*======================================================  GLOBAL VARIABLES  ==*/

esMem                   StaticMem      = ES_MEM_INITIALIZER();
esMem                   EventHeapMem   = ES_MEM_INITIALIZER();


/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void nativeFsm(void) {
    appUsb();
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

int main(void) {
    void *              heap;
    
    /*--  Initialize drivers  ------------------------------------------------*/
    initClockDriver();
    initIntrDriver();
    initGpioDriver();
    initSpiDriver();
    initAdcDriver();
    initFlashDriver();
    initRtcDriver();
    initSysTickDriver();

    /*--  Set-up memories  ---------------------------------------------------*/
    esMemInit(
        &esGlobalStaticMemClass,
        &StaticMem,
        StaticMemBuff,
        sizeof(StaticMemBuff),
        0);                                                                     /* Set-up static memory                                     */
    esMemAlloc(&StaticMem, CONFIG_EVENT_HEAP_SIZE, &heap);                      /* Allocate memory for event heap manager                   */
    esMemInit(
        &esGlobalHeapMemClass,
        &EventHeapMem,
        heap,
        CONFIG_EVENT_HEAP_SIZE,
        0);                                                                     /* Set-up heap memory                                       */

    /*--  Initialize modules  ------------------------------------------------*/
    initBatteryModule();
    initBuzzerModule();
    initUsbModule();
    initPSensorModule();
    initMotorModule();
    initStorageModule(&StaticMem);
    initGpuModule();
    initPdetectorModule();

    /*--  Setup NVM storage  -------------------------------------------------*/
    storageRegisterEntry(&DataLogStorage);
    storageRegisterEntry(&TouchStorage);
    storageRegisterEntry(&ConfigStorage);
    storageRegisterEntry(&ArrayDescStorage);

    /*--  Boot the rest of modules  ------------------------------------------*/
    initAppDataLog();
    initAppConfig();

    appUserSetCurrent(APPUSER_OPERATOR_ID);
    
    /*--  Start up tone  -----------------------------------------------------*/
    //buzzerTone(20);
    
    /*--  Initialize virtual timers  -----------------------------------------*/
    esModuleVTimerInit();

    /*--  Register a memory to use for events  -------------------------------*/
    esEventRegisterMem(&EventHeapMem);

    /*--  Initialize EDS kernel  ---------------------------------------------*/
    esEdsInit();

    /*--  Create EPAs  -------------------------------------------------------*/
    ES_ENSURE(esEpaCreate(&GuiEpa,     &GuiSm,     &StaticMem, &Gui));
    ES_ENSURE(esEpaCreate(&TouchEpa,   &TouchSm,   &StaticMem, &Touch));
    
    /*--  Set application idle routine  --------------------------------------*/
    esEdsSetIdle(nativeFsm);

    /*--  Start multitasking  ------------------------------------------------*/
    esEdsStart();

    /*--  In case we abort or terminate clean up everything  -----------------*/
    esEdsTerm();
    esMemTerm(&EventHeapMem);

    return (0);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of main.c
 ******************************************************************************/

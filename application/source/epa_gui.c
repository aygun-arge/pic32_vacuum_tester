
/*=========================================================  INCLUDE FILES  ==*/

#include "epa_gui.h"
#include "eds/epa.h"
#include "vtimer/vtimer.h"

#include "driver/rtc.h"

#include "FT_Platform.h"

#include "app_config.h"
#include "app_time.h"
#include "app_motor.h"
#include "app_psensor.h"
#include "app_battery.h"
#include "app_usb.h"

/*=========================================================  LOCAL MACRO's  ==*/

/**@name        Display coefficients
 */
#define DISP_WIDTH                      320
#define DISP_HEIGHT                     240
#define DISP_H_CYCLE                    408
#define DISP_H_OFFSET                   70
#define DISP_H_SYNC0                    0
#define DISP_H_SYNC1                    10
#define DISP_V_CYCLE                    263
#define DISP_V_OFFSET                   13
#define DISP_V_SYNC0                    0
#define DISP_V_SYNC1                    2
#define DISP_PCLK                       8
#define DISP_SWIZZLE                    2
#define DISP_PCLK_POL                   0

/*--  Normal fonts  ----------------------------------------------------------*/
#define DEF_N1_FONT_SIZE                27

/*--  Big fonts  -------------------------------------------------------------*/
#define DEF_B1_FONT_SIZE                30

#define DEF_WEBSITE                     "www.nitto.com"

#define WELCOME_HW_VERSION              "Hardware version: "
#define WELCOME_SW_VERSION              "Software version: "
#define WELCOME_GREETING                "Vacuum tester"

#define GUI_TABLE(entry)                                                        \
    entry(stateInit,                TOP)                                        \
    entry(stateWakeUpLcd,           TOP)                                        \
    entry(stateMain,                TOP)                                        \
    entry(stateWelcome,             TOP)

/*======================================================  LOCAL DATA TYPES  ==*/

enum guiStateId {
    ES_STATE_ID_INIT(GUI_TABLE)
};

enum localEvents {
    SLEEP_TIMEOUT_ = ES_EVENT_LOCAL_ID
};

struct wspace {
    struct esVTimer     sleep;
    uint32_t            retry;
};


enum poratorDetectorState {
    PORATOR_IS_DETECTED,
    PORATOR_IS_NOT_DETECTED,
};

enum usbState {
    USB_IS_PLUGGED_AND_VALID,
    USB_IS_PLUGGED_AND_INVALID,
    USB_IS_NOT_PLUGGED
};

enum powerState {
    PWR_BAT_FULL,
    PWR_BAT_CHARGING,
    PWR_BAT_DISCHARING
};

struct screenMainStatus {
    bool                isDutDetected;
    char                usb[20];
    char                battery[20];
    char                date[20];
    char                time[10];
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void gpuInitEarly(void);
static void gpuInitLate(void);

static void sleepTimeout(void * arg);

static void screenWelcome(void);
static void screenMain(struct screenMainStatus * status);
static void screenExport(void);
static void screenSettings(void);

static esAction stateInit       (struct wspace *, const esEvent *);
static esAction stateWakeUpLcd  (struct wspace *, const esEvent *);
static esAction stateWelcome    (struct wspace *, const esEvent *);
static esAction stateMain       (struct wspace *, const esEvent *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("GUI", CONFIG_EPA_GUI_NAME, "Nenad Radulovic");

static const esSmTable      GuiTable[] = ES_STATE_TABLE_INIT(GUI_TABLE);

static Ft_Gpu_Hal_Context_t Gpu;

static struct spiHandle     GpuSpi;

/*======================================================  GLOBAL VARIABLES  ==*/

const struct esEpaDefine GuiEpa = ES_EPA_DEFINE(
    CONFIG_EPA_GUI_NAME,
    CONFIG_EPA_GUI_PRIORITY,
    CONFIG_EPA_GUI_QUEUE_SIZE);
const struct esSmDefine  GuiSm = ES_SM_DEFINE(
    GuiTable,
    sizeof(struct wspace),
    stateInit);
struct esEpa *           Gui;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/*--  SUPPORT  ---------------------------------------------------------------*/

static void gpuInitEarly(void) {
    /* Values specific to QVGA LCD display */
    Ft_Gpu_HalInit_t halinit;                       /* Not used in this port */

    Gpu.hal_handle = &GpuSpi;
    Ft_Gpu_Hal_Init(&halinit);
    Ft_Gpu_Hal_Open(&Gpu);

    /* Do a power cycle for safer side */
    Ft_Gpu_Hal_Powercycle(&Gpu, FT_TRUE);
    //Ft_Gpu_Hal_Rd16(&Gpu, RAM_G);

    /* Access address 0 to wake up the FT800 */
    Ft_Gpu_HostCommand(&Gpu, FT_GPU_ACTIVE_M);
    Ft_Gpu_Hal_Sleep(40);

    /* Set the clk to external clock */
    Ft_Gpu_HostCommand(&Gpu, FT_GPU_EXTERNAL_OSC);
    Ft_Gpu_Hal_Sleep(20);

    /* Switch PLL output to 48MHz */
    Ft_Gpu_HostCommand(&Gpu, FT_GPU_PLL_48M);
    Ft_Gpu_Hal_Sleep(20);

    /* Do a core reset for safer side */
    Ft_Gpu_HostCommand(&Gpu, FT_GPU_CORE_RESET);
}

static void gpuInitLate(void) {
    Ft_Gpu_Hal_Wr16(&Gpu, REG_HCYCLE,   DISP_H_CYCLE);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_HOFFSET,  DISP_H_OFFSET);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_HSYNC0,   DISP_H_SYNC0);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_HSYNC1,   DISP_H_SYNC1);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_VCYCLE,   DISP_V_CYCLE);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_VOFFSET,  DISP_V_OFFSET);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_VSYNC0,   DISP_V_SYNC0);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_VSYNC1,   DISP_V_SYNC1);
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_SWIZZLE,  DISP_SWIZZLE);
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_PCLK_POL, DISP_PCLK_POL);

    Ft_Gpu_Hal_Wr16(&Gpu, REG_HSIZE,    DISP_WIDTH);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_VSIZE,    DISP_HEIGHT);

    /* Touch configuration - configure the resistance value to 1200 - this value is specific to customer requirement and derived by experiment */
    Ft_Gpu_Hal_Wr16(&Gpu, REG_TOUCH_RZTHRESH,1200);
    Ft_Gpu_Hal_Wr32(&Gpu, RAM_DL + 0,   CLEAR_COLOR_RGB(0, 0, 0));//set the background to black
    Ft_Gpu_Hal_Wr32(&Gpu, RAM_DL + 4,   CLEAR(1,1,1));
    Ft_Gpu_Hal_Wr32(&Gpu, RAM_DL + 8,   DISPLAY());
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_DLSWAP,   DLSWAP_FRAME);

    Ft_Gpu_Hal_Wr8(&Gpu,  REG_GPIO_DIR, 0x83 | Ft_Gpu_Hal_Rd8(&Gpu, REG_GPIO_DIR));
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_GPIO,     0x83 | Ft_Gpu_Hal_Rd8(&Gpu, REG_GPIO));

    Ft_Gpu_Hal_Wr8(&Gpu,  REG_PCLK,     DISP_PCLK);                             /* After this display is visible on the LCD                 */
}

static void sleepTimeout(void * arg) {
    struct esEvent *    timeout;
    esError             error;

    (void)arg;
    ES_ENSURE(error = esEventCreate(
        sizeof(struct esEvent),
        SLEEP_TIMEOUT_,
        &timeout));

    if (error == ES_ERROR_NONE) {
        esEpaSendAheadEvent(Gui, timeout);
    }
}

static void screenWelcome(void) {
    char        buffer[100];
    uint32_t    length;
    
    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    Ft_Gpu_Hal_WrCmd32(&Gpu,CLEAR_COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu,COLOR_RGB(0, 0, 0));
    Ft_Gpu_Hal_WrCmd32(&Gpu,CLEAR(1,1,1));
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 80, DEF_B1_FONT_SIZE, OPT_CENTER, WELCOME_GREETING);
    memcpy(buffer, WELCOME_HW_VERSION, sizeof(WELCOME_HW_VERSION));
    length = sizeof(WELCOME_HW_VERSION) - 1u;
    memcpy(&buffer[length], CONFIG_HARDWARE_VERSION, sizeof(CONFIG_HARDWARE_VERSION));
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 120, DEF_N1_FONT_SIZE, OPT_CENTER, buffer);
    memcpy(buffer, WELCOME_SW_VERSION, sizeof(WELCOME_SW_VERSION));
    length = sizeof(WELCOME_SW_VERSION) - 1u;
    memcpy(&buffer[length], CONFIG_SOFTWARE_VERSION, sizeof(CONFIG_SOFTWARE_VERSION));
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 140, DEF_N1_FONT_SIZE, OPT_CENTER, buffer);
    length = snprintRtcDate(buffer);
    buffer[length] = '\0';
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 160, DEF_N1_FONT_SIZE, OPT_CENTER, buffer);
    length = snprintRtcTime(buffer);
    buffer[length] = '\0';
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 180, DEF_N1_FONT_SIZE, OPT_CENTER, buffer);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 220, DEF_N1_FONT_SIZE, OPT_CENTER, DEF_WEBSITE);
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}


static void screenMain(struct screenMainStatus * status) {
    char *              text;

    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    Ft_Gpu_Hal_WrCmd32(&Gpu,CLEAR_COLOR_RGB(224, 224, 224));
    Ft_Gpu_Hal_WrCmd32(&Gpu,COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu,CLEAR(1,1,1));
    Ft_Gpu_CoCmd_Button(&Gpu, 80,  80, 160, 80, DEF_B1_FONT_SIZE, 0, "TEST");
    Ft_Gpu_CoCmd_Button(&Gpu, 10,  10, 80,  40, DEF_N1_FONT_SIZE, 0, "Settings");
    Ft_Gpu_CoCmd_Button(&Gpu, 230, 10, 80,  40, DEF_N1_FONT_SIZE, 0, "Export");

    if (status->isDutDetected) {
        Ft_Gpu_Hal_WrCmd32(&Gpu,COLOR_RGB(0, 0, 0));
        text = "Porator is detected";
    } else {
        Ft_Gpu_Hal_WrCmd32(&Gpu,COLOR_RGB(255, 0, 0));
        text = "Put the porator on the test pad.";
    }
    Ft_Gpu_CoCmd_Text(&Gpu, 160, 185, DEF_N1_FONT_SIZE, OPT_CENTER, text);
    Ft_Gpu_Hal_WrCmd32(&Gpu,COLOR_RGB(0, 0, 0));
    Ft_Gpu_CoCmd_Text(&Gpu, 160, 23, DEF_N1_FONT_SIZE, OPT_CENTER, status->date);
    Ft_Gpu_CoCmd_Text(&Gpu, 160, 42, DEF_N1_FONT_SIZE, OPT_CENTER, status->time);
    Ft_Gpu_CoCmd_Text(&Gpu, 10,  225, DEF_N1_FONT_SIZE, OPT_CENTERY, "USB:");
    Ft_Gpu_CoCmd_Text(&Gpu, 50,  225, DEF_N1_FONT_SIZE, OPT_CENTERY, status->usb);
#if (PATCH_LEVEL_POWER_STATUS_1 == 0)
    Ft_Gpu_CoCmd_Text(&Gpu, 180, 225, DEF_N1_FONT_SIZE, OPT_CENTERY, "BAT:");
    Ft_Gpu_CoCmd_Text(&Gpu, 220, 225, DEF_N1_FONT_SIZE, OPT_CENTERY, status->battery);
#elif (PATCH_LEVEL_POWER_STATUS_1 == 1)
    Ft_Gpu_CoCmd_Text(&Gpu, 180, 225, DEF_N1_FONT_SIZE, OPT_CENTERY, "PWR:");
    Ft_Gpu_CoCmd_Text(&Gpu, 220, 225, DEF_N1_FONT_SIZE, OPT_CENTERY, status->battery);
#endif
    Ft_Gpu_Hal_WrCmd32(&Gpu, BEGIN(3));
    Ft_Gpu_Hal_WrCmd32(&Gpu, VERTEX2II(10,  210, 0, 64));
    Ft_Gpu_Hal_WrCmd32(&Gpu, VERTEX2II(310, 210, 0, 64));
    Ft_Gpu_Hal_WrCmd32(&Gpu, END());
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

/*--  End of SUPPORT  --------------------------------------------------------*/

static esAction stateInit(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_INIT: {
            wspace->retry = 10u;
            esVTimerInit(&wspace->sleep);
            gpuInitEarly();

            return (ES_STATE_TRANSITION(stateWakeUpLcd));
        }
        default: {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateWakeUpLcd(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_INIT : {
            ft_uint8_t chipid;

            chipid = Ft_Gpu_Hal_Rd8(&Gpu, REG_ID);

            if (chipid == 0x7C) {
                gpuInitLate();

                return (ES_STATE_TRANSITION(stateWelcome));
            } else if (wspace->retry != 0u) {
                wspace->retry--;
                esVTimerStart(
                    &wspace->sleep,
                    ES_VTMR_TIME_TO_TICK_MS(100),
                    sleepTimeout,
                    NULL);

                return (ES_STATE_HANDLED());
            } else {

                return (ES_STATE_TRANSITION(stateInit));
            }
        }
        case SLEEP_TIMEOUT_: {

            return (ES_STATE_TRANSITION(stateWakeUpLcd));
        }
        default: {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateWelcome(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY: {
            esVTimerStart(
                &wspace->sleep,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TIME_WELCOME),
                sleepTimeout,
                NULL);
            screenWelcome();

            return (ES_STATE_HANDLED());
        }
        case SLEEP_TIMEOUT_: {

            return (ES_STATE_TRANSITION(stateMain));
        }
        default: {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateMain(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY: {
            struct screenMainStatus status;

            snprintRtcTime(status.time);
            snprintRtcDate(status.date);
            snprintBatteryStatus(status.battery);
            snprintUsbStatus(status.usb);
            status.isDutDetected = isDutDetected();
            screenMain(&status);
            esVTimerStart(
                &wspace->sleep,
                ES_VTMR_TIME_TO_TICK_MS(1000),
                sleepTimeout,
                NULL);

            return (ES_STATE_HANDLED());
        }
        case SLEEP_TIMEOUT_: {

            return (ES_STATE_TRANSITION(stateMain));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa_gui.c
 ******************************************************************************/

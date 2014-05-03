
/*=========================================================  INCLUDE FILES  ==*/

#include "epa_gui.h"
#include "eds/epa.h"
#include "vtimer/vtimer.h"

#include "driver/rtc.h"

#include "FT_Platform.h"
#include "HardwareProfile.h"
#include "software_profile.h"

#include "app_config.h"
#include "app_time.h"
#include "app_motor.h"
#include "app_psensor.h"
#include "app_battery.h"
#include "app_usb.h"
#include "main.h"

#include "logo.h"
#include "app_buzzer.h"
#include "gobject.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CONFIG_GOBJECT_HEAP_SIZE        4096

#define CONFIG_PRE_TEST_MS              500
#define CONFIG_TEST_CANCEL_MS           5000
#define CONFIG_TEST_FAIL_MS             5000
#define CONFIG_TEST_OVERVIEW_MS         5000
#define CONFIG_TEST_REFRESH_MS          20
#define CONFIG_MAIN_REFRESH_MS          20

#define DEF_VACUUM_UNIT                 "mmHg"

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
#define DEF_N2_FONT_SIZE                29

/*--  Big fonts  -------------------------------------------------------------*/
#define DEF_B1_FONT_SIZE                30

#define DEF_WEBSITE                     "www.nitto.com"

#define WELCOME_HW_VERSION              "Hardware version: "
#define WELCOME_SW_VERSION              "Software version: "
#define WELCOME_GREETING                "Vacuum tester"

#define POS_ROW_1                       70
#define POS_ROW_1_5                     100
#define POS_ROW_2                       130
#define POS_ROW_2_5                     160
#define POS_COLUMN_4                    40
#define POS_COLUMN_18                   180
#define POS_COLUMN_25                   250
#define POS_COLUMN_HALF                 (DISP_WIDTH / 2)

#define POS_TITLE_V                     30
#define POS_TITLE_H                     (DISP_WIDTH / 2)

#define SCREEN_TEST_EN_FIRST_INFO       (0x1u << 0)
#define SCREEN_TEST_EN_SECOND_INFO      (0x1u << 1)
#define SCREEN_TEST_EN_FIRST_PROGRESS   (0x1u << 2)
#define SCREEN_TEST_EN_SECOND_PROGRESS  (0x1u << 3)
#define SCREEN_TEST_EN_FIRST_STATUS     (0x1u << 4)
#define SCREEN_TEST_EN_SECOND_STATUS    (0x1u << 5)
#define SCREEN_TEST_EN_BUTTON           (0x1u << 6)

#define GUI_TABLE(entry)                                                        \
    entry(stateInit,                TOP)                                        \
    entry(stateWakeUpLcd,           TOP)                                        \
    entry(stateMain,                TOP)                                        \
    entry(statePreTest,             TOP)                                        \
    entry(stateTestFirstTh,         TOP)                                        \
    entry(stateTestSecondTh,        TOP)                                        \
    entry(stateTestResults,         TOP)                                        \
    entry(stateTestResultsNotify,   TOP)                                        \
    entry(stateTestResultsSaving,   TOP)                                        \
    entry(stateSettings,            TOP)                                        \
    entry(stateSettingsAbout,       TOP)                                        \
    entry(stateSettingsAdmin,       TOP)                                        \
    entry(stateSettingsLcdCalib,    TOP)                                        \
    entry(stateExport,              TOP)                                        \
    entry(stateExportInsert,        TOP)                                        \
    entry(stateExportMount,         TOP)                                        \
    entry(stateExportChoose,        TOP)                                        \
    entry(stateExportSaving,        TOP)                                        \
    entry(stateWelcome,             TOP)

/*======================================================  LOCAL DATA TYPES  ==*/

enum guiStateId {
    ES_STATE_ID_INIT(GUI_TABLE)
};

enum localEvents {
    WAKEUP_TIMEOUT_ = ES_EVENT_LOCAL_ID,
    WELCOME_WAIT_,
    MAIN_REFRESH_,
    PRE_TEST_WAIT_,
    FIRST_TH_TIMEOUT_,
    FIRST_TH_REFRESH_,
    SECOND_TH_TIMEOUT_,
    SECOND_TH_REFRESH_,
    TEST_RESULTS_NOTIFY_REFRESH_,
    TEST_RESULTS_NOTIFY_TIMEOUT_,
    SETTINGS_REFRESH_,
    SETTINGS_ABOUT_REFRESH_,
    EXPORT_INSERT_REFRESH_,
    EXPORT_CHOOSE_REFRESH_
};

enum testTitle {
    SCREEN_TEST_PREPARING,
    SCREEN_TEST_1_IN_PROGRESS,
    SCREEN_TEST_2_IN_PROGRESS,
    SCREEN_TEST_FAILED,
    SCREEN_TEST_CANCELLED,
    SCREEN_TEST_SUCCESS
};

enum testResult {
    TEST_SUCCESS,
    TEST_FAILURE,
    TEST_CANCELLED
};

enum buttonBackPos {
    DOWN_LEFT,
    DOWN_MIDDLE,
};

struct testStatus {
    bool                isValid;
    bool                isCancelled;
    bool                isExecuted;
    uint32_t            rawMaxValue;
    uint32_t            time;
};


struct screenMain {
    bool                isDutDetected;
    char                battery[20];
    char                date[20];
    char                time[10];
};

struct screenTest {
    enum testTitle      title;
    bool                isBackgroundEnabled;
    uint32_t            mask;
    int32_t             firstThVal;
    uint32_t            firstThProgress;
    char                firstThStatus[40];
    int32_t             secondThVal;
    uint32_t            secondThProgress;
    char                secondThStatus[40];
    char                button[16];
};

struct screenExportChoose {
    uint32_t            begin[3];
    uint32_t            end[3];
    uint32_t            focus;
};

struct wspace {
    struct esVTimer     timeout;
    struct esVTimer     refresh;
    uint32_t            retry;
    uint32_t            rawIdleVacuum;
    struct testStatus   firstTh;
    struct testStatus   secondTh;
    union screen {
        struct screenMain   main;
        struct screenTest   test;
        struct screenExportChoose exportChoose;
    }                   screen;
    const uint8_t *     notification;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void gpuInitEarly(void);
static void gpuInitLate(void);

static void timeout(void * arg);

static void screenWelcome(void);
static void screenMain(struct screenMain * status);
static void screenExportInsert(void);
static void screenSettings(void);

static esAction stateInit               (struct wspace *, const esEvent *);
static esAction stateWakeUpLcd          (struct wspace *, const esEvent *);
static esAction stateWelcome            (struct wspace *, const esEvent *);
static esAction stateMain               (struct wspace *, const esEvent *);
static esAction statePreTest            (struct wspace *, const esEvent *);
static esAction stateTestFirstTh        (struct wspace *, const esEvent *);
static esAction stateTestSecondTh       (struct wspace *, const esEvent *);
static esAction stateTestResults        (struct wspace *, const esEvent *);
static esAction stateTestResultsNotify  (struct wspace *, const esEvent *);
static esAction stateTestResultsSaving  (struct wspace *, const esEvent *);
static esAction stateSettings           (struct wspace *, const esEvent *);
static esAction stateSettingsAbout      (struct wspace *, const esEvent *);
static esAction stateSettingsAdmin      (struct wspace *, const esEvent *);
static esAction stateSettingsLcdCalib   (struct wspace *, const esEvent *);
static esAction stateExport             (struct wspace *, const esEvent *);
static esAction stateExportInsert       (struct wspace *, const esEvent *);
static esAction stateExportMount        (struct wspace *, const esEvent *);
static esAction stateExportChoose       (struct wspace *, const esEvent *);
static esAction stateExportSaving       (struct wspace *, const esEvent *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("GUI", CONFIG_EPA_GUI_NAME, "Nenad Radulovic");

static const esSmTable      GuiTable[] = ES_STATE_TABLE_INIT(GUI_TABLE);

static const uint8_t        StartNotification[] = {20, 100, 20, 0};
static const uint8_t        FailNotification[] = {150, 150, 175, 175, 200, 0};
static const uint8_t        ConfusedNotification[] = {20, 100, 20, 100, 40, 100, 40, 100, 60, 0};
static const uint8_t        SuccessNotification[] = {40, 100, 40, 100, 40, 0};

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

    /* Touch configuration - configure the resistance value to 1200 - this value 
     * is specific to customer requirement and derived by experiment.
     */
    Ft_Gpu_Hal_Wr16(&Gpu, REG_TOUCH_RZTHRESH, 1200);
    Ft_Gpu_Hal_Wr32(&Gpu, RAM_DL + 0,   CLEAR_COLOR_RGB(0, 0, 0));              /* Set the background to black                              */
    Ft_Gpu_Hal_Wr32(&Gpu, RAM_DL + 4,   CLEAR(1,1,1));
    Ft_Gpu_Hal_Wr32(&Gpu, RAM_DL + 8,   DISPLAY());
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_DLSWAP,   DLSWAP_FRAME);

    Ft_Gpu_Hal_Wr8(&Gpu,  REG_GPIO_DIR, 0x83 | Ft_Gpu_Hal_Rd8(&Gpu, REG_GPIO_DIR));
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_GPIO,     0x83 | Ft_Gpu_Hal_Rd8(&Gpu, REG_GPIO));
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_PWM_DUTY, 0);                                     /* Completely turn off LCD, it will be lit at later stage   */
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_PCLK,     DISP_PCLK);                             /* After this display is visible on the LCD                 */
}

static void timeout(void * arg) {
    struct esEvent *    timeout;
    esError             error;

    ES_ENSURE(error = esEventCreate(
        sizeof(struct esEvent),
        (uint32_t)arg,
        &timeout));

    if (error == ES_ERROR_NONE) {
        esEpaSendAheadEvent(Gui, timeout);
    }
}

static uint8_t getKey(void) {
    static uint16_t oldState;
    uint16_t        newState;
    uint16_t        retval;

    newState = Ft_Gpu_Hal_Rd8(&Gpu, REG_TOUCH_TAG);

    if ((oldState != 0) && (newState == 0)) {
        retval = oldState;
    } else {
        retval = 0;
    }
    oldState = newState;

    return (retval);
}

static void fadeIn(void) {
    uint32_t    pwm;

    for (pwm = 0; pwm <=128 ; pwm++) {
        Ft_Gpu_Hal_Wr8(&Gpu, REG_PWM_DUTY, pwm);
        Ft_Gpu_Hal_Sleep(4);//sleep for 2 ms
    }
}

static void fadeOut(void) {
    uint32_t    pwm;

    for (pwm = 128; pwm >= 0 ; pwm--) {
        Ft_Gpu_Hal_Wr8(&Gpu, REG_PWM_DUTY, pwm);
        Ft_Gpu_Hal_Sleep(2);//sleep for 2 ms
    }
}

static void fadeOff(void) {
    Ft_Gpu_Hal_Wr8(&Gpu, REG_PWM_DUTY, 128);
}

static void constructBackground(void) {
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_COLOR_RGB(224, 224, 224));
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR(1, 1, 1));
    Ft_Gpu_CoCmd_Gradient(&Gpu, 0,0, 0x707070, 0, DISP_HEIGHT, 0xe0e0e0);
}

static void constructTitle(const char * title) {
    Ft_Gpu_CoCmd_Text(&Gpu, POS_TITLE_H,  POS_TITLE_V, DEF_B1_FONT_SIZE, OPT_CENTER, title);
}

static void constructButtonBack(enum buttonBackPos position) {
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('B'));
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_CoCmd_FgColor(&Gpu, COLOR_RGB(12, 128, 48));

    switch (position) {
        case DOWN_LEFT: {
            Ft_Gpu_CoCmd_Button(&Gpu, 20, 180, 130, 40, DEF_N1_FONT_SIZE, 0, "Back");
            break;
        }
        default : {
            Ft_Gpu_CoCmd_Button(&Gpu, 98, 180, 130, 40, DEF_N1_FONT_SIZE, 0, "Back");
        }
    }
    Ft_Gpu_CoCmd_ColdStart(&Gpu);
}

static void screenWelcome(void) {
    char        buffer[100];

    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR(1,1,1));
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 80, DEF_B1_FONT_SIZE, OPT_CENTER, WELCOME_GREETING);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 120, DEF_N1_FONT_SIZE, OPT_CENTER, WELCOME_HW_VERSION CONFIG_HARDWARE_VERSION);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 140, DEF_N1_FONT_SIZE, OPT_CENTER, WELCOME_SW_VERSION CONFIG_SOFTWARE_VERSION);
    strcpy(buffer, BUILD_DATE);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 160, DEF_N1_FONT_SIZE, OPT_CENTER, buffer);
    strcpy(buffer, BUILD_TIME);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 180, DEF_N1_FONT_SIZE, OPT_CENTER, buffer);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 220, DEF_N1_FONT_SIZE, OPT_CENTER, DEF_WEBSITE);
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

static void screenSettingsAbout(void) {
    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    constructBackground();
    constructTitle("About");
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 120, DEF_N1_FONT_SIZE, OPT_CENTER, WELCOME_HW_VERSION CONFIG_HARDWARE_VERSION);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 140, DEF_N1_FONT_SIZE, OPT_CENTER, WELCOME_SW_VERSION CONFIG_SOFTWARE_VERSION);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 160, DEF_N1_FONT_SIZE, OPT_CENTER, DEF_WEBSITE);
    constructButtonBack(DOWN_MIDDLE);
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

static void screenCalibrate(void) {
    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    constructBackground();
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 80, DEF_B1_FONT_SIZE, OPT_CENTER, "Touch Calibration");
    Ft_Gpu_CoCmd_Text(&Gpu,DISP_WIDTH / 2 ,DISP_HEIGHT/2,26,OPT_CENTERX|OPT_CENTERY,"Please tap on the dot");
    Ft_Gpu_CoCmd_Calibrate(&Gpu, 0);
    Ft_Gpu_Hal_WrCmd32(&Gpu,DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

static void screenMain(struct screenMain * status) {
    char *              text;

    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    constructBackground();
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_TAG(0));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG_MASK(1));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('S'));
    Ft_Gpu_CoCmd_Button(&Gpu, 20,  20, 130,  40, DEF_N1_FONT_SIZE, 0, "Settings");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('E'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 20, 130,  40, DEF_N1_FONT_SIZE, 0, "Export");

    if (status->isDutDetected) {
        Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
        Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('T'));
        Ft_Gpu_CoCmd_Button(&Gpu, 80,  80, 160, 80, DEF_B1_FONT_SIZE, 0, "TEST");
        Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));
        text = "Porator is detected";
    } else {
        Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(92, 92, 92));
        Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('t'));
        Ft_Gpu_CoCmd_FgColor(&Gpu, COLOR_RGB(112, 112, 112));
        Ft_Gpu_CoCmd_Button(&Gpu, 80,  80, 160, 80, DEF_B1_FONT_SIZE, 0, "TEST");
        Ft_Gpu_CoCmd_ColdStart(&Gpu);
        Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 0, 0));
        text = "Put the porator on the test pad.";
    }
    Ft_Gpu_CoCmd_Text(&Gpu, 160, 185, DEF_N1_FONT_SIZE, OPT_CENTER, text);
    Ft_Gpu_Hal_WrCmd32(&Gpu,          COLOR_RGB(0, 0, 0));
    Ft_Gpu_CoCmd_Text(&Gpu, 140, 225, DEF_N1_FONT_SIZE, OPT_CENTERY, status->date);
    Ft_Gpu_CoCmd_Text(&Gpu, 260, 225, DEF_N1_FONT_SIZE, OPT_CENTERY, status->time);
    Ft_Gpu_CoCmd_Text(&Gpu, 10,  225, DEF_N1_FONT_SIZE, OPT_CENTERY, "BAT:");
    Ft_Gpu_CoCmd_Text(&Gpu, 50, 225,  DEF_N1_FONT_SIZE, OPT_CENTERY, status->battery);
    Ft_Gpu_Hal_WrCmd32(&Gpu, BEGIN(3));
    Ft_Gpu_Hal_WrCmd32(&Gpu, VERTEX2II(10,  210, 0, 64));
    Ft_Gpu_Hal_WrCmd32(&Gpu, VERTEX2II(310, 210, 0, 64));
    Ft_Gpu_Hal_WrCmd32(&Gpu, END());
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

static enum testResult evaluateTest(struct wspace * wspace) {
     if (wspace->firstTh.isValid && wspace->secondTh.isValid) {

         return (TEST_SUCCESS);
    } else if (wspace->firstTh.isCancelled || wspace->secondTh.isCancelled) {

        return (TEST_CANCELLED);
    } else {

        return (TEST_FAILURE);
    }
}

static void screenTestToggleBackground(struct wspace * wspace) {
    if (wspace->screen.test.isBackgroundEnabled == true) {
        wspace->screen.test.isBackgroundEnabled = false;
    } else {
        wspace->screen.test.isBackgroundEnabled = true;
    }
}

static void screenTestPrepare(struct wspace * wspace) {

    switch (evaluateTest(wspace)) {
        case TEST_SUCCESS : {
            wspace->screen.test.title = SCREEN_TEST_SUCCESS;

            break;
        }
        case TEST_CANCELLED : {
            wspace->screen.test.title = SCREEN_TEST_CANCELLED;

            break;
        }
        default : {
            wspace->screen.test.title = SCREEN_TEST_FAILED;
        }
    }

    if (wspace->firstTh.isValid) {
        strcpy(wspace->screen.test.firstThStatus, "PASSED");
    } else if (wspace->firstTh.isCancelled) {
        strcpy(wspace->screen.test.firstThStatus, "CANCELLED");
    } else if (wspace->firstTh.isExecuted == false) {
        strcpy(wspace->screen.test.firstThStatus, "SKIPPED");
    } else {
        strcpy(wspace->screen.test.firstThStatus, "FAILED");
    }

    if (wspace->secondTh.isValid) {
        strcpy(wspace->screen.test.secondThStatus, "PASSED");
    } else if (wspace->secondTh.isCancelled) {
        strcpy(wspace->screen.test.secondThStatus, "CANCELLED");
    } else if (wspace->secondTh.isExecuted == false) {
        strcpy(wspace->screen.test.secondThStatus, "SKIPPED");
    } else {
        strcpy(wspace->screen.test.secondThStatus, "FAILED");
    }
    wspace->screen.test.mask =
        SCREEN_TEST_EN_FIRST_INFO  | SCREEN_TEST_EN_FIRST_STATUS  |
        SCREEN_TEST_EN_SECOND_INFO | SCREEN_TEST_EN_SECOND_STATUS |
        SCREEN_TEST_EN_BUTTON;
    wspace->screen.test.firstThVal  = wspace->firstTh.rawMaxValue;
    wspace->screen.test.secondThVal = wspace->secondTh.rawMaxValue;
    wspace->screen.test.isBackgroundEnabled = true;
    strcpy(wspace->screen.test.button, "Continue");
}

static void screenTestDump(struct screenTest * status) {
    Ft_Gpu_CoCmd_Dlstart(&Gpu);

    switch (status->title) {
        case SCREEN_TEST_PREPARING     :
        case SCREEN_TEST_1_IN_PROGRESS :
        case SCREEN_TEST_2_IN_PROGRESS : {
            constructBackground();
            constructTitle("Test in progress");
            break;
        }
        case SCREEN_TEST_FAILED : {
            if (status->isBackgroundEnabled) {
                Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_COLOR_RGB(224, 16, 16));
                Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));
                Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR(1,1,1));
            } else {
                constructBackground();
            }
            constructTitle("Test failed");
            break;
        }
        case SCREEN_TEST_CANCELLED: {
            if (status->isBackgroundEnabled) {
                Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_COLOR_RGB(224, 224, 16));
                Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));
                Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR(1,1,1));
            } else {
                constructBackground();
            }
            constructTitle("Test cancelled");
            break;
        }
        case SCREEN_TEST_SUCCESS: {
            if (status->isBackgroundEnabled) {
                Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_COLOR_RGB(16, 224, 16));
                Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));
                Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR(1, 1, 1));
            } else {
                constructBackground();
            }
            constructTitle("Test success");
            break;
        }
    }

    if (status->mask & SCREEN_TEST_EN_FIRST_INFO) {
        Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_4,   POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY, "First threshold");
        Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_18,  POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY, "[" DEF_VACUUM_UNIT "]:");
        Ft_Gpu_CoCmd_Number(&Gpu, POS_COLUMN_25,  POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY, status->firstThVal);
    }
    
    if (status->mask & SCREEN_TEST_EN_FIRST_STATUS) {
        Ft_Gpu_CoCmd_Text(&Gpu, POS_COLUMN_HALF,  POS_ROW_1_5, DEF_N1_FONT_SIZE, OPT_CENTER, status->firstThStatus);
    } else if (status->mask & SCREEN_TEST_EN_FIRST_PROGRESS) {
        Ft_Gpu_CoCmd_Progress(&Gpu, POS_COLUMN_4, POS_ROW_1_5 - 5, DISP_WIDTH - (POS_COLUMN_4 * 2), 10, 0, status->firstThVal, status->firstThProgress);
    }
    
    if (status->mask & SCREEN_TEST_EN_SECOND_INFO) {
        Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_4,   POS_ROW_2, DEF_N1_FONT_SIZE, OPT_CENTERY, "Second threshold");
        Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_18,  POS_ROW_2, DEF_N1_FONT_SIZE, OPT_CENTERY, "[" DEF_VACUUM_UNIT "]:");
        Ft_Gpu_CoCmd_Number(&Gpu, POS_COLUMN_25,  POS_ROW_2, DEF_N1_FONT_SIZE, OPT_CENTERY, status->secondThVal);
    }

    if (status->mask & SCREEN_TEST_EN_SECOND_STATUS) {
        Ft_Gpu_CoCmd_Text(&Gpu, POS_COLUMN_HALF,  POS_ROW_2_5, DEF_N1_FONT_SIZE, OPT_CENTER, status->secondThStatus);
    } else if (status->mask & SCREEN_TEST_EN_SECOND_PROGRESS) {
        Ft_Gpu_CoCmd_Progress(&Gpu, POS_COLUMN_4, POS_ROW_2_5 - 5, DISP_WIDTH - (POS_COLUMN_4 * 2), 10, 0, status->secondThVal, status->secondThProgress);
    }

    if (status->mask & SCREEN_TEST_EN_BUTTON) {
        Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
        Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_TAG(0));
        Ft_Gpu_Hal_WrCmd32(&Gpu, TAG_MASK(1));
        Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('B'));
        Ft_Gpu_CoCmd_Button(&Gpu, 95, 180, 130, 40, DEF_N1_FONT_SIZE, 0, status->button);
    }
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

static void screenTestSaving(void) {
    static uint32_t record = 12;
    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    constructBackground();
    constructTitle("Saving...");
    Ft_Gpu_CoCmd_Text(&Gpu, 160,  200, DEF_N1_FONT_SIZE, OPT_CENTER, "Record number:");
    Ft_Gpu_CoCmd_Number(&Gpu, 240,  200, DEF_N1_FONT_SIZE, OPT_CENTER, record++);
    Ft_Gpu_CoCmd_Spinner(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, 0, 0);
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

static void screenExportInsert(void) {
    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    constructBackground();
    constructTitle("Export");
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, DEF_N1_FONT_SIZE, OPT_CENTER, "Please insert USB flash drive");
    constructButtonBack(DOWN_MIDDLE);
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

static void screenExportMount(void) {
    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    constructBackground();
    constructTitle("Export");
    Ft_Gpu_CoCmd_Spinner(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, 0, 0);
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

static void screenExportSaving(void) {
    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    constructBackground();
    Ft_Gpu_CoCmd_Text(&Gpu, POS_TITLE_H,  POS_TITLE_V, DEF_B1_FONT_SIZE, OPT_CENTER, "Saving data...");
    Ft_Gpu_CoCmd_Spinner(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, 0, 0);
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

#define EXPORT_MONTH                    0
#define EXPORT_DAY                      1
#define EXPORT_YEAR                     2

static void screenExportChoose(struct screenExportChoose * status) {
    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    constructBackground();
    Ft_Gpu_CoCmd_Text(&Gpu, POS_TITLE_H,  POS_TITLE_V, DEF_B1_FONT_SIZE, OPT_CENTER, "Export");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_TAG(0));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG_MASK(1));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('>'));
    Ft_Gpu_CoCmd_Button(&Gpu,  20, 60, 40, 40, DEF_B1_FONT_SIZE, 0, ">");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('<'));
    Ft_Gpu_CoCmd_Button(&Gpu, 20, 120, 40, 40, DEF_B1_FONT_SIZE, 0, "<");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('+'));
    Ft_Gpu_CoCmd_Button(&Gpu, 260, 60, 40, 40, DEF_B1_FONT_SIZE, 0, "+");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('-'));
    Ft_Gpu_CoCmd_Button(&Gpu, 260, 120, 40, 40, DEF_B1_FONT_SIZE, 0, "-");
    constructButtonBack(DOWN_LEFT);
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('E'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 180, 130, 40, DEF_N1_FONT_SIZE, 0, "Export");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));

    if (status->focus == 0) {
        Ft_Gpu_CoCmd_Number(&Gpu, 100, 80, DEF_N2_FONT_SIZE, OPT_CENTER, status->begin[EXPORT_MONTH]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 100, 80, DEF_N1_FONT_SIZE, OPT_CENTER, status->begin[EXPORT_MONTH]);
    }

    if (status->focus == 1) {
        Ft_Gpu_CoCmd_Number(&Gpu, 150, 80, DEF_N2_FONT_SIZE, OPT_CENTER, status->begin[EXPORT_DAY]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 150, 80, DEF_N1_FONT_SIZE, OPT_CENTER, status->begin[EXPORT_DAY]);
    }

    if (status->focus == 2) {
        Ft_Gpu_CoCmd_Number(&Gpu, 210, 80, DEF_N2_FONT_SIZE, OPT_CENTER, status->begin[EXPORT_YEAR]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 210, 80, DEF_N1_FONT_SIZE, OPT_CENTER, status->begin[EXPORT_YEAR]);
    }
    Ft_Gpu_CoCmd_Text(&Gpu, 125,  80,  DEF_N1_FONT_SIZE, OPT_CENTER, "-");
    Ft_Gpu_CoCmd_Text(&Gpu, 175,  80,  DEF_N1_FONT_SIZE, OPT_CENTER, "-");

    if (status->focus == 3) {
        Ft_Gpu_CoCmd_Number(&Gpu, 100, 140, DEF_N2_FONT_SIZE, OPT_CENTER, status->end[EXPORT_MONTH]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 100, 140, DEF_N1_FONT_SIZE, OPT_CENTER, status->end[EXPORT_MONTH]);
    }

    if (status->focus == 4) {
        Ft_Gpu_CoCmd_Number(&Gpu, 150, 140, DEF_N2_FONT_SIZE, OPT_CENTER, status->end[EXPORT_DAY]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 150, 140, DEF_N1_FONT_SIZE, OPT_CENTER, status->end[EXPORT_DAY]);
    }

    if (status->focus == 5) {
        Ft_Gpu_CoCmd_Number(&Gpu, 210, 140, DEF_N2_FONT_SIZE, OPT_CENTER, status->end[EXPORT_YEAR]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 210, 140, DEF_N1_FONT_SIZE, OPT_CENTER, status->end[EXPORT_YEAR]);
    }
    Ft_Gpu_CoCmd_Text(&Gpu, 125,  140,  DEF_N1_FONT_SIZE, OPT_CENTER, "-");
    Ft_Gpu_CoCmd_Text(&Gpu, 175,  140,  DEF_N1_FONT_SIZE, OPT_CENTER, "-");
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

static void screenSettings(void) {
    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    constructBackground();
    Ft_Gpu_CoCmd_Text(&Gpu, POS_TITLE_H,  POS_TITLE_V, DEF_B1_FONT_SIZE, OPT_CENTER, "Settings");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_TAG(0));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG_MASK(1));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('A'));
    Ft_Gpu_CoCmd_Button(&Gpu, 20, 60, 130, 40, DEF_N1_FONT_SIZE, 0, "About");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('U'));
    Ft_Gpu_CoCmd_FgColor(&Gpu, COLOR_RGB(128, 48, 12));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 60, 130, 40, DEF_N1_FONT_SIZE, 0, "Administration");
    constructButtonBack(DOWN_MIDDLE);
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

static void screenSettingsAdmin(void) {
    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    constructBackground();
    Ft_Gpu_CoCmd_Text(&Gpu, POS_TITLE_H,  POS_TITLE_V, DEF_B1_FONT_SIZE, OPT_CENTER, "Administration");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_TAG(0));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG_MASK(1));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('S'));
    Ft_Gpu_CoCmd_Button(&Gpu, 20,  60, 130, 40, DEF_N1_FONT_SIZE, 0, "Sensor Calib.");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('L'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 60, 130, 40, DEF_N1_FONT_SIZE, 0, "LCD Calib.");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('P'));
    Ft_Gpu_CoCmd_Button(&Gpu, 20,  120, 130, 40, DEF_N1_FONT_SIZE, 0, "Password");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('G'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 120, 130, 40, DEF_N1_FONT_SIZE, 0, "Parameters");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('R'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 180, 130, 40, DEF_N1_FONT_SIZE, 0, "Clock");
    constructButtonBack(DOWN_LEFT);
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

static void screenSettingsAuth(void) {
    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    constructBackground();
    Ft_Gpu_CoCmd_Text(&Gpu, POS_TITLE_H,  POS_TITLE_V, DEF_B1_FONT_SIZE, OPT_CENTER, "Enter password");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_TAG(0));
    Ft_Gpu_CoCmd_Keys(&Gpu,20, 80, 280, 40, DEF_N1_FONT_SIZE, 0, "12345");
    Ft_Gpu_CoCmd_Keys(&Gpu,20, 122, 280, 40, DEF_N1_FONT_SIZE, 0, "67890");
    constructButtonBack(DOWN_MIDDLE);
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

/*--  End of SUPPORT  --------------------------------------------------------*/

static esAction stateInit(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_INIT: {
            static esMem gObjectHeapMem = ES_MEM_INITIALIZER();                 /* STATIC: exists during execution                          */
            void *      buffer;

            wspace->retry = 10u;
            esVTimerInit(&wspace->timeout);
            esVTimerInit(&wspace->refresh);
            esMemAlloc(&StaticMem, CONFIG_GOBJECT_HEAP_SIZE, &buffer);          /* Allocate memory for gObject heap manager                 */
            esMemInit(
                &esGlobalHeapMemClass,
                &gObjectHeapMem,
                buffer,
                CONFIG_GOBJECT_HEAP_SIZE,
                0);                                                             /* Set-up heap memory                                       */
            initGobjectModule(&gObjectHeapMem, Gui);

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

            //chipid = Ft_Gpu_Hal_Rd8(&Gpu, REG_ID);

            if (chipid == 0x7C) {
                //gpuInitLate();

                return (ES_STATE_TRANSITION(stateWelcome));
            } else if (wspace->retry != 0u) {
                wspace->retry--;
                esVTimerStart(
                    &wspace->timeout,
                    ES_VTMR_TIME_TO_TICK_MS(100),
                    timeout,
                    (void *)WAKEUP_TIMEOUT_);

                return (ES_STATE_HANDLED());
            } else {

                return (ES_STATE_TRANSITION(stateInit));
            }
        }
        case WAKEUP_TIMEOUT_: {

            return (ES_STATE_TRANSITION(stateWakeUpLcd));
        }
        case ES_EXIT: {
            esVTimerCancel(&wspace->timeout);

            return (ES_STATE_HANDLED());
        }
        default: {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateWelcome(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY: {
            screenWelcome();
            //fadeIn();
            esVTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TIME_WELCOME),
                timeout,
                (void *)WELCOME_WAIT_);

            return (ES_STATE_HANDLED());
        }
        case WELCOME_WAIT_: {
            screenCalibrate();
            
            return (ES_STATE_TRANSITION(stateMain));
        }
        default: {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateMain(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_INIT: {
            struct screenMain status;

            snprintRtcTime(status.time);
            snprintRtcDate(status.date);
            snprintBatteryStatus(status.battery);
            status.isDutDetected = isDutDetected();
            screenMain(&status);
            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                timeout,
                (void *)MAIN_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case MAIN_REFRESH_: {
            switch (getKey()) {
                case 'T' : {

                    return (ES_STATE_TRANSITION(statePreTest));
                }
                case 't' : {

                    break;
                }
                case 'S' : {

                    return (ES_STATE_TRANSITION(stateSettings));
                }
                case 'E' : {

                    return (ES_STATE_TRANSITION(stateExport));
                }
                default : {

                    break;
                }
            }
            return (ES_STATE_TRANSITION(stateMain));
        }
        case ES_EXIT: {
            esVTimerCancel(&wspace->refresh);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}


static esAction statePreTest(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY: {
            struct screenTest status;
            esVTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_PRE_TEST_MS),
                timeout,
                (void *)PRE_TEST_WAIT_);
            status.title = SCREEN_TEST_PREPARING;
            status.mask  = SCREEN_TEST_EN_FIRST_STATUS;
            strcpy(status.firstThStatus, "preparing tests");
            screenTestDump(&status);
            buzzerMelody(StartNotification);

            return (ES_STATE_HANDLED());
        }
        case PRE_TEST_WAIT_: {

            if (isDutDetected()) {
                wspace->firstTh.isValid      = false;
                wspace->firstTh.isCancelled  = false;
                wspace->firstTh.isExecuted   = false;
                wspace->firstTh.rawMaxValue  = 0u;
                wspace->firstTh.time         = configGetFirstThTimeout();
                wspace->secondTh.isValid     = false;
                wspace->secondTh.isCancelled = false;
                wspace->secondTh.isExecuted  = false;
                wspace->secondTh.rawMaxValue = 0u;
                wspace->firstTh.time         = configGetSecondThTimeout();
                wspace->rawIdleVacuum        = getDutRawValue();

                return (ES_STATE_TRANSITION(stateTestFirstTh));
            } else {

                return (ES_STATE_TRANSITION(stateMain));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateTestFirstTh(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY: {
            wspace->firstTh.isExecuted = true;
            esVTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(configGetFirstThTimeout()),
                timeout,
                (void *)FIRST_TH_TIMEOUT_);
            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS),
                timeout,
                (void *)FIRST_TH_REFRESH_);
            motorEnable();

            return (ES_STATE_HANDLED());
        }
        case FIRST_TH_REFRESH_: {
            uint32_t rawValue;
            struct screenTest status;

            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS),
                timeout,
                (void *)FIRST_TH_REFRESH_);
            rawValue = getDutRawValue();
            status.title = SCREEN_TEST_1_IN_PROGRESS;
            status.mask  = SCREEN_TEST_EN_FIRST_INFO | SCREEN_TEST_EN_FIRST_PROGRESS;
            status.firstThVal      = rawValue;
            status.firstThProgress = 1024;
            screenTestDump(&status);

            if (isDutDetected()) {
                
                if (wspace->rawIdleVacuum >= rawValue) {
                    uint32_t rawVacuum;

                    rawVacuum = wspace->rawIdleVacuum - rawValue;

                    if (wspace->firstTh.rawMaxValue < rawVacuum) {
                        wspace->firstTh.rawMaxValue = rawVacuum;
                    }

                    if (rawVacuum >= configGetFirstThRawVacuum()) {
                        wspace->firstTh.isValid = true;
                        wspace->firstTh.time = configGetFirstThTimeout() -
                            esVTimerGetRemaining(&wspace->timeout);
                        wspace->secondTh.rawMaxValue = wspace->firstTh.rawMaxValue; /* Set the maxumum value for the second pass, too       */

                        return (ES_STATE_TRANSITION(stateTestSecondTh));
                    }
                }
            } else {
                motorDisable();
                wspace->firstTh.isCancelled = true;

                return (ES_STATE_TRANSITION(stateTestResults));
            }

            return (ES_STATE_HANDLED());
        }
        case FIRST_TH_TIMEOUT_: {
            motorDisable();

            return (ES_STATE_TRANSITION(stateTestResults));
        }
        case ES_EXIT : {
            esVTimerCancel(&wspace->refresh);
            esVTimerCancel(&wspace->timeout);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateTestSecondTh(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY: {
            wspace->secondTh.isExecuted = true;
            esVTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(configGetSecondThTimeout()),
                timeout,
                (void *)SECOND_TH_TIMEOUT_);
            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS),
                timeout,
                (void *)SECOND_TH_REFRESH_);
            motorEnable();

            return (ES_STATE_HANDLED());
        }
        case SECOND_TH_REFRESH_: {
            uint32_t rawValue;
            struct screenTest status;

            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS),
                timeout,
                (void *)SECOND_TH_REFRESH_);
            rawValue = getDutRawValue();
            status.title = SCREEN_TEST_2_IN_PROGRESS;
            status.mask  = SCREEN_TEST_EN_FIRST_INFO  | SCREEN_TEST_EN_FIRST_STATUS |
                           SCREEN_TEST_EN_SECOND_INFO | SCREEN_TEST_EN_SECOND_PROGRESS;
            status.firstThVal       = wspace->firstTh.rawMaxValue;
            strcpy(status.firstThStatus, "PASSED");
            status.secondThVal      = rawValue;
            status.secondThProgress = 1024;
            screenTestDump(&status);

            if (isDutDetected()) {

                if (wspace->rawIdleVacuum >= rawValue) {
                    uint32_t rawVacuum;

                    rawVacuum = wspace->rawIdleVacuum - rawValue;

                    if (wspace->secondTh.rawMaxValue < rawVacuum) {
                        wspace->secondTh.rawMaxValue = rawVacuum;
                    }

                    if (rawVacuum >= configGetFirstThRawVacuum()) {
                        wspace->secondTh.isValid = true;
                        wspace->secondTh.time = configGetSecondThTimeout() -
                            esVTimerGetRemaining(&wspace->timeout);

                        return (ES_STATE_TRANSITION(stateTestResults));
                    }
                }
            } else {
                motorDisable();
                wspace->secondTh.isCancelled = true;

                return (ES_STATE_TRANSITION(stateTestResults));
            }

            return (ES_STATE_HANDLED());
        }
        case WAKEUP_TIMEOUT_: {

            return (ES_STATE_TRANSITION(stateTestResults));
        }
        case ES_EXIT: {
            esVTimerCancel(&wspace->refresh);
            esVTimerCancel(&wspace->timeout);
            motorDisable();

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateTestResults(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY: {
            switch (evaluateTest(wspace)) {
                case TEST_SUCCESS : {
                    wspace->notification = SuccessNotification;

                    break;
                }
                case TEST_CANCELLED : {
                    wspace->notification = ConfusedNotification;

                    break;
                }
                default : {
                    wspace->notification = FailNotification;
                }
            }
            buzzerMelody(wspace->notification);
            screenTestPrepare(wspace);

            return (ES_STATE_HANDLED());
        }
        case ES_INIT: {

            return (ES_STATE_TRANSITION(stateTestResultsNotify));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateTestResultsNotify(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY : {
            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                timeout,
                (void *)TEST_RESULTS_NOTIFY_REFRESH_);
            esVTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(*wspace->notification),
                timeout,
                (void *)TEST_RESULTS_NOTIFY_TIMEOUT_);

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT: {
            esVTimerCancel(&wspace->refresh);
            esVTimerCancel(&wspace->timeout);

            return (ES_STATE_HANDLED());
        }
        case TEST_RESULTS_NOTIFY_REFRESH_: {
            if (getKey() == 'B') {

                return (ES_STATE_TRANSITION(stateTestResultsSaving));
            } else {
                esVTimerStart(
                    &wspace->refresh,
                    ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                    timeout,
                    (void *)TEST_RESULTS_NOTIFY_REFRESH_);

                return (ES_STATE_HANDLED());
            }
        }
        case TEST_RESULTS_NOTIFY_TIMEOUT_: {
            screenTestDump(&wspace->screen.test);
            screenTestToggleBackground(wspace);
            wspace->notification++;

            if (*wspace->notification != 0) {
                esVTimerStart(
                    &wspace->timeout,
                    ES_VTMR_TIME_TO_TICK_MS(*wspace->notification),
                    timeout,
                    (void *)TEST_RESULTS_NOTIFY_TIMEOUT_);
            }
            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateTestResultsSaving(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY: {
            screenTestSaving();
            esVTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(1000),
                timeout,
                (void *)WAKEUP_TIMEOUT_);

            return (ES_STATE_HANDLED());
        }
        case WAKEUP_TIMEOUT_: {

            return (ES_STATE_TRANSITION(stateMain));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}
static esAction stateSettings(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY: {
            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                timeout,
                (void *)SETTINGS_REFRESH_);
            screenSettings();

            return (ES_STATE_HANDLED());
        }
        case SETTINGS_REFRESH_: {

            switch (getKey()) {
                case 'A' : {

                    return (ES_STATE_TRANSITION(stateSettingsAbout));
                }
                case 'U' : {

                    return (ES_STATE_TRANSITION(stateSettingsAdmin));
                }
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateMain));
                }
                default: {
                    break;
                }
            }
            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                timeout,
                (void *)SETTINGS_REFRESH_);
            
            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSettingsAbout(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY: {
            screenSettingsAbout();
            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                timeout,
                (void *)SETTINGS_ABOUT_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case SETTINGS_ABOUT_REFRESH_: {
            switch (getKey()) {
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateSettings));
                }
            }
            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                timeout,
                (void *)SETTINGS_ABOUT_REFRESH_);
            
            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSettingsAuthorize(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_INIT: {
            return (ES_STATE_TRANSITION(stateSettingsAdmin));
        }
    }
    return (ES_STATE_IGNORED());
}

static esAction stateSettingsLcdCalib(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY: {
            screenCalibrate();

            return (ES_STATE_HANDLED());
        }
        case ES_INIT: {

            return (ES_STATE_TRANSITION(stateSettingsAdmin));
        }
        default: {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSettingsAdmin(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY: {
            screenSettingsAdmin();
            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                timeout,
                (void *)EXPORT_INSERT_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case EXPORT_INSERT_REFRESH_: {
            switch (getKey()) {
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateSettings));
                }
                case 'L' : {

                    return (ES_STATE_TRANSITION(stateSettingsLcdCalib));
                }
            }
            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                timeout,
                (void *)EXPORT_INSERT_REFRESH_);

            return (ES_STATE_HANDLED());
        }
    }
    return (ES_STATE_IGNORED());
}

static esAction stateExport(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY: {

            return (ES_STATE_HANDLED());
        }
        case ES_INIT: {
            if (isUsbDetected()) {

                return (ES_STATE_TRANSITION(stateExportMount));
            } else {

                return (ES_STATE_TRANSITION(stateExportInsert));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateExportInsert(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY: {
            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                timeout,
                (void *)EXPORT_INSERT_REFRESH_);
            screenExportInsert();

            return (ES_STATE_HANDLED());
        }
        case EXPORT_INSERT_REFRESH_: {
            if (getKey() == 'B') {

                return (ES_STATE_TRANSITION(stateMain));
            } else if (isUsbDetected()) {

                return (ES_STATE_TRANSITION(stateExportMount));
            } else {
                esVTimerStart(
                    &wspace->refresh,
                    ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                    timeout,
                    (void *)EXPORT_INSERT_REFRESH_);

                return (ES_STATE_HANDLED());
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateExportMount(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY : {
            screenExportMount();

            return (ES_STATE_HANDLED());
        }
        case ES_INIT: {
            if (isUsbMounted()) {

                return (ES_STATE_TRANSITION(stateExportChoose));
            } else {

                return (ES_STATE_TRANSITION(stateMain));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateExportChoose(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY: {
            wspace->screen.exportChoose.begin[EXPORT_DAY]   = 20;
            wspace->screen.exportChoose.begin[EXPORT_MONTH] = 4;
            wspace->screen.exportChoose.begin[EXPORT_YEAR]  = 2014;
            wspace->screen.exportChoose.end[EXPORT_DAY]   = 20;
            wspace->screen.exportChoose.end[EXPORT_MONTH] = 4;
            wspace->screen.exportChoose.end[EXPORT_YEAR]  = 2014;
            wspace->screen.exportChoose.focus             = 0;
            screenExportChoose(&wspace->screen.exportChoose);
            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                timeout,
                (void *)EXPORT_CHOOSE_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case EXPORT_CHOOSE_REFRESH_: {

            if (isUsbDetected() == false) {
                
                return (ES_STATE_TRANSITION(stateMain));
            }
            switch (getKey()) {
                case 'B': {

                    return (ES_STATE_TRANSITION(stateMain));
                }
                case '>' : {
                    if (wspace->screen.exportChoose.focus != 5u) {
                        wspace->screen.exportChoose.focus++;
                    }
                    break;
                }
                case '<' : {
                    if (wspace->screen.exportChoose.focus != 0u) {
                        wspace->screen.exportChoose.focus--;
                    }
                    break;
                }
                case '+' : {
                    if (wspace->screen.exportChoose.focus < 3) {
                        wspace->screen.exportChoose.begin[wspace->screen.exportChoose.focus]++;
                    } else {
                        wspace->screen.exportChoose.end[wspace->screen.exportChoose.focus - 3]++;
                    }
                    break;
                }
                case '-' : {
                    if (wspace->screen.exportChoose.focus < 3) {
                        wspace->screen.exportChoose.begin[wspace->screen.exportChoose.focus]--;
                    } else {
                        wspace->screen.exportChoose.end[wspace->screen.exportChoose.focus - 3]--;
                    }
                    break;
                }
                case 'E' : {

                    return (ES_STATE_TRANSITION(stateExportSaving));
                }
                default: {

                }
            }
            screenExportChoose(&wspace->screen.exportChoose);
            esVTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                timeout,
                (void *)EXPORT_CHOOSE_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}


static esAction stateExportSaving(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY: {
            screenExportSaving();
            esVTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(2000),
                timeout,
                (void *)WAKEUP_TIMEOUT_);

            return (ES_STATE_HANDLED());
        }
        case WAKEUP_TIMEOUT_: {

            return (ES_STATE_TRANSITION(stateExport));
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

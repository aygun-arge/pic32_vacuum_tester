
/*=========================================================  INCLUDE FILES  ==*/

#include "epa_gui.h"
#include "eds/epa.h"
#include "vtimer/vtimer.h"

#include "HardwareProfile.h"
#include "software_profile.h"

#include "app_config.h"
#include "app_motor.h"
#include "app_psensor.h"
#include "app_battery.h"
#include "app_usb.h"
#include "app_timer.h"
#include "app_time.h"
#include "app_gpu.h"
#include "main.h"

#include "logo.h"
#include "app_buzzer.h"
#include "app_storage.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CONFIG_PRE_TEST_MS              500
#define CONFIG_TEST_CANCEL_MS           5000
#define CONFIG_TEST_FAIL_MS             5000
#define CONFIG_TEST_OVERVIEW_MS         5000
#define CONFIG_TEST_REFRESH_MS          20
#define CONFIG_TOUCH_REFRESH_MS         20
#define CONFIG_MAIN_REFRESH_MS          100

#define DEF_VACUUM_UNIT                 "\"Hg"

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
#define SCREEN_TEST_BACK_BUTTON         (0x1u << 6)
#define SCREEN_TEST_RETRY_BUTTON        (0x1u << 7)

#define GUI_TABLE(entry)                                                        \
    entry(stateInit,                TOP)                                        \
    entry(stateWakeUpDisplay,       TOP)                                        \
    entry(stateSetupTouch,          TOP)                                        \
    entry(stateMain,                TOP)                                        \
    entry(statePreTest,             TOP)                                        \
    entry(stateTestFirstTh,         TOP)                                        \
    entry(stateTestSecondTh,        TOP)                                        \
    entry(stateTestResults,         TOP)                                        \
    entry(stateTestResultsNotify,   TOP)                                        \
    entry(stateTestResultsSaving,   TOP)                                        \
    entry(stateSettings,            TOP)                                        \
    entry(stateSettingsAbout,       TOP)                                        \
    entry(stateSettingsAuthorize,   TOP)                                        \
    entry(stateSettingsAdmin,       TOP)                                        \
    entry(stateSettingsClock,       TOP)                                        \
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
    SETTINGS_AUTH_REFRESH_,
    SETTINGS_ABOUT_REFRESH_,
    EXPORT_INSERT_REFRESH_,
    EXPORT_CHOOSE_REFRESH_
};

enum testTitle {
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
    bool                isRetryEnabled;
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
    struct appTimer     timeout;
    struct appTimer     refresh;
    union state {
        struct wakeUpLcd {
            uint32_t            retry;
        }                   wakeUpLcd;
        struct main {
            bool                isDutInPlace;
            char                battery[20];
            char                time[20];
            char                date[20];
        }                   main;
        struct test {
            struct testStatus {
                bool                isValid;
                bool                isCancelled;
                bool                isExecuted;
                uint32_t            rawMaxValue;
                uint32_t            time;
            }                   th[2];
            uint32_t            rawIdleVacuum;
            bool                isBackgroundEnabled;
            bool                isRetryEnabled;
            uint32_t            mask;
            int32_t             firstThVal;
            uint32_t            firstThProgress;
            int32_t             secondThVal;
            uint32_t            secondThProgress;
            uint32_t            count;
        }                   test;
        struct settingsAuthorize {
            uint32_t            counter;
            uint32_t            numOfCharactes;
        }                   settingsAuthorize;
        struct settingsClock {
            uint32_t            focus;
            struct appTime      time;
        }                   settingsClock;
    }                   state;
    const uint8_t *     notification;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void screenWelcome(void);
static void screenMain(const union state * state);
static void screenExportInsert(void);
static void screenSettings(void);

static esAction stateInit               (struct wspace *, const esEvent *);
static esAction stateWakeUpDisplay      (struct wspace *, const esEvent *);
static esAction stateSetupTouch         (struct wspace *, const esEvent *);
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
static esAction stateSettingsAuthorize  (struct wspace *, const esEvent *);
static esAction stateSettingsAdmin      (struct wspace *, const esEvent *);
static esAction stateSettingsClock      (struct wspace *, const esEvent *);
static esAction stateSettingsLcdCalib   (struct wspace *, const esEvent *);
static esAction stateExport             (struct wspace *, const esEvent *);
static esAction stateExportInsert       (struct wspace *, const esEvent *);
static esAction stateExportMount        (struct wspace *, const esEvent *);
static esAction stateExportChoose       (struct wspace *, const esEvent *);
static esAction stateExportSaving       (struct wspace *, const esEvent *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("GUI", CONFIG_EPA_GUI_NAME, "Nenad Radulovic");

static const esSmTable      GuiTable[] = ES_STATE_TABLE_INIT(GUI_TABLE);

static const uint8_t OkNotification[] = {20, 100, 20, 0};
static const uint8_t FailNotification[] = {150, 150, 175, 175, 200, 0};
static const uint8_t ConfusedNotification[] = {20, 100, 20, 100, 40, 100, 40, 100, 60, 0};
static const uint8_t SuccessNotification[] = {40, 100, 40, 100, 40, 0};

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


static void constructBackground(void) {
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_COLOR_RGB(224, 224, 224));
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR(1, 0, 0));
    Ft_Gpu_CoCmd_Gradient(&Gpu, 0,0, 0x707070, 0, DISP_HEIGHT, 0xe0e0e0);
}

static void constructTitle(const char * title) {
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));
    Ft_Gpu_CoCmd_Text(&Gpu, POS_TITLE_H,  POS_TITLE_V, DEF_B1_FONT_SIZE, OPT_CENTER, title);
}

static void constructButtonBack(enum buttonBackPos position) {
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('B'));
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_CoCmd_FgColor(&Gpu, COLOR_RGB(8, 120, 40));

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
    Ft_Gpu_Hal_WrMem(&Gpu, RAM_G + 131072L, (const uint8_t *)ManufacturerLogo, ManufacturerLogoInfo.size);				/* copy data continuously into RAM_G memory */
    gpuBegin();
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR(1,0,0));
    Ft_Gpu_Hal_WrCmd32(&Gpu, BITMAP_HANDLE(13));
    Ft_Gpu_Hal_WrCmd32(&Gpu, BITMAP_SOURCE(131072L));
    Ft_Gpu_Hal_WrCmd32(&Gpu, BITMAP_LAYOUT(ManufacturerLogoInfo.format, ManufacturerLogoInfo.linestride, ManufacturerLogoInfo.height));
    Ft_Gpu_Hal_WrCmd32(&Gpu, BITMAP_SIZE(NEAREST, BORDER, BORDER, ManufacturerLogoInfo.pixelsX, ManufacturerLogoInfo.pixelsY));
    Ft_Gpu_Hal_WrCmd32(&Gpu, BEGIN(BITMAPS));
    Ft_Gpu_Hal_WrCmd32(&Gpu, VERTEX2II(35, 10, 13, 0));
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 80,  DEF_B1_FONT_SIZE, OPT_CENTER, WELCOME_GREETING);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 120, DEF_N1_FONT_SIZE, OPT_CENTER, WELCOME_HW_VERSION CONFIG_HARDWARE_VERSION);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 140, DEF_N1_FONT_SIZE, OPT_CENTER, WELCOME_SW_VERSION CONFIG_SOFTWARE_VERSION);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 160, DEF_N1_FONT_SIZE, OPT_CENTER, BUILD_DATE);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 180, DEF_N1_FONT_SIZE, OPT_CENTER, BUILD_TIME);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 220, DEF_N1_FONT_SIZE, OPT_CENTER, DEF_WEBSITE);
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

static void screenSettingsAbout(void) {
    gpuBegin();
    constructBackground();
    constructTitle("About");
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 120, DEF_N1_FONT_SIZE, OPT_CENTER, WELCOME_HW_VERSION CONFIG_HARDWARE_VERSION);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 140, DEF_N1_FONT_SIZE, OPT_CENTER, WELCOME_SW_VERSION CONFIG_SOFTWARE_VERSION);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 160, DEF_N1_FONT_SIZE, OPT_CENTER, DEF_WEBSITE);
    constructButtonBack(DOWN_MIDDLE);
    gpuEnd();
}

static void screenSettingsClock(const union state * state) {
    uint32_t            textSize[7];
    char                buffer[10];

    gpuBegin();
    constructBackground();
    constructTitle("Clock");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('>'));
    Ft_Gpu_CoCmd_Button(&Gpu,  20, 60, 40, 40, DEF_B1_FONT_SIZE, 0, ">");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('<'));
    Ft_Gpu_CoCmd_Button(&Gpu, 20, 120, 40, 40, DEF_B1_FONT_SIZE, 0, "<");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('+'));
    Ft_Gpu_CoCmd_Button(&Gpu, 260, 60, 40, 40, DEF_B1_FONT_SIZE, 0, "+");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('-'));
    Ft_Gpu_CoCmd_Button(&Gpu, 260, 120, 40, 40, DEF_B1_FONT_SIZE, 0, "-");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('S'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 180, 130, 40, DEF_N1_FONT_SIZE, 0, "Set");
    constructButtonBack(DOWN_LEFT);
    textSize[0] = DEF_N1_FONT_SIZE;
    textSize[1] = DEF_N1_FONT_SIZE;
    textSize[2] = DEF_N1_FONT_SIZE;
    textSize[3] = DEF_N1_FONT_SIZE;
    textSize[4] = DEF_N1_FONT_SIZE;
    textSize[5] = DEF_N1_FONT_SIZE;
    textSize[6] = DEF_N1_FONT_SIZE;
    textSize[state->settingsClock.focus] = DEF_N2_FONT_SIZE;
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));
    Ft_Gpu_CoCmd_Number(&Gpu, 100, 80,  textSize[0], OPT_CENTER, state->settingsClock.time.hour);
    Ft_Gpu_CoCmd_Number(&Gpu, 140, 80,  textSize[1], OPT_CENTER, state->settingsClock.time.minute);
    Ft_Gpu_CoCmd_Number(&Gpu, 180, 80,  textSize[2], OPT_CENTER, state->settingsClock.time.seconds);
    snprintRtcDaySelector(&state->settingsClock.time, buffer);
    Ft_Gpu_CoCmd_Text(&Gpu,   230, 80,  textSize[3], OPT_CENTER, buffer);
    Ft_Gpu_CoCmd_Number(&Gpu, 100, 140, textSize[4], OPT_CENTER, state->settingsClock.time.month);
    Ft_Gpu_CoCmd_Number(&Gpu, 150, 140, textSize[5], OPT_CENTER, state->settingsClock.time.day);
    Ft_Gpu_CoCmd_Number(&Gpu, 210, 140, textSize[6], OPT_CENTER, state->settingsClock.time.year);
    gpuEnd();
}

static void screenCalibrate(void) {
    gpuBegin();
    constructBackground();
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 80, DEF_B1_FONT_SIZE, OPT_CENTER, "Touch Calibration");
    Ft_Gpu_CoCmd_Text(&Gpu,DISP_WIDTH / 2 ,DISP_HEIGHT/2,26,OPT_CENTERX|OPT_CENTERY,"Please tap on the dot");
    Ft_Gpu_CoCmd_Calibrate(&Gpu, 0);
    gpuEnd();
}

static void screenMain(const union state * state) {
    char *              text;

    gpuBegin();
    constructBackground();
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('S'));
    Ft_Gpu_CoCmd_Button(&Gpu, 20,  20, 130,  40, DEF_N1_FONT_SIZE, 0, "Settings");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('E'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 20, 130,  40, DEF_N1_FONT_SIZE, 0, "Export");

    if (state->main.isDutInPlace) {
        Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
        Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('T'));
        Ft_Gpu_CoCmd_Button(&Gpu, 80,  80, 160, 80, DEF_B1_FONT_SIZE, 0, "TEST");
        Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));
        text = "Porator is detected";
    } else {
        Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(92, 92, 92));
        Ft_Gpu_CoCmd_FgColor(&Gpu, COLOR_RGB(112, 112, 112));
        Ft_Gpu_CoCmd_Button(&Gpu, 80,  80, 160, 80, DEF_B1_FONT_SIZE, 0, "TEST");
        Ft_Gpu_CoCmd_ColdStart(&Gpu);
        Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 0, 0));
        text = "Put the porator on the test pad.";
    }
    Ft_Gpu_CoCmd_Text(&Gpu, 160, 185, DEF_N1_FONT_SIZE, OPT_CENTER, text);
    Ft_Gpu_Hal_WrCmd32(&Gpu,          COLOR_RGB(0, 0, 0));
    Ft_Gpu_CoCmd_Text(&Gpu, 140, 225, DEF_N1_FONT_SIZE, OPT_CENTERY, state->main.date);
    Ft_Gpu_CoCmd_Text(&Gpu, 240, 225, DEF_N1_FONT_SIZE, OPT_CENTERY, state->main.time);
    Ft_Gpu_CoCmd_Text(&Gpu, 10,  225, DEF_N1_FONT_SIZE, OPT_CENTERY, "BAT:");
    Ft_Gpu_CoCmd_Text(&Gpu, 50, 225,  DEF_N1_FONT_SIZE, OPT_CENTERY, state->main.battery);
    Ft_Gpu_Hal_WrCmd32(&Gpu, BEGIN(LINES));
    Ft_Gpu_Hal_WrCmd32(&Gpu, VERTEX2II(10,  210, 0, 64));
    Ft_Gpu_Hal_WrCmd32(&Gpu, VERTEX2II(310, 210, 0, 64));
    Ft_Gpu_Hal_WrCmd32(&Gpu, END());
    gpuEnd();
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

    wspace->screen.test.mask = 0;
    
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
        strcpy(wspace->screen.test.button, "Repeat");
        wspace->screen.test.mask = SCREEN_TEST_RETRY_BUTTON;
    } else if (wspace->secondTh.isExecuted == false) {
        strcpy(wspace->screen.test.secondThStatus, "SKIPPED");
        strcpy(wspace->screen.test.button, "Repeat");
        wspace->screen.test.mask = SCREEN_TEST_RETRY_BUTTON;
    } else {
        strcpy(wspace->screen.test.secondThStatus, "FAILED");
        strcpy(wspace->screen.test.button, "Retry");
        wspace->screen.test.mask = SCREEN_TEST_RETRY_BUTTON;
    }
    wspace->screen.test.mask |=
        SCREEN_TEST_EN_FIRST_INFO  | SCREEN_TEST_EN_FIRST_STATUS  |
        SCREEN_TEST_EN_SECOND_INFO | SCREEN_TEST_EN_SECOND_STATUS |
        SCREEN_TEST_BACK_BUTTON;
    wspace->screen.test.firstThVal  = wspace->firstTh.rawMaxValue;
    wspace->screen.test.secondThVal = wspace->secondTh.rawMaxValue;
    wspace->screen.test.isBackgroundEnabled = true;
    
}

static void screenPreTest(const union state * state) {
    (void)state;
    gpuBegin();
    constructBackground();
    constructTitle("Test in progress");
    Ft_Gpu_CoCmd_Text(&Gpu, POS_COLUMN_HALF,  POS_ROW_1_5, DEF_N1_FONT_SIZE, OPT_CENTER,
        "preparing tests");
    gpuEnd();
}

static void screenTestTh0(const union state * state) {
    gpuBegin();
    constructBackground();
    constructTitle("Test in progress");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_4,   POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY,
        "First threshold");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_18,  POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY,
        "[" DEF_VACUUM_UNIT "]:");
    Ft_Gpu_CoCmd_Number(&Gpu, POS_COLUMN_25,  POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY,
        state->test.th[0].rawMaxValue);
    Ft_Gpu_CoCmd_Progress(&Gpu, POS_COLUMN_4, POS_ROW_1_5 - 5, DISP_WIDTH - (POS_COLUMN_4 * 2), 10,
        0, state->test.th[0].rawMaxValue, state->test.rawIdleVacuum);
    gpuEnd();
}

static void screenTestTh1(const union state * state) {
    gpuBegin();
    constructBackground();
    constructTitle("Test in progress");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_4,   POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY,
        "First threshold");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_18,  POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY,
        "[" DEF_VACUUM_UNIT "]:");
    Ft_Gpu_CoCmd_Number(&Gpu, POS_COLUMN_25,  POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY,
        state->test.th[0].rawMaxValue);
    Ft_Gpu_CoCmd_Text(&Gpu, POS_COLUMN_HALF,  POS_ROW_1_5, DEF_N1_FONT_SIZE, OPT_CENTER, "PASSED");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_4,   POS_ROW_2, DEF_N1_FONT_SIZE, OPT_CENTERY,
        "Second threshold");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_18,  POS_ROW_2, DEF_N1_FONT_SIZE, OPT_CENTERY,
        "[" DEF_VACUUM_UNIT "]:");
    Ft_Gpu_CoCmd_Number(&Gpu, POS_COLUMN_25,  POS_ROW_2, DEF_N1_FONT_SIZE, OPT_CENTERY,
        state->test.th[1].rawMaxValue);
    Ft_Gpu_CoCmd_Progress(&Gpu, POS_COLUMN_4, POS_ROW_2_5 - 5, DISP_WIDTH - (POS_COLUMN_4 * 2), 10,
        0, state->test.th[1].rawMaxValue, state->test.rawIdleVacuum);
    gpuEnd();
}

static void screenTestDump(const union state * state) {
    gpuBegin();

    switch (state->test.title) {
        case SCREEN_TEST_1_IN_PROGRESS :
        case SCREEN_TEST_2_IN_PROGRESS : {
            constructBackground();
            constructTitle("Test in progress");
            break;
        }
        case SCREEN_TEST_FAILED : {
            if (state->test.isBackgroundEnabled) {
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
            if (state->test.isBackgroundEnabled) {
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
            if (state->test.isBackgroundEnabled) {
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

    if (status->mask & SCREEN_TEST_BACK_BUTTON) {
        constructButtonBack(DOWN_LEFT);
    }

    if (status->mask & SCREEN_TEST_RETRY_BUTTON) {
        Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
        Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_TAG(0));
        Ft_Gpu_Hal_WrCmd32(&Gpu, TAG_MASK(1));
        Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('R'));
        Ft_Gpu_CoCmd_Button(&Gpu, 170, 180, 130, 40, DEF_N1_FONT_SIZE, 0, status->button);
    }
    gpuEnd();
}

static void screenTestSaving(void) {
    static uint32_t record = 12;
    gpuBegin();
    constructBackground();
    constructTitle("Saving...");
    Ft_Gpu_CoCmd_Text(&Gpu, 160,  200, DEF_N1_FONT_SIZE, OPT_CENTER, "Record number:");
    Ft_Gpu_CoCmd_Number(&Gpu, 240,  200, DEF_N1_FONT_SIZE, OPT_CENTER, record++);
    Ft_Gpu_CoCmd_Spinner(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, 0, 0);
    gpuEnd();
}

static void screenExportInsert(void) {
    gpuBegin();
    constructBackground();
    constructTitle("Export");
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, DEF_N1_FONT_SIZE, OPT_CENTER, "Please insert USB flash drive");
    constructButtonBack(DOWN_MIDDLE);
    gpuEnd();
}

static void screenExportMount(void) {
    gpuBegin();
    constructBackground();
    constructTitle("Export");
    Ft_Gpu_CoCmd_Spinner(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, 0, 0);
    gpuEnd();
}

static void screenExportSaving(void) {
    gpuBegin();
    constructBackground();
    constructTitle("Saving data...");
    Ft_Gpu_CoCmd_Spinner(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, 0, 0);
    gpuEnd();
}

#define EXPORT_MONTH                    0
#define EXPORT_DAY                      1
#define EXPORT_YEAR                     2

static void screenExportChoose(struct screenExportChoose * status) {
    gpuBegin();
    constructBackground();
    constructTitle("Export");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
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
    gpuEnd();
}

static void screenSettings(void) {
    gpuBegin();
    constructBackground();
    constructTitle("Settings");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('A'));
    Ft_Gpu_CoCmd_Button(&Gpu, 20, 60, 130, 40, DEF_N1_FONT_SIZE, 0, "About");
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('U'));
    Ft_Gpu_CoCmd_FgColor(&Gpu, COLOR_RGB(128, 48, 12));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 60, 130, 40, DEF_N1_FONT_SIZE, 0, "Administration");
    constructButtonBack(DOWN_MIDDLE);
    gpuEnd();
}

static void screenSettingsAdmin(void) {
    gpuBegin();
    constructBackground();
    constructTitle("Administration");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
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
    gpuEnd();
}

static void screenSettingsAuth(void) {
    gpuBegin();
    constructBackground();
    constructTitle("Enter password");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_CoCmd_Keys(&Gpu,20, 80, 280, 40, DEF_N1_FONT_SIZE, 0, "12345");
    Ft_Gpu_CoCmd_Keys(&Gpu,20, 122, 280, 40, DEF_N1_FONT_SIZE, 0, "67890");
    constructButtonBack(DOWN_MIDDLE);
    gpuEnd();
}

/*--  End of SUPPORT  --------------------------------------------------------*/

static esAction stateInit(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_INIT: {
            wspace->state.wakeUpLcd.retry = 100u;
            appTimerInit(&wspace->timeout);
            appTimerInit(&wspace->refresh);

            return (ES_STATE_TRANSITION(stateWakeUpDisplay));
        }
        default: {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateWakeUpDisplay(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_INIT : {

            if (isGpuReady()) {
                gpuSetupDisplay();

                return (ES_STATE_TRANSITION(stateSetupTouch));
            } else if (wspace->state.wakeUpLcd.retry != 0u) {
                wspace->state.wakeUpLcd.retry--;
                appTimerStart(
                    &wspace->timeout,
                    ES_VTMR_TIME_TO_TICK_MS(10),
                    WAKEUP_TIMEOUT_);

                return (ES_STATE_HANDLED());
            } else {

                return (ES_STATE_TRANSITION(stateInit));
            }
        }
        case WAKEUP_TIMEOUT_: {

            return (ES_STATE_TRANSITION(stateWakeUpDisplay));
        }
        case ES_EXIT: {
            appTimerCancel(&wspace->timeout);

            return (ES_STATE_HANDLED());
        }
        default: {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetupTouch(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   request;
            esError     error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_TOUCH_INITIALIZE, &request));

            if (error == ES_ERROR_NONE) {
                ES_ENSURE(esEpaSendEvent(Touch, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_STATUS : {
            const struct touchStatusEvent * touchStatusEvent;

            touchStatusEvent = (const struct touchStatusEvent *)event;

            if (touchStatusEvent->status == TOUCH_INITIALIZED) {

                return (ES_STATE_TRANSITION(stateWelcome));
            } else if (touchStatusEvent->status == TOUCH_NOT_INITIALIZED) {
                esEvent * request;
                esError   error;
                gpuFadeIn();
                screenCalibrate();
                ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_TOUCH_CALIBRATE, &request));

                if (error == ES_ERROR_NONE) {
                    ES_ENSURE(esEpaSendEvent(Touch, request));
                }
            } else if (touchStatusEvent->status == TOUCH_NOT_CALIBRATED) {
                /* Even if we fail the calibration continue and hope the defaults will work */

                return (ES_STATE_TRANSITION(stateWelcome));
            }

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateWelcome(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY: {
            screenWelcome();
            gpuFadeIn();
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TIME_WELCOME),
                WELCOME_WAIT_);

            return (ES_STATE_HANDLED());
        }
        case WELCOME_WAIT_: {
            
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
            struct appTime time;
            esEvent *      request;

            appTimeGet(&time);
            snprintRtcTime(&time, wspace->state.main.time);
            snprintRtcDate(&time, wspace->state.main.date);
            snprintBatteryStatus(wspace->state.main.battery);
            wspace->state.main.isDutInPlace = isDutDetected();
            screenMain(&wspace->state);
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                MAIN_REFRESH_);
            ES_ENSURE(esEventCreate(sizeof(*request), EVT_TOUCH_ENABLE, &request));
            ES_ENSURE(esEpaSendEvent(Touch, request));

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touch;

            touch = (const struct touchEvent *)event;

            switch (touch->tag) {
                case 'T' : {

                    return (ES_STATE_TRANSITION(statePreTest));
                }
                case 'S' : {

                    return (ES_STATE_TRANSITION(stateSettings));
                }
                case 'E' : {

                    return (ES_STATE_TRANSITION(stateExport));
                }
                default : {

                    return (ES_STATE_HANDLED());
                }
            }
        }
        case MAIN_REFRESH_: {
            struct appTime time;

            appTimeGet(&time);
            snprintRtcTime(&time, wspace->state.main.time);
            snprintRtcDate(&time, wspace->state.main.date);
            snprintBatteryStatus(wspace->state.main.battery);
            wspace->state.main.isDutInPlace = isDutDetected();
            screenMain(&wspace->state);
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                MAIN_REFRESH_);
            
            return (ES_STATE_HANDLED());
        }
        case ES_EXIT: {
            esEvent *      request;

            appTimerCancel(&wspace->refresh);
            ES_ENSURE(esEventCreate(sizeof(*request), EVT_TOUCH_DISABLE, &request));
            ES_ENSURE(esEpaSendEvent(Touch, request));

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
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_PRE_TEST_MS),
                PRE_TEST_WAIT_);
            screenPreTest(&wspace->state);
            buzzerMelody(OkNotification);

            return (ES_STATE_HANDLED());
        }
        case PRE_TEST_WAIT_: {

            if (isDutDetected()) {
                wspace->state.test.count = 1u;
                wspace->state.test.th[0].isValid     = false;
                wspace->state.test.th[0].isCancelled = false;
                wspace->state.test.th[0].isExecuted  = false;
                wspace->state.test.th[0].rawMaxValue = 0u;
                wspace->state.test.th[0].time        = configGetFirstThTimeout();
                wspace->state.test.th[1].isValid     = false;
                wspace->state.test.th[1].isCancelled = false;
                wspace->state.test.th[1].isExecuted  = false;
                wspace->state.test.th[1].rawMaxValue = 0u;
                wspace->state.test.th[1].time        = configGetSecondThTimeout();
                wspace->state.test.rawIdleVacuum     = getDutRawValue();

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
            wspace->state.test.th[0].isExecuted = true;
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(configGetFirstThTimeout()),
                FIRST_TH_TIMEOUT_);
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS),
                FIRST_TH_REFRESH_);
            motorEnable();

            return (ES_STATE_HANDLED());
        }
        case FIRST_TH_REFRESH_: {
            
            if (isDutDetected()) {
                uint32_t rawValue;

                rawValue = getDutRawValue();
                
                if (wspace->state.test.rawIdleVacuum >= rawValue) {
                    uint32_t rawVacuum;

                    rawVacuum = wspace->state.test.rawIdleVacuum - rawValue;

                    if (wspace->state.test.th[0].rawMaxValue < rawVacuum) {
                        wspace->state.test.th[0].rawMaxValue = rawVacuum;
                    }

                    if (rawVacuum >= configGetFirstThRawVacuum()) {
                        wspace->state.test.th[0].isValid = true;
                        wspace->state.test.th[0].time = configGetFirstThTimeout() -
                            appTimerGetRemaining(&wspace->timeout);
                        wspace->state.test.th[1].rawMaxValue = wspace->state.test.th[0].rawMaxValue; /* Set the maxumum value for the second pass, too       */

                        return (ES_STATE_TRANSITION(stateTestSecondTh));
                    }
                    screenTestTh0(&wspace->state);
                }
                appTimerStart(
                    &wspace->refresh,
                    ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS),
                    FIRST_TH_REFRESH_);
            } else {
                motorDisable();
                wspace->state.test.th[0].isCancelled = true;

                return (ES_STATE_TRANSITION(stateTestResults));
            }

            return (ES_STATE_HANDLED());
        }
        case FIRST_TH_TIMEOUT_: {
            motorDisable();

            return (ES_STATE_TRANSITION(stateTestResults));
        }
        case ES_EXIT : {
            appTimerCancel(&wspace->refresh);
            appTimerCancel(&wspace->timeout);

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
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(configGetSecondThTimeout()),
                SECOND_TH_TIMEOUT_);
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS),
                SECOND_TH_REFRESH_);
            motorEnable();

            return (ES_STATE_HANDLED());
        }
        case SECOND_TH_REFRESH_: {
            uint32_t rawValue;
            struct screenTest status;

            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS),
                SECOND_TH_REFRESH_);
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
                            appTimerGetRemaining(&wspace->timeout);

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
            appTimerCancel(&wspace->refresh);
            appTimerCancel(&wspace->timeout);
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
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                TEST_RESULTS_NOTIFY_REFRESH_);
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(*wspace->notification),
                TEST_RESULTS_NOTIFY_TIMEOUT_);

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT: {
            appTimerCancel(&wspace->refresh);
            appTimerCancel(&wspace->timeout);

            return (ES_STATE_HANDLED());
        }
        case TEST_RESULTS_NOTIFY_REFRESH_: {
            switch (gpuGetKey()) {
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateTestResultsSaving));
                }
                case 'R' : {

                    return (ES_STATE_TRANSITION(statePreTest));
                }
            }
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                TEST_RESULTS_NOTIFY_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case TEST_RESULTS_NOTIFY_TIMEOUT_: {
            screenTestDump(&wspace->screen.test);
            screenTestToggleBackground(wspace);
            wspace->notification++;

            if (*wspace->notification != 0) {
                appTimerStart(
                    &wspace->timeout,
                    ES_VTMR_TIME_TO_TICK_MS(*wspace->notification),
                    TEST_RESULTS_NOTIFY_TIMEOUT_);
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
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(1000),
                WAKEUP_TIMEOUT_);

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
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                SETTINGS_REFRESH_);
            screenSettings();

            return (ES_STATE_HANDLED());
        }
        case SETTINGS_REFRESH_: {

            switch (gpuGetKey()) {
                case 'A' : {

                    return (ES_STATE_TRANSITION(stateSettingsAbout));
                }
                case 'U' : {

                    return (ES_STATE_TRANSITION(stateSettingsAuthorize));
                }
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateMain));
                }
                default: {
                    break;
                }
            }
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                SETTINGS_REFRESH_);
            
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
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                SETTINGS_ABOUT_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case SETTINGS_ABOUT_REFRESH_: {
            switch (gpuGetKey()) {
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateSettings));
                }
            }
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                SETTINGS_ABOUT_REFRESH_);
            
            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSettingsAuthorize(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY : {
            screenSettingsAuth();
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                SETTINGS_AUTH_REFRESH_);
            wspace->state.settingsAuthorize.numOfCharactes = configPasswordLength();
            wspace->state.settingsAuthorize.counter        = 0u;

            return (ES_STATE_HANDLED());
        }
        case SETTINGS_AUTH_REFRESH_ :{
            char        key;

            switch (key = gpuGetKey()) {
                case 'B' : {
                    
                    return (ES_STATE_TRANSITION(stateSettings));
                }
                default : {
                    if (key != 0) {

                        if (configIsPasswordCharValid(key, wspace->state.settingsAuthorize.counter)) {
                            wspace->state.settingsAuthorize.counter++;
                            
                            if (wspace->state.settingsAuthorize.counter ==
                                wspace->state.settingsAuthorize.numOfCharactes) {
                                buzzerMelody(OkNotification);

                                return (ES_STATE_TRANSITION(stateSettingsAdmin));
                            }
                        } else {
                            buzzerMelody(ConfusedNotification);

                            return (ES_STATE_TRANSITION(stateSettings));
                        }
                        buzzerTone(20);
                    }
                }
            }
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                SETTINGS_AUTH_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
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
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                EXPORT_INSERT_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case EXPORT_INSERT_REFRESH_: {
            switch (gpuGetKey()) {
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateSettings));
                }
                case 'R' : {

                    return (ES_STATE_TRANSITION(stateSettingsClock));
                }
                case 'L' : {

                    return (ES_STATE_TRANSITION(stateSettingsLcdCalib));
                }
            }
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                EXPORT_INSERT_REFRESH_);

            return (ES_STATE_HANDLED());
        }
    }
    return (ES_STATE_IGNORED());
}

static esAction stateSettingsClock(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY: {
            wspace->state.settingsClock.focus = 0;
            appTimeGet(&wspace->state.settingsClock.time);
            screenSettingsClock(&wspace->state);

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
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                EXPORT_INSERT_REFRESH_);
            screenExportInsert();

            return (ES_STATE_HANDLED());
        }
        case EXPORT_INSERT_REFRESH_: {
            if (gpuGetKey() == 'B') {

                return (ES_STATE_TRANSITION(stateMain));
            } else if (isUsbDetected()) {

                return (ES_STATE_TRANSITION(stateExportMount));
            } else {
                appTimerStart(
                    &wspace->refresh,
                    ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                    EXPORT_INSERT_REFRESH_);

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
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                EXPORT_CHOOSE_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case EXPORT_CHOOSE_REFRESH_: {

            if (isUsbDetected() == false) {
                
                return (ES_STATE_TRANSITION(stateMain));
            }
            switch (gpuGetKey()) {
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
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                EXPORT_CHOOSE_REFRESH_);

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
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(2000),
                WAKEUP_TIMEOUT_);

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


/*=========================================================  INCLUDE FILES  ==*/

#include <stdlib.h>

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

#define EXPORT_MONTH                    0
#define EXPORT_DAY                      1
#define EXPORT_YEAR                     2

#define GUI_TABLE(entry)                                                        \
    entry(stateInit,                TOP)                                        \
    entry(stateWakeUpDisplay,       TOP)                                        \
    entry(stateSetupTouch,          TOP)                                        \
    entry(stateWelcome,             TOP)                                        \
    entry(stateMain,                TOP)                                        \
    entry(statePreTest,             TOP)                                        \
    entry(stateTestFirstTh,         TOP)                                        \
    entry(stateTestSecondTh,        TOP)                                        \
    entry(stateTestResults,         TOP)                                        \
    entry(stateTestResultsSaving,   TOP)                                        \
    entry(stateSettings,            TOP)                                        \
    entry(stateSettingsAbout,       TOP)                                        \
    entry(stateSettingsAuthorize,   TOP)                                        \
    entry(stateSettingsAdmin,       TOP)                                        \
    entry(stateSettingsClock,       TOP)                                        \
    entry(stateSettingsCalibLcd,    TOP)                                        \
    entry(stateSettingsCalibSens,   TOP)                                        \
    entry(stateSettingsCalibSensZ,  TOP)                                        \
    entry(stateSettingsCalibSensL,  TOP)                                        \
    entry(stateSettingsCalibSensH,  TOP)                                        \
    entry(stateExport,              TOP)                                        \
    entry(stateExportInsert,        TOP)                                        \
    entry(stateExportMount,         TOP)                                        \
    entry(stateExportChoose,        TOP)                                        \
    entry(stateExportSaving,        TOP)                                        
    

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
    SETTINGS_SENSZLH_REFRESH_,
    EXPORT_INSERT_REFRESH_,
    EXPORT_CHOOSE_REFRESH_
};

enum testState {
    TEST_NOT_EXECUTED,
    TEST_STARTED,
    TEST_FAILED,
    TEST_CANCELED,
    TEST_VALID
};

enum buttonBackPos {
    DOWN_LEFT,
    DOWN_MIDDLE,
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
                enum testState      state;
                uint32_t            rawMaxValue;
                uint32_t            rawThValue;
                uint32_t            time;
            }                   th[2];
            enum testState      state;
            const uint8_t *     notification;
            uint32_t            rawIdleVacuum;
            bool                isBackgroundEnabled;
            uint32_t            count;
        }                   test;
        struct settingsAuthorize {
            uint32_t            counter;
            uint32_t            numOfCharactes;
        }                   settingsAuthorize;
        struct exportChoose {
            uint32_t            begin[3];
            uint32_t            end[3];
            uint32_t            focus;
        }                   exportChoose;
        struct settingsClock {
            uint32_t            focus;
            struct appTime      time;
        }                   settingsClock;
        struct calibSensZHL {
            uint32_t            vacuumTarget;
            uint32_t            rawFullScale;
            uint32_t            rawVacuum;
        }                   calibSensZHL;
        struct progress {
            char *              title;
            char *              description;
            uint32_t            background;
        }                   progress;
    }                   state;
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
static esAction stateTestResultsSaving  (struct wspace *, const esEvent *);
static esAction stateSettings           (struct wspace *, const esEvent *);
static esAction stateSettingsAbout      (struct wspace *, const esEvent *);
static esAction stateSettingsAuthorize  (struct wspace *, const esEvent *);
static esAction stateSettingsAdmin      (struct wspace *, const esEvent *);
static esAction stateSettingsClock      (struct wspace *, const esEvent *);
static esAction stateSettingsCalibLcd   (struct wspace *, const esEvent *);
static esAction stateSettingsCalibSens  (struct wspace *, const esEvent *);
static esAction stateSettingsCalibSensZ (struct wspace *, const esEvent *);
static esAction stateSettingsCalibSensL (struct wspace *, const esEvent *);
static esAction stateSettingsCalibSensH (struct wspace *, const esEvent *);
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

static enum testState evaluateTests(const union state * state) {
    if ((state->test.th[0].state == TEST_VALID) && (state->test.th[1].state == TEST_VALID)) {
        
        return (TEST_VALID);
    } else if ((state->test.th[0].state == TEST_CANCELED) || (state->test.th[1].state == TEST_CANCELED)) {
        
        return (TEST_CANCELED);
    } else {

        return (TEST_FAILED);
    }
}

static void testToggleBackground(union state * state) {
    if (state->test.isBackgroundEnabled == true) {
        state->test.isBackgroundEnabled = false;
    } else {
        state->test.isBackgroundEnabled = true;
    }
}

static void constructBackground(uint32_t background) {
    if (background == 0) {
        background = CLEAR_COLOR_RGB(224, 224, 224);
    }
    Ft_Gpu_Hal_WrCmd32(&Gpu, background);
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
    Ft_Gpu_Hal_WrMem(&Gpu, RAM_G + 131072L, (const uint8_t *)ManufacturerLogo, 
        ManufacturerLogoInfo.size);                                                                 /* copy data continuously into RAM_G memory */
    gpuBegin();
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR(1,0,0));
    Ft_Gpu_Hal_WrCmd32(&Gpu, BITMAP_HANDLE(13));
    Ft_Gpu_Hal_WrCmd32(&Gpu, BITMAP_SOURCE(131072L));
    Ft_Gpu_Hal_WrCmd32(&Gpu, BITMAP_LAYOUT(ManufacturerLogoInfo.format,
        ManufacturerLogoInfo.linestride, ManufacturerLogoInfo.height));
    Ft_Gpu_Hal_WrCmd32(&Gpu, BITMAP_SIZE(NEAREST, BORDER, BORDER, ManufacturerLogoInfo.pixelsX,
        ManufacturerLogoInfo.pixelsY));
    Ft_Gpu_Hal_WrCmd32(&Gpu, BEGIN(BITMAPS));
    Ft_Gpu_Hal_WrCmd32(&Gpu, VERTEX2II(35, 10, 13, 0));
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 80,  DEF_B1_FONT_SIZE, OPT_CENTER, WELCOME_GREETING);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 120, DEF_N1_FONT_SIZE, OPT_CENTER, WELCOME_HW_VERSION
        CONFIG_HARDWARE_VERSION);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 140, DEF_N1_FONT_SIZE, OPT_CENTER, WELCOME_SW_VERSION
        CONFIG_SOFTWARE_VERSION);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 160, DEF_N1_FONT_SIZE, OPT_CENTER, BUILD_DATE);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 180, DEF_N1_FONT_SIZE, OPT_CENTER, BUILD_TIME);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 220, DEF_N1_FONT_SIZE, OPT_CENTER, DEF_WEBSITE);
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}

static void screenProgress(const union state * state) {
    gpuBegin();
    constructBackground(state->progress.background);
    constructTitle(state->progress.title);
    Ft_Gpu_CoCmd_Text(&Gpu, 160,  200, DEF_N1_FONT_SIZE, OPT_CENTER, state->progress.description);
    Ft_Gpu_CoCmd_Spinner(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, 0, 0);
    gpuEnd();
}

static void screenMain(const union state * state) {
    char *              text;

    gpuBegin();
    constructBackground(0);
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
        Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('t'));
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

static void screenPreTest(const union state * state) {
    (void)state;

    gpuBegin();
    constructBackground(0);
    constructTitle("Preparing tests");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_4,   POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY,
        "Please wait");
    gpuEnd();
}

static void screenTestTh0(const union state * state) {
    gpuBegin();
    constructBackground(0);
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
    constructBackground(0);
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

static void screenTestResults(const union state * state) {
    uint32_t            colorBackground;
    const char *        text;

    if (state->test.state == TEST_VALID) {
        colorBackground = CLEAR_COLOR_RGB(16, 224, 16);
        text            = "Test succeessful";
    } else if (state->test.state == TEST_CANCELED) {
        colorBackground = CLEAR_COLOR_RGB(224, 224, 16);
        text            = "Test cancelled";
    } else {
        colorBackground = CLEAR_COLOR_RGB(224, 16, 16);
        text            = "Test failed";
    }
    gpuBegin();

    if (state->test.isBackgroundEnabled) {
        constructBackground(colorBackground);
    } else {
        constructBackground(0);
    }
    constructTitle(text);
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_4,   POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY,
        "First threshold");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_18,  POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY,
        "[" DEF_VACUUM_UNIT "]:");
    Ft_Gpu_CoCmd_Number(&Gpu, POS_COLUMN_25,  POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY,
        state->test.th[0].rawMaxValue);

    if (state->test.th[0].state == TEST_VALID) {
        text = "PASSED";
    } else if (state->test.th[0].state == TEST_CANCELED) {
        text = "CANCELED";
    } else {
        text = "FAILED";
    }
    Ft_Gpu_CoCmd_Text(&Gpu, POS_COLUMN_HALF,  POS_ROW_1_5, DEF_N1_FONT_SIZE, OPT_CENTER, text);
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_4,   POS_ROW_2, DEF_N1_FONT_SIZE, OPT_CENTERY,
        "Second threshold");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_18,  POS_ROW_2, DEF_N1_FONT_SIZE, OPT_CENTERY,
        "[" DEF_VACUUM_UNIT "]:");
    Ft_Gpu_CoCmd_Number(&Gpu, POS_COLUMN_25,  POS_ROW_2, DEF_N1_FONT_SIZE, OPT_CENTERY,
        state->test.th[1].rawMaxValue);

    if (state->test.th[1].state == TEST_VALID) {
        text = "PASSED";
    } else if (state->test.th[1].state == TEST_CANCELED) {
        text = "CANCELED";
    } else {
        text = "FAILED";
    }
    Ft_Gpu_CoCmd_Text(&Gpu, POS_COLUMN_HALF,  POS_ROW_2_5, DEF_N1_FONT_SIZE, OPT_CENTER, text);
    constructButtonBack(DOWN_LEFT);
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('R'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 180, 130, 40, DEF_N1_FONT_SIZE, 0, "Repeat");
    gpuEnd();
}

static void screenTestSaving(void) {
    static uint32_t record = 12;
    gpuBegin();
    constructBackground(0);
    constructTitle("Saving...");
    Ft_Gpu_CoCmd_Text(&Gpu, 160,  200, DEF_N1_FONT_SIZE, OPT_CENTER, "Record number:");
    Ft_Gpu_CoCmd_Number(&Gpu, 240,  200, DEF_N1_FONT_SIZE, OPT_CENTER, record++);
    Ft_Gpu_CoCmd_Spinner(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, 0, 0);
    gpuEnd();
}

static void screenExportInsert(void) {
    gpuBegin();
    constructBackground(0);
    constructTitle("Export");
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, DEF_N1_FONT_SIZE, OPT_CENTER,
        "Please insert USB flash drive");
    constructButtonBack(DOWN_MIDDLE);
    gpuEnd();
}

static void screenExportMount(void) {
    gpuBegin();
    constructBackground(0);
    constructTitle("Export");
    Ft_Gpu_CoCmd_Spinner(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, 0, 0);
    gpuEnd();
}

static void screenExportSaving(void) {
    gpuBegin();
    constructBackground(0);
    constructTitle("Saving data...");
    Ft_Gpu_CoCmd_Spinner(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, 0, 0);
    gpuEnd();
}

static void screenExportChoose(const union state * state) {
    gpuBegin();
    constructBackground(0);
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

    if (state->exportChoose.focus == 0) {
        Ft_Gpu_CoCmd_Number(&Gpu, 100, 80, DEF_N2_FONT_SIZE, OPT_CENTER,
            state->exportChoose.begin[EXPORT_MONTH]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 100, 80, DEF_N1_FONT_SIZE, OPT_CENTER,
            state->exportChoose.begin[EXPORT_MONTH]);
    }

    if (state->exportChoose.focus == 1) {
        Ft_Gpu_CoCmd_Number(&Gpu, 150, 80, DEF_N2_FONT_SIZE, OPT_CENTER,
            state->exportChoose.begin[EXPORT_DAY]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 150, 80, DEF_N1_FONT_SIZE, OPT_CENTER,
            state->exportChoose.begin[EXPORT_DAY]);
    }

    if (state->exportChoose.focus == 2) {
        Ft_Gpu_CoCmd_Number(&Gpu, 210, 80, DEF_N2_FONT_SIZE, OPT_CENTER,
            state->exportChoose.begin[EXPORT_YEAR]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 210, 80, DEF_N1_FONT_SIZE, OPT_CENTER,
            state->exportChoose.begin[EXPORT_YEAR]);
    }
    Ft_Gpu_CoCmd_Text(&Gpu, 125,  80,  DEF_N1_FONT_SIZE, OPT_CENTER, "-");
    Ft_Gpu_CoCmd_Text(&Gpu, 175,  80,  DEF_N1_FONT_SIZE, OPT_CENTER, "-");

    if (state->exportChoose.focus == 3) {
        Ft_Gpu_CoCmd_Number(&Gpu, 100, 140, DEF_N2_FONT_SIZE, OPT_CENTER,
            state->exportChoose.end[EXPORT_MONTH]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 100, 140, DEF_N1_FONT_SIZE, OPT_CENTER,
            state->exportChoose.end[EXPORT_MONTH]);
    }

    if (state->exportChoose.focus == 4) {
        Ft_Gpu_CoCmd_Number(&Gpu, 150, 140, DEF_N2_FONT_SIZE, OPT_CENTER,
            state->exportChoose.end[EXPORT_DAY]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 150, 140, DEF_N1_FONT_SIZE, OPT_CENTER,
            state->exportChoose.end[EXPORT_DAY]);
    }

    if (state->exportChoose.focus == 5) {
        Ft_Gpu_CoCmd_Number(&Gpu, 210, 140, DEF_N2_FONT_SIZE, OPT_CENTER,
            state->exportChoose.end[EXPORT_YEAR]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 210, 140, DEF_N1_FONT_SIZE, OPT_CENTER,
            state->exportChoose.end[EXPORT_YEAR]);
    }
    Ft_Gpu_CoCmd_Text(&Gpu, 125,  140,  DEF_N1_FONT_SIZE, OPT_CENTER, "-");
    Ft_Gpu_CoCmd_Text(&Gpu, 175,  140,  DEF_N1_FONT_SIZE, OPT_CENTER, "-");
    gpuEnd();
}

static void screenSettings(void) {
    gpuBegin();
    constructBackground(0);
    constructTitle("Settings");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('A'));
    Ft_Gpu_CoCmd_Button(&Gpu, 20, 60, 130, 40, DEF_N1_FONT_SIZE, 0, "About");
    Ft_Gpu_CoCmd_FgColor(&Gpu, COLOR_RGB(128, 48, 12));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('U'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 60, 130, 40, DEF_N1_FONT_SIZE, 0, "Administration");
    constructButtonBack(DOWN_MIDDLE);
    gpuEnd();
}

static void screenSettingsAdmin(void) {
    gpuBegin();
    constructBackground(0);
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
    constructBackground(0);
    constructTitle("Enter password");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_CoCmd_Keys(&Gpu,20, 80, 280, 40, DEF_N1_FONT_SIZE, 0, "12345");
    Ft_Gpu_CoCmd_Keys(&Gpu,20, 122, 280, 40, DEF_N1_FONT_SIZE, 0, "67890");
    constructButtonBack(DOWN_MIDDLE);
    gpuEnd();
}

static void screenSettingsAbout(void) {
    gpuBegin();
    constructBackground(0);
    constructTitle("About");
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 120, DEF_N1_FONT_SIZE, OPT_CENTER, WELCOME_HW_VERSION
        CONFIG_HARDWARE_VERSION);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 140, DEF_N1_FONT_SIZE, OPT_CENTER, WELCOME_SW_VERSION
        CONFIG_SOFTWARE_VERSION);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 160, DEF_N1_FONT_SIZE, OPT_CENTER, DEF_WEBSITE);
    constructButtonBack(DOWN_MIDDLE);
    gpuEnd();
}

static void screenSettingsClock(const union state * state) {
    uint32_t            textSize[7];
    char                buffer[10];

    gpuBegin();
    constructBackground(0);
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

static void screenSettingsCalibLcd(void) {
    gpuBegin();
    constructBackground(0);
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 80, DEF_B1_FONT_SIZE, OPT_CENTER, "Touch Calibration");
    Ft_Gpu_CoCmd_Text(&Gpu,DISP_WIDTH / 2 ,DISP_HEIGHT/2,26,OPT_CENTERX|OPT_CENTERY,
        "Please tap on the dot");
    Ft_Gpu_CoCmd_Calibrate(&Gpu, 0);
    gpuEnd();
}

static void screenSettingsCalibSensor(void) {
    gpuBegin();
    constructBackground(0);
    constructTitle("Calibrate Sensor");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('Z'));
    Ft_Gpu_CoCmd_Button(&Gpu, 20,  60, 130, 40, DEF_N1_FONT_SIZE, 0, "0 " DEF_VACUUM_UNIT);
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('L'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 60, 130, 40, DEF_N1_FONT_SIZE, 0, "5 " DEF_VACUUM_UNIT);
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('H'));
    Ft_Gpu_CoCmd_Button(&Gpu, 20,  120, 130, 40, DEF_N1_FONT_SIZE, 0, "10 " DEF_VACUUM_UNIT);
    constructButtonBack(DOWN_LEFT);
    gpuEnd();
}

static void screenSettingsCalibSensorZLH(const union state * state) {
    gpuBegin();
    constructBackground(0);
    constructTitle("Calibrate Sensor");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_4,   POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY,
        "Apply vacuum");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_18,  POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY,
        "[" DEF_VACUUM_UNIT "]:");
    Ft_Gpu_CoCmd_Number(&Gpu, POS_COLUMN_25,  POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY,
        state->calibSensZHL.vacuumTarget);
    Ft_Gpu_CoCmd_Number(&Gpu, DISP_WIDTH / 2,  POS_ROW_2, DEF_N2_FONT_SIZE, OPT_CENTER,
        state->calibSensZHL.rawVacuum);
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_CoCmd_Progress(&Gpu, POS_COLUMN_4, POS_ROW_1_5 - 5, DISP_WIDTH - (POS_COLUMN_4 * 2), 10,
        0, state->calibSensZHL.rawVacuum, state->calibSensZHL.rawFullScale);
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('S'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 180, 130, 40, DEF_N1_FONT_SIZE, 0, "Save");
    constructButtonBack(DOWN_LEFT);
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

            if ((touchStatusEvent->status == TOUCH_INITIALIZED) ||
                (touchStatusEvent->status == TOUCH_CALIBRATED)) {

                return (ES_STATE_TRANSITION(stateWelcome));
            } else if (touchStatusEvent->status == TOUCH_NOT_INITIALIZED) {
                esEvent * request;
                esError   error;
                gpuFadeIn();
                screenSettingsCalibLcd();
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
            esEvent *   request;
            esError     error;

            ES_ENSURE(error = esEventCreate(sizeof(*request), EVT_TOUCH_ENABLE, &request));

            if (error == ES_ERROR_NONE) {
                ES_ENSURE(esEpaSendEvent(Touch, request));
            }
            
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

            appTimeGet(&time);
            snprintRtcTime(&time, wspace->state.main.time);
            snprintRtcDate(&time, wspace->state.main.date);
            snprintBatteryStatus(wspace->state.main.battery);
            wspace->state.main.isDutInPlace = isDutDetected();
            screenMain(&wspace->state);
#if 1
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                MAIN_REFRESH_);
#endif

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT: {
            appTimerCancel(&wspace->refresh);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {

            switch (((const struct touchEvent *)event)->tag) {
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
#if 1
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                MAIN_REFRESH_);
#endif
            
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
                wspace->state.test.th[0].state       = TEST_NOT_EXECUTED;
                wspace->state.test.th[0].rawMaxValue = 0u;
                wspace->state.test.th[0].rawThValue  = configGetFirstThRawVacuum();
                wspace->state.test.th[0].time        = configGetFirstThTimeout();
                wspace->state.test.th[1].state       = TEST_NOT_EXECUTED;
                wspace->state.test.th[1].rawMaxValue = 0u;
                wspace->state.test.th[1].rawThValue  = configGetSecondThRawVacuum();
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
            wspace->state.test.th[0].state = TEST_STARTED;
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(wspace->state.test.th[0].time),
                FIRST_TH_TIMEOUT_);
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS),
                FIRST_TH_REFRESH_);
            screenTestTh0(&wspace->state);
            motorEnable();

            return (ES_STATE_HANDLED());
        }
        case FIRST_TH_REFRESH_: {
            
            if (isDutDetected()) {
                uint32_t rawValue;

                rawValue = getDutRawValue();
                
                if (wspace->state.test.rawIdleVacuum > rawValue) {
                    uint32_t rawVacuum;

                    rawVacuum = wspace->state.test.rawIdleVacuum - rawValue;

                    if (wspace->state.test.th[0].rawMaxValue < rawVacuum) {
                        wspace->state.test.th[0].rawMaxValue = rawVacuum;
                    }

                    if (rawVacuum >= wspace->state.test.th[0].rawThValue) {
                        wspace->state.test.th[0].state = TEST_VALID;
                        wspace->state.test.th[0].time -= appTimerGetRemaining(&wspace->timeout);
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
                wspace->state.test.th[0].state = TEST_CANCELED;

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
            wspace->state.test.th[1].state = TEST_STARTED;
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(configGetSecondThTimeout()),
                SECOND_TH_TIMEOUT_);
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS),
                SECOND_TH_REFRESH_);
            screenTestTh1(&wspace->state);
            motorEnable();

            return (ES_STATE_HANDLED());
        }
        case SECOND_TH_REFRESH_: {

            if (isDutDetected()) {
                uint32_t rawValue;

                rawValue = getDutRawValue();

                if (wspace->state.test.rawIdleVacuum > rawValue) {
                    uint32_t rawVacuum;

                    rawVacuum = wspace->state.test.rawIdleVacuum - rawValue;

                    if (wspace->state.test.th[1].rawMaxValue < rawVacuum) {
                        wspace->state.test.th[1].rawMaxValue = rawVacuum;
                    }

                    if (rawVacuum >= wspace->state.test.th[1].rawThValue) {
                        wspace->state.test.th[1].state = TEST_VALID;
                        wspace->state.test.th[1].time -= appTimerGetRemaining(&wspace->timeout);

                        return (ES_STATE_TRANSITION(stateTestResults));
                    }
                     screenTestTh1(&wspace->state);
                }
                appTimerStart(
                    &wspace->refresh,
                    ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS),
                    SECOND_TH_REFRESH_);
            } else {
                motorDisable();
                wspace->state.test.th[1].state = TEST_CANCELED;

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
        case ES_ENTRY : {
            wspace->state.test.isBackgroundEnabled = false;
            wspace->state.test.state = evaluateTests(&wspace->state);

            switch (wspace->state.test.state) {
                case TEST_VALID : {
                    wspace->state.test.notification = SuccessNotification;
                    break;
                }
                case TEST_CANCELED : {
                    wspace->state.test.notification = ConfusedNotification;
                    break;
                }
                default : {
                    wspace->state.test.notification = FailNotification;
                }
            }
            buzzerMelody(wspace->state.test.notification);
            screenTestResults(&wspace->state);
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(*wspace->state.test.notification),
                TEST_RESULTS_NOTIFY_TIMEOUT_);

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT: {
            appTimerCancel(&wspace->timeout);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            
            switch (((const struct touchEvent *)event)->tag) {
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateTestResultsSaving));
                }
                case 'R' : {

                    return (ES_STATE_TRANSITION(statePreTest));
                }
                default: {
                    break;
                }
            }

            return (ES_STATE_HANDLED());
        }
        case TEST_RESULTS_NOTIFY_TIMEOUT_: {
            screenTestResults(&wspace->state);
            testToggleBackground(&wspace->state);
            wspace->state.test.notification++;

            if (*wspace->state.test.notification != 0) {
                appTimerStart(
                    &wspace->timeout,
                    ES_VTMR_TIME_TO_TICK_MS(*wspace->state.test.notification),
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
            screenSettings();

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {

            switch (((const struct touchEvent *)event)->tag) {
                case 'A' : {

                    return (ES_STATE_TRANSITION(stateSettingsAbout));
                }
                case 'U' : {

                    return (ES_STATE_TRANSITION(stateSettingsAuthorize));
                }
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateMain));
                }
                default : {
                    break;
                }
            }
            
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

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {

            if (((const struct touchEvent *)event)->tag == 'B') {

                return (ES_STATE_TRANSITION(stateSettings));
            } else {

                return (ES_STATE_HANDLED());
            }
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
            wspace->state.settingsAuthorize.numOfCharactes = configPasswordLength();
            wspace->state.settingsAuthorize.counter        = 0u;

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG :{
            uint8_t     key;

            switch (key = ((const struct touchEvent *)event)->tag) {
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

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSettingsAdmin(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY: {
            screenSettingsAdmin();

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {

            switch (((const struct touchEvent *)event)->tag) {
                case 'S' : {

                    return (ES_STATE_TRANSITION(stateSettingsCalibSens));
                }
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateSettings));
                }
                case 'R' : {

                    return (ES_STATE_TRANSITION(stateSettingsClock));
                }
                case 'L' : {

                    return (ES_STATE_TRANSITION(stateSettingsCalibLcd));
                }
                default : {
                    break;
                }
            }

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSettingsCalibLcd(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY: {
            esEvent * request;
            esError   error;

            screenSettingsCalibLcd();
            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_TOUCH_CALIBRATE, &request));

            if (error == ES_ERROR_NONE) {
                ES_ENSURE(esEpaSendEvent(Touch, request));
            }
            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_TOUCH_ENABLE, &request));

            if (error == ES_ERROR_NONE) {
                ES_ENSURE(esEpaSendEvent(Touch, request));
            }

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

static esAction stateSettingsCalibSens(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY : {
            screenSettingsCalibSensor();

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {

            switch (((const struct touchEvent *)event)->tag) {
                case 'Z' : {

                    return (ES_STATE_TRANSITION(stateSettingsCalibSensZ));
                }
                case 'L' : {

                    return (ES_STATE_TRANSITION(stateSettingsCalibSensL));
                }
                case 'H' : {

                    return (ES_STATE_TRANSITION(stateSettingsCalibSensH));
                }
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateSettingsAdmin));
                }
                default : {

                    return (ES_STATE_HANDLED());
                }
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSettingsCalibSensZ(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY : {
            wspace->state.calibSensZHL.vacuumTarget = 0;
            wspace->state.calibSensZHL.rawFullScale = 1024;
            wspace->state.calibSensZHL.rawVacuum    = getDutRawValue();
            screenSettingsCalibSensorZLH(&wspace->state);
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                SETTINGS_SENSZLH_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT : {
            appTimerCancel(&wspace->refresh);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            switch (((const struct touchEvent *)event)->tag) {
                case 'S' : {

                    if (configSetRawIdleVacuum(wspace->state.calibSensZHL.rawVacuum) != true) {
                        /*
                         * TODO: SAVE FAILED
                         */
                    }

                    return (ES_STATE_TRANSITION(stateSettingsCalibSens));
                }
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateSettingsCalibSens));
                }
                default : {

                    return (ES_STATE_HANDLED());
                }
            }
        }
        case SETTINGS_SENSZLH_REFRESH_ : {
            wspace->state.calibSensZHL.rawVacuum = getDutRawValue();
            screenSettingsCalibSensorZLH(&wspace->state);
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                SETTINGS_SENSZLH_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSettingsCalibSensL(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY : {
            wspace->state.calibSensZHL.vacuumTarget = 5;
            wspace->state.calibSensZHL.rawFullScale = configGetRawIdleVacuum();
            wspace->state.calibSensZHL.rawVacuum    = min(
                getDutRawValue(),
                wspace->state.calibSensZHL.rawFullScale);
            screenSettingsCalibSensorZLH(&wspace->state);
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                SETTINGS_SENSZLH_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT : {
            appTimerCancel(&wspace->refresh);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            switch (((const struct touchEvent *)event)->tag) {
                case 'S' : {

                    if (configSetFirstThRawVacuum(wspace->state.calibSensZHL.rawVacuum) != true) {
                        /*
                         * TODO: SAVE FAILED
                         */
                    }

                    return (ES_STATE_TRANSITION(stateSettingsCalibSens));
                }
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateSettingsCalibSens));
                }
                default : {

                    return (ES_STATE_HANDLED());
                }
            }
        }
        case SETTINGS_SENSZLH_REFRESH_ : {
            wspace->state.calibSensZHL.rawVacuum = getDutRawValue();
            screenSettingsCalibSensorZLH(&wspace->state);
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                SETTINGS_SENSZLH_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSettingsCalibSensH(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY : {
            wspace->state.calibSensZHL.vacuumTarget = 10;
            wspace->state.calibSensZHL.rawFullScale = configGetRawIdleVacuum();
            wspace->state.calibSensZHL.rawVacuum    = min(
                getDutRawValue(),
                wspace->state.calibSensZHL.rawFullScale);
            screenSettingsCalibSensorZLH(&wspace->state);
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                SETTINGS_SENSZLH_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT : {
            appTimerCancel(&wspace->refresh);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            switch (((const struct touchEvent *)event)->tag) {
                case 'S' : {

                    if (configSetSecondThRawVacuum(wspace->state.calibSensZHL.rawVacuum) != true) {
                        /*
                         * TODO: SAVE FAILED
                         */
                    }

                    return (ES_STATE_TRANSITION(stateSettingsCalibSens));
                }
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateSettingsCalibSens));
                }
                default : {

                    return (ES_STATE_HANDLED());
                }
            }
        }
        case SETTINGS_SENSZLH_REFRESH_ : {
            wspace->state.calibSensZHL.rawVacuum = getDutRawValue();
            screenSettingsCalibSensorZLH(&wspace->state);
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                SETTINGS_SENSZLH_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSettingsClock(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY: {
            wspace->state.settingsClock.focus = 0;
            appTimeGet(&wspace->state.settingsClock.time);
            screenSettingsClock(&wspace->state);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
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

            if (isUsbDetected()) {

                return (ES_STATE_TRANSITION(stateExportMount));
            } else {
                appTimerStart(
                    &wspace->refresh,
                    ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                    EXPORT_INSERT_REFRESH_);

                return (ES_STATE_HANDLED());
            }
        }
        case EVT_TOUCH_TAG : {
            
            if (((const struct touchEvent *)event)->tag == 'B') {

                return (ES_STATE_TRANSITION(stateMain));
            } else {

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
            struct appTime currentTime;

            appTimeGet(&currentTime);
            wspace->state.exportChoose.begin[EXPORT_DAY]   = 20;
            wspace->state.exportChoose.begin[EXPORT_MONTH] = 4;
            wspace->state.exportChoose.begin[EXPORT_YEAR]  = 2014;
            wspace->state.exportChoose.end[EXPORT_DAY]     = currentTime.day;
            wspace->state.exportChoose.end[EXPORT_MONTH]   = currentTime.month;
            wspace->state.exportChoose.end[EXPORT_YEAR]    = currentTime.year;
            wspace->state.exportChoose.focus               = 0;
            screenExportChoose(&wspace->state);
            appTimerStart(&wspace->refresh, ES_VTMR_TIME_TO_TICK_MS(100), EXPORT_CHOOSE_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case EXPORT_CHOOSE_REFRESH_: {

            if (isUsbDetected() == false) {
                
                return (ES_STATE_TRANSITION(stateMain));
            } else {
                
                return (ES_STATE_HANDLED());
            }
        }
        case EVT_TOUCH_TAG : {

            switch (((const struct touchEvent *)event)->tag) {
                case 'B': {

                    return (ES_STATE_TRANSITION(stateMain));
                }
                case '>' : {
                    if (wspace->state.exportChoose.focus != 5u) {
                        wspace->state.exportChoose.focus++;
                    }
                    break;
                }
                case '<' : {
                    if (wspace->state.exportChoose.focus != 0u) {
                        wspace->state.exportChoose.focus--;
                    }
                    break;
                }
                case '+' : {
                    if (wspace->state.exportChoose.focus < 3) {
                        wspace->state.exportChoose.begin[wspace->state.exportChoose.focus]++;
                    } else {
                        wspace->state.exportChoose.end[wspace->state.exportChoose.focus - 3]++;
                    }
                    break;
                }
                case '-' : {
                    if (wspace->state.exportChoose.focus < 3) {
                        wspace->state.exportChoose.begin[wspace->state.exportChoose.focus]--;
                    } else {
                        wspace->state.exportChoose.end[wspace->state.exportChoose.focus - 3]--;
                    }
                    break;
                }
                case 'E' : {

                    return (ES_STATE_TRANSITION(stateExportSaving));
                }
                default: {
                    break;
                }
            }
            screenExportChoose(&wspace->state);

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

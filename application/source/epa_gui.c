
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
#include "app_user.h"
#include "app_data_log.h"
#include "app_pdetector.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CONFIG_ZERO_CALIB_MS            2000
#define CONFIG_TEST_CANCEL_MS           5000
#define CONFIG_TEST_FAIL_MS             5000
#define CONFIG_TEST_OVERVIEW_MS         5000
#define CONFIG_TEST_REFRESH_MS          10
#define CONFIG_TOUCH_REFRESH_MS         20
#define CONFIG_MAIN_REFRESH_MS          1000

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
#define POS_COLUMN_2                    20
#define POS_COLUMN_4                    40
#define POS_COLUMN_13                   130
#define POS_COLUMN_18                   180
#define POS_COLUMN_25                   250
#define POS_COLUMN_26                   260
#define POS_COLUMN_HALF                 (DISP_WIDTH / 2)

#define POS_TITLE_V                     30
#define POS_TITLE_H                     (DISP_WIDTH / 2)

#define B_IS_ACTIVE                     true
#define B_IS_INACTIVE                   false

#define SCREEN_TEST_EN_FIRST_INFO       (0x1u << 0)
#define SCREEN_TEST_EN_SECOND_INFO      (0x1u << 1)
#define SCREEN_TEST_EN_FIRST_PROGRESS   (0x1u << 2)
#define SCREEN_TEST_EN_SECOND_PROGRESS  (0x1u << 3)
#define SCREEN_TEST_EN_FIRST_STATUS     (0x1u << 4)
#define SCREEN_TEST_EN_SECOND_STATUS    (0x1u << 5)
#define SCREEN_TEST_BACK_BUTTON         (0x1u << 6)
#define SCREEN_TEST_RETRY_BUTTON        (0x1u << 7)

#define CLOCK_YEAR                      6
#define CLOCK_MONTH                     4
#define CLOCK_DAY                       5
#define CLOCK_AMPM                      3
#define CLOCK_SECOND                    2
#define CLOCK_MINUTE                    1
#define CLOCK_HOUR                      0

#define EXPORT_MONTH                    0
#define EXPORT_DAY                      1
#define EXPORT_YEAR                     2

#define GUI_TABLE(entry)                                                        \
    entry(stateInit,                TOP)                                        \
    entry(stateWakeUpDisplay,       TOP)                                        \
    entry(stateSetupTouch,          TOP)                                        \
    entry(stateWelcome,             TOP)                                        \
    entry(stateZeroCalib,           TOP)                                        \
    entry(stateMain,                TOP)                                        \
    entry(stateProgress,            TOP)                                        \
    entry(stateRemoveCurrent,       TOP)                                        \
    entry(stateTest,                TOP)                                        \
    entry(stateTestInProgress,      stateTest)                                  \
    entry(stateTestFirstTh,         stateTestInProgress)                        \
    entry(stateTestSecondTh,        stateTestInProgress)                        \
    entry(stateTestResultStatic,    stateTest)                                  \
    entry(stateTestResultReleased,  stateTest)                                  \
    entry(stateTestResultReady,     stateTest)                                  \
    entry(stateSettings,            TOP)                                        \
    entry(stateSettingsAbout,       TOP)                                        \
    entry(stateSettingsAuthorize,   TOP)                                        \
    entry(stateSettingsAdmin,       TOP)                                        \
    entry(stateSettingsClock,       TOP)                                        \
    entry(stateSettingsCalibLcd,    TOP)                                        \
    entry(stateSettingsCalibSens,   TOP)                                        \
    entry(stateSettingsCalibSensL,  TOP)                                        \
    entry(stateSettingsCalibSensH,  TOP)                                        \
    entry(stateExport,              TOP)                                        \
    entry(stateExportNoData,        TOP)                                        \
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
    ZERO_CALIB_WAIT_,
    FIRST_TH_TIMEOUT_,
    FIRST_TH_REFRESH_,
    SECOND_TH_TIMEOUT_,
    SECOND_TH_REFRESH_,
    SETTINGS_SENSZLH_REFRESH_,
    EXPORT_INSERT_REFRESH_,
    EXPORT_CHOOSE_REFRESH_,
    PROGRESS_TIMEOUT_
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
    uint32_t            rawIdleVacuum;
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
            const uint8_t *     notification;
            uint32_t            rawIdleVacuum;
            uint32_t            count;
            struct testResults {
                const char *        title;
                const char *        button;
                bool                is_rbutton_active;
                bool                is_bbutton_active;
                uint32_t            rawMax0Value;
                const char *        state0;
                uint32_t            rawMax1Value;
                const char *        state1;
                uint32_t            background;
            }                   testResults;
        }                   test;
        struct testReport {
            uint32_t                nEntries;
        }                   testReport;
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
            esAction            nextState;
            uint32_t            timeout;
        }                   progress;
    }                   state;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void screenWelcome(void);
static void screenMain(const union state * state);
static void screenExportInsert(void);
static void screenSettings(void);

static esAction stateInit               (void *, const esEvent *);
static esAction stateWakeUpDisplay      (void *, const esEvent *);
static esAction stateSetupTouch         (void *, const esEvent *);
static esAction stateWelcome            (void *, const esEvent *);
static esAction stateZeroCalib          (void *, const esEvent *);
static esAction stateMain               (void *, const esEvent *);
static esAction stateProgress           (void *, const esEvent *);
static esAction stateRemoveCurrent      (void *, const esEvent *);
static esAction stateTest               (void *, const esEvent *);
static esAction stateTestInProgress     (void *, const esEvent *);
static esAction stateTestFirstTh        (void *, const esEvent *);
static esAction stateTestSecondTh       (void *, const esEvent *);
static esAction stateTestResultStatic   (void *, const esEvent *);
static esAction stateTestResultReleased (void *, const esEvent *);
static esAction stateTestResultReady    (void *, const esEvent *);
static esAction stateSettings           (void *, const esEvent *);
static esAction stateSettingsAbout      (void *, const esEvent *);
static esAction stateSettingsAuthorize  (void *, const esEvent *);
static esAction stateSettingsAdmin      (void *, const esEvent *);
static esAction stateSettingsClock      (void *, const esEvent *);
static esAction stateSettingsCalibLcd   (void *, const esEvent *);
static esAction stateSettingsCalibSens  (void *, const esEvent *);
static esAction stateSettingsCalibSensL (void *, const esEvent *);
static esAction stateSettingsCalibSensH (void *, const esEvent *);
static esAction stateExport             (void *, const esEvent *);
static esAction stateExportNoData       (void *, const esEvent *);
static esAction stateExportInsert       (void *, const esEvent *);
static esAction stateExportMount        (void *, const esEvent *);
static esAction stateExportChoose       (void *, const esEvent *);
static esAction stateExportSaving       (void *, const esEvent *);

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

static void constructBackground(uint32_t background) {
    if (background == 0) {
        Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_COLOR_RGB(224, 224, 224));
        Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR(1, 0, 0));
        Ft_Gpu_CoCmd_Gradient(&Gpu, 0, 0, 0x707070, 0, DISP_HEIGHT, 0xe0e0e0);
    } else {
        Ft_Gpu_Hal_WrCmd32(&Gpu, background);
        Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR(1, 0, 0));
    }
}

static void constructTitle(const char * title) {
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));
    Ft_Gpu_CoCmd_Text(&Gpu, POS_TITLE_H,  POS_TITLE_V, DEF_B1_FONT_SIZE, OPT_CENTER, title);
}

static void constructButtonBack(enum buttonBackPos position, bool active) {

    if (active) {
        Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('B'));
        Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
        Ft_Gpu_CoCmd_FgColor(&Gpu, COLOR_RGB(8, 120, 40));
    } else {
        Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('b'));
        Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(92, 92, 92));
        Ft_Gpu_CoCmd_FgColor(&Gpu, COLOR_RGB(112, 112, 112));
    }

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
    /* copy data continuously into RAM_G memory */
    Ft_Gpu_Hal_WrMem(&Gpu, RAM_G + 131072L, (const uint8_t *)ManufacturerLogo, ManufacturerLogoInfo.size);              
    gpuBegin();
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR(1,0,0));
    Ft_Gpu_Hal_WrCmd32(&Gpu, BITMAP_HANDLE(13));
    Ft_Gpu_Hal_WrCmd32(&Gpu, BITMAP_SOURCE(131072L));
    Ft_Gpu_Hal_WrCmd32(&Gpu, BITMAP_LAYOUT(ManufacturerLogoInfo.format, ManufacturerLogoInfo.linestride,
        ManufacturerLogoInfo.height));
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


static void screenTestTh0(const union state * state) {
    gpuBegin();
    constructBackground(0);
    constructTitle("Test in progress");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_2,   POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY, "1st threshold");
    Ft_Gpu_CoCmd_Spinner(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, 0, 0);
    gpuEnd();
}

static void screenTestTh1(const union state * state) {
    gpuBegin();
    constructBackground(0);
    constructTitle("Test in progress");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_2,   POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY, "2nd threshold");
    Ft_Gpu_CoCmd_Spinner(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, 0, 0);
    gpuEnd();
}

static void screenTestResults(const union state * state) {
    gpuBegin();
    constructBackground(state->test.testResults.background);
    constructTitle(state->test.testResults.title);
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_2,   POS_ROW_1,   DEF_N1_FONT_SIZE, OPT_CENTERY, "1st threshold");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_13,  POS_ROW_1,   DEF_N1_FONT_SIZE, OPT_CENTERY, "[" DEF_VACUUM_UNIT "]:");
    Ft_Gpu_CoCmd_Number(&Gpu, POS_COLUMN_18,  POS_ROW_1,   DEF_N1_FONT_SIZE, OPT_CENTERY,
        state->test.testResults.rawMax0Value);
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_26,  POS_ROW_1,   DEF_N1_FONT_SIZE, OPT_CENTER, state->test.testResults.state0);
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_2,   POS_ROW_1_5, DEF_N1_FONT_SIZE, OPT_CENTERY, "2nd threshold");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_13,  POS_ROW_1_5, DEF_N1_FONT_SIZE, OPT_CENTERY, "[" DEF_VACUUM_UNIT "]:");
    Ft_Gpu_CoCmd_Number(&Gpu, POS_COLUMN_18,  POS_ROW_1_5, DEF_N1_FONT_SIZE, OPT_CENTERY,
        state->test.testResults.rawMax1Value);
    Ft_Gpu_CoCmd_Text(&Gpu,  POS_COLUMN_26,  POS_ROW_1_5, DEF_N1_FONT_SIZE, OPT_CENTER, state->test.testResults.state1);

    if (state->test.testResults.is_rbutton_active) {
        Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
        Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('R'));
        Ft_Gpu_CoCmd_Button(&Gpu, 170, 140, 130, 80, DEF_N1_FONT_SIZE, 0, state->test.testResults.button);
    } else {
        Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(92, 92, 92));
        Ft_Gpu_CoCmd_FgColor(&Gpu, COLOR_RGB(112, 112, 112));
        Ft_Gpu_CoCmd_Button(&Gpu, 170, 140, 130, 80, DEF_N1_FONT_SIZE, 0, state->test.testResults.button);
    }

    constructButtonBack(DOWN_LEFT, state->test.testResults.is_bbutton_active);
    gpuEnd();
}

static void screenTestSaving(const union state * state) {
    gpuBegin();
    constructBackground(0);
    constructTitle("Saving...");
    Ft_Gpu_CoCmd_Text(&Gpu, 160,  200, DEF_N1_FONT_SIZE, OPT_CENTER, "Saving record number:");
    Ft_Gpu_CoCmd_Number(&Gpu, 240,  200, DEF_N1_FONT_SIZE, OPT_CENTERY, state->testReport.nEntries);
    Ft_Gpu_CoCmd_Spinner(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, 0, 0);
    gpuEnd();
}

static void screenExportNoData(void) {
    gpuBegin();
    constructBackground(0);
    constructTitle("Export");
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, DEF_N1_FONT_SIZE, OPT_CENTER,
        "There is no data log to export");
    constructButtonBack(DOWN_MIDDLE, B_IS_ACTIVE);
    gpuEnd();
}

static void screenExportInsert(void) {
    gpuBegin();
    constructBackground(0);
    constructTitle("Export");
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, DEF_N1_FONT_SIZE, OPT_CENTER,
        "Please insert USB flash drive");
    constructButtonBack(DOWN_MIDDLE, B_IS_ACTIVE);
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
    constructButtonBack(DOWN_LEFT, B_IS_ACTIVE);
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('E'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 180, 130, 40, DEF_N1_FONT_SIZE, 0, "Export");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));

    if (state->exportChoose.focus == 0) {
        Ft_Gpu_CoCmd_Number(&Gpu, 100, 80, DEF_N2_FONT_SIZE, OPT_CENTER, state->exportChoose.begin[EXPORT_MONTH]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 100, 80, DEF_N1_FONT_SIZE, OPT_CENTER, state->exportChoose.begin[EXPORT_MONTH]);
    }

    if (state->exportChoose.focus == 1) {
        Ft_Gpu_CoCmd_Number(&Gpu, 150, 80, DEF_N2_FONT_SIZE, OPT_CENTER, state->exportChoose.begin[EXPORT_DAY]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 150, 80, DEF_N1_FONT_SIZE, OPT_CENTER, state->exportChoose.begin[EXPORT_DAY]);
    }

    if (state->exportChoose.focus == 2) {
        Ft_Gpu_CoCmd_Number(&Gpu, 210, 80, DEF_N2_FONT_SIZE, OPT_CENTER, state->exportChoose.begin[EXPORT_YEAR]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 210, 80, DEF_N1_FONT_SIZE, OPT_CENTER, state->exportChoose.begin[EXPORT_YEAR]);
    }
    Ft_Gpu_CoCmd_Text(&Gpu, 125,  80,  DEF_N1_FONT_SIZE, OPT_CENTER, "-");
    Ft_Gpu_CoCmd_Text(&Gpu, 175,  80,  DEF_N1_FONT_SIZE, OPT_CENTER, "-");

    if (state->exportChoose.focus == 3) {
        Ft_Gpu_CoCmd_Number(&Gpu, 100, 140, DEF_N2_FONT_SIZE, OPT_CENTER, state->exportChoose.end[EXPORT_MONTH]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 100, 140, DEF_N1_FONT_SIZE, OPT_CENTER, state->exportChoose.end[EXPORT_MONTH]);
    }

    if (state->exportChoose.focus == 4) {
        Ft_Gpu_CoCmd_Number(&Gpu, 150, 140, DEF_N2_FONT_SIZE, OPT_CENTER, state->exportChoose.end[EXPORT_DAY]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 150, 140, DEF_N1_FONT_SIZE, OPT_CENTER, state->exportChoose.end[EXPORT_DAY]);
    }

    if (state->exportChoose.focus == 5) {
        Ft_Gpu_CoCmd_Number(&Gpu, 210, 140, DEF_N2_FONT_SIZE, OPT_CENTER, state->exportChoose.end[EXPORT_YEAR]);
    } else {
        Ft_Gpu_CoCmd_Number(&Gpu, 210, 140, DEF_N1_FONT_SIZE, OPT_CENTER, state->exportChoose.end[EXPORT_YEAR]);
    }
    Ft_Gpu_CoCmd_Text(&Gpu, 125,  140,  DEF_N1_FONT_SIZE, OPT_CENTER, "-");
    Ft_Gpu_CoCmd_Text(&Gpu, 175,  140,  DEF_N1_FONT_SIZE, OPT_CENTER, "-");
    gpuEnd();
}

static void screenSettings(void) {
    struct appUser user;

    appUserGetCurrent(&user);
    gpuBegin();
    constructBackground(0);
    constructTitle("Settings");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('A'));
    Ft_Gpu_CoCmd_Button(&Gpu, 20, 60, 130, 40, DEF_N1_FONT_SIZE, 0, "About");
#if (CONFIG_ALLWAYS_ASK_PASSWD == 0)
    if (user.id != APPUSER_ADMINISTRATOR_ID) {
        Ft_Gpu_CoCmd_FgColor(&Gpu, COLOR_RGB(128, 48, 12));
    }
#else
    Ft_Gpu_CoCmd_FgColor(&Gpu, COLOR_RGB(128, 48, 12));
#endif
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('U'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 60, 130, 40, DEF_N1_FONT_SIZE, 0, "Administration");
    constructButtonBack(DOWN_MIDDLE, B_IS_ACTIVE);
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
    constructButtonBack(DOWN_LEFT, B_IS_ACTIVE);
    gpuEnd();
}

static void screenSettingsAuth(void) {
    gpuBegin();
    constructBackground(0);
    constructTitle("Enter password");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_CoCmd_Keys(&Gpu,20, 80, 280, 40, DEF_N1_FONT_SIZE, 0, "12345");
    Ft_Gpu_CoCmd_Keys(&Gpu,20, 122, 280, 40, DEF_N1_FONT_SIZE, 0, "67890");
    constructButtonBack(DOWN_MIDDLE, B_IS_ACTIVE);
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
    constructButtonBack(DOWN_MIDDLE, B_IS_ACTIVE);
    gpuEnd();
}

static void screenSettingsClock(const union state * state) {
    uint32_t            textSize[7];
    char                buffer[10];

    gpuBegin();
    constructBackground(0);
    constructTitle("Clock");
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
    Ft_Gpu_CoCmd_Number(&Gpu, 180, 80,  textSize[2], OPT_CENTER, state->settingsClock.time.second);
    snprintRtcDaySelector(&state->settingsClock.time, buffer);
    Ft_Gpu_CoCmd_Text(&Gpu,   230, 80,  textSize[3], OPT_CENTER, buffer);
    Ft_Gpu_CoCmd_Number(&Gpu, 100, 140, textSize[4], OPT_CENTER, state->settingsClock.time.month);
    Ft_Gpu_CoCmd_Number(&Gpu, 150, 140, textSize[5], OPT_CENTER, state->settingsClock.time.day);
    Ft_Gpu_CoCmd_Number(&Gpu, 210, 140, textSize[6], OPT_CENTER, state->settingsClock.time.year);
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
    constructButtonBack(DOWN_LEFT, B_IS_ACTIVE);
    gpuEnd();
}

static void screenSettingsCalibLcd(void) {
    gpuBegin();
    constructBackground(0);
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));
    Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, 80, DEF_B1_FONT_SIZE, OPT_CENTER, "Touch Calibration");
    Ft_Gpu_CoCmd_Text(&Gpu,DISP_WIDTH / 2 ,DISP_HEIGHT/2,26,OPT_CENTERX|OPT_CENTERY, "Please tap on the dot");
    Ft_Gpu_CoCmd_Calibrate(&Gpu, 0);
    gpuEnd();
}

static void screenSettingsCalibSensor(void) {
    gpuBegin();
    constructBackground(0);
    constructTitle("Calibrate Sensor");
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('L'));
    Ft_Gpu_CoCmd_Button(&Gpu, 20,  60, 130, 40, DEF_N1_FONT_SIZE, 0, "5 " DEF_VACUUM_UNIT);
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('H'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 60, 130, 40, DEF_N1_FONT_SIZE, 0, "10 " DEF_VACUUM_UNIT);
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('R'));
    Ft_Gpu_CoCmd_Button(&Gpu, 20,  120, 130, 40, DEF_N1_FONT_SIZE, 0, "Reset");
    constructButtonBack(DOWN_MIDDLE, B_IS_ACTIVE);
    gpuEnd();
}

static void screenSettingsCalibSensorZLH(const union state * state) {
    gpuBegin();
    constructBackground(0);
    constructTitle("Calibrate Sensor");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_4,   POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY, "Apply vacuum");
    Ft_Gpu_CoCmd_Text(&Gpu,   POS_COLUMN_18,  POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY, "[" DEF_VACUUM_UNIT "]:");
    Ft_Gpu_CoCmd_Number(&Gpu, POS_COLUMN_25,  POS_ROW_1, DEF_N1_FONT_SIZE, OPT_CENTERY,
        state->calibSensZHL.vacuumTarget);
    Ft_Gpu_CoCmd_Number(&Gpu, DISP_WIDTH / 2,  POS_ROW_2, DEF_N2_FONT_SIZE, OPT_CENTER, state->calibSensZHL.rawVacuum);
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(255, 255, 255));
    Ft_Gpu_CoCmd_Progress(&Gpu, POS_COLUMN_4, POS_ROW_1_5 - 5, DISP_WIDTH - (POS_COLUMN_4 * 2), 10, 0,
        state->calibSensZHL.rawVacuum, state->calibSensZHL.rawFullScale);
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG('S'));
    Ft_Gpu_CoCmd_Button(&Gpu, 170, 180, 130, 40, DEF_N1_FONT_SIZE, 0, "Save");
    constructButtonBack(DOWN_LEFT, B_IS_ACTIVE);
    gpuEnd();
}

/*--  End of SUPPORT  --------------------------------------------------------*/

static esAction stateInit(void * space, const esEvent * event) {
    struct wspace * wspace = space;

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

static esAction stateWakeUpDisplay(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_INIT : {

            if (isGpuReady()) {
                gpuSetupDisplay();

                return (ES_STATE_TRANSITION(stateSetupTouch));
            } else if (wspace->state.wakeUpLcd.retry != 0u) {
                wspace->state.wakeUpLcd.retry--;
                appTimerStart( &wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(10), WAKEUP_TIMEOUT_);

                return (ES_STATE_HANDLED());
            } else {

                return (ES_STATE_TRANSITION(stateInit));
            }
        }
        case WAKEUP_TIMEOUT_: {

            return (ES_STATE_TRANSITION(stateWakeUpDisplay));
        }
        default: {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetupTouch(void * space, const esEvent * event) {
    (void)space;

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

static esAction stateWelcome(void * space, const esEvent * event) {
    struct wspace * wspace = space;

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
            
            return (ES_STATE_TRANSITION(stateZeroCalib));
        }
        default: {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateZeroCalib(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY: {
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_ZERO_CALIB_MS), ZERO_CALIB_WAIT_);
            wspace->state.progress.background  = 0;
            wspace->state.progress.title       = "Zero calibration";
            wspace->state.progress.description = "Please wait...";
            screenProgress(&wspace->state);
            buzzerMelody(OkNotification);

            return (ES_STATE_HANDLED());
        }
        case ZERO_CALIB_WAIT_: {
            wspace->rawIdleVacuum = getDutRawValue();

            return (ES_STATE_TRANSITION(stateMain));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateMain(void * space, const esEvent * event) {
    struct wspace * wspace = space;
    
    switch (event->id) {
        case ES_ENTRY: {
            struct appTime time;

            appTimeGet(&time);
            snprintRtcTime(&time, wspace->state.main.time);
            snprintRtcDate(&time, wspace->state.main.date);
            snprintBatteryStatus(wspace->state.main.battery);
            wspace->state.main.isDutInPlace = false;
            screenMain(&wspace->state);
            appTimerStart(
                &wspace->refresh,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                MAIN_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT: {
            appTimerCancel(&wspace->refresh);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touchEvent = (const struct touchEvent *)event;

            switch (touchEvent->tag) {
                case 'T' : {
                    wspace->state.test.count = 0u;

                    return (ES_STATE_TRANSITION(stateTest));
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
        case EVT_PDETECT_PRESS  : {
            wspace->state.main.isDutInPlace = true;
            screenMain(&wspace->state);

            return (ES_STATE_HANDLED());
        }
        case EVT_PDETECT_RELEASE: {
            wspace->state.main.isDutInPlace = false;
            screenMain(&wspace->state);

            return (ES_STATE_HANDLED());
        }
        case MAIN_REFRESH_      : {
            struct appTime time;

            appTimeGet(&time);
            snprintRtcTime(&time, wspace->state.main.time);
            snprintRtcDate(&time, wspace->state.main.date);
            snprintBatteryStatus(wspace->state.main.battery);
            screenMain(&wspace->state);
            appTimerStart(&wspace->refresh, ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS), MAIN_REFRESH_);
            
            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateRemoveCurrent(void * space, const esEvent * event)
{
    switch (event->id) {
        case ES_ENTRY : {
            /*
             * TODO: Notify user
             */
            return (ES_STATE_HANDLED());
        }
        case EVT_PDETECT_RELEASE : {
            return (ES_STATE_TRANSITION(stateMain));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateTest(void * space, const esEvent * event)
{
    struct wspace *             wspace = space;
    
    switch (event->id) {
        case ES_ENTRY : {
            wspace->state.test.count             = 0u;
            wspace->state.test.th[0].state       = TEST_NOT_EXECUTED;
            wspace->state.test.th[0].rawMaxValue = 0u;
            wspace->state.test.th[0].rawThValue  = configGetTh0RawVacuum();
            wspace->state.test.th[0].time        = configGetTh0Timeout();
            wspace->state.test.th[1].state       = TEST_NOT_EXECUTED;
            wspace->state.test.th[1].rawMaxValue = 0u;
            wspace->state.test.th[1].rawThValue  = configGetTh1RawVacuum();
            wspace->state.test.th[1].time        = configGetTh1Timeout();
            wspace->state.test.rawIdleVacuum     = wspace->rawIdleVacuum;

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT : {
            struct appDataLog entry;

            if ((wspace->state.test.th[0].state == TEST_VALID) &&
                (wspace->state.test.th[1].state == TEST_VALID)) {
                entry.hasPassed = true;
            } else {
                entry.hasPassed = false;
            }
            entry.th[0].time        = wspace->state.test.th[0].time;
            entry.th[0].rawMaxValue = wspace->state.test.th[0].rawMaxValue;
            entry.th[1].time        = wspace->state.test.th[1].time;
            entry.th[1].rawMaxValue = wspace->state.test.th[1].rawMaxValue;
            entry.numOfTests        = wspace->state.test.count;
            appTimeGet(&entry.timestamp);
            appUserGetCurrent(&entry.user);
            appDataLogNumberOfEntries(&wspace->state.testReport.nEntries);
            screenTestSaving(&wspace->state);
            appDataLogSave(&entry);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touchEvent =
                (const struct touchEvent *)event;

            switch (touchEvent->tag) {
                case 'B' : {
                    
                    return (ES_STATE_TRANSITION(stateMain));
                }
                case 'R' : {

                    return (ES_STATE_TRANSITION(stateTest));
                }
                default: {

                    return (ES_STATE_IGNORED());
                }
            }
        }
        case ES_INIT : {

            return (ES_STATE_TRANSITION(stateTestInProgress));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateTestInProgress(void * space, const esEvent * event)
{
    struct wspace *             wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            wspace->state.test.count++;
            //motorEnable();

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT : {
            //motorDisable();
            
            return (ES_STATE_HANDLED());
        }
        case ES_INIT : {

            return (ES_STATE_TRANSITION(stateTestFirstTh));
        }
        case EVT_PDETECT_RELEASE : {

            return (ES_STATE_TRANSITION(stateTestResultReleased));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateProgress(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY: {
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(wspace->state.progress.timeout), PROGRESS_TIMEOUT_);
            screenProgress(&wspace->state);

            return (ES_STATE_HANDLED());
        }
        case PROGRESS_TIMEOUT_: {

            return (wspace->state.progress.nextState);
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateTestFirstTh(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY: {
            wspace->state.test.th[0].state = TEST_STARTED;
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(wspace->state.test.th[0].time), FIRST_TH_TIMEOUT_);
            appTimerStart(&wspace->refresh, ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS), FIRST_TH_REFRESH_);
            screenTestTh0(&wspace->state);

            return (ES_STATE_HANDLED());
        }
        case FIRST_TH_REFRESH_: {
            uint32_t rawValue;

            rawValue = getDutRawValue();

            if (wspace->rawIdleVacuum > rawValue) {
                uint32_t rawVacuum;

                rawVacuum = wspace->rawIdleVacuum - rawValue;

                if (wspace->state.test.th[0].rawMaxValue < rawVacuum) {
                    wspace->state.test.th[0].rawMaxValue = rawVacuum;
                }

                if (rawVacuum >= wspace->state.test.th[0].rawThValue) {
                    wspace->state.test.th[0].state = TEST_VALID;
                    wspace->state.test.th[0].time -= appTimerGetRemaining(&wspace->timeout);
                    /* Set the maxumum value for the second pass, too. */
                    wspace->state.test.th[1].rawMaxValue = wspace->state.test.th[0].rawMaxValue;

                    return (ES_STATE_TRANSITION(stateTestSecondTh));
                }
            }
            appTimerStart(&wspace->refresh, ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS), FIRST_TH_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case FIRST_TH_TIMEOUT_: {

            return (ES_STATE_TRANSITION(stateTestResultStatic));
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

static esAction stateTestSecondTh(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY: {
            wspace->state.test.th[1].state = TEST_STARTED;
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(wspace->state.test.th[1].time), SECOND_TH_TIMEOUT_);
            appTimerStart(&wspace->refresh, ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS), SECOND_TH_REFRESH_);
            screenTestTh1(&wspace->state);

            return (ES_STATE_HANDLED());
        }
        case SECOND_TH_REFRESH_: {
            uint32_t rawValue;

            rawValue = getDutRawValue();

            if (wspace->rawIdleVacuum > rawValue) {
                uint32_t rawVacuum;

                rawVacuum = wspace->rawIdleVacuum - rawValue;

                if (wspace->state.test.th[1].rawMaxValue < rawVacuum) {
                    wspace->state.test.th[1].rawMaxValue = rawVacuum;
                }

                if (rawVacuum >= wspace->state.test.th[1].rawThValue) {
                    wspace->state.test.th[1].state = TEST_VALID;
                    wspace->state.test.th[1].time -= appTimerGetRemaining(&wspace->timeout);

                    return (ES_STATE_TRANSITION(stateTestResultStatic));
                }
            }
            appTimerStart(&wspace->refresh, ES_VTMR_TIME_TO_TICK_MS(CONFIG_TEST_REFRESH_MS), SECOND_TH_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case SECOND_TH_TIMEOUT_: {

            return (ES_STATE_TRANSITION(stateTestResultStatic));
        }
        case ES_EXIT: {
            appTimerCancel(&wspace->refresh);
            appTimerCancel(&wspace->timeout);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateTestResultStatic(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            wspace->state.test.testResults.rawMax0Value = wspace->state.test.th[0].rawMaxValue;
            wspace->state.test.testResults.rawMax1Value = wspace->state.test.th[1].rawMaxValue;

            if (wspace->state.test.th[0].state == TEST_VALID) {
                wspace->state.test.testResults.state0 = "PASSED";
            } else {
                wspace->state.test.testResults.state0 = "FAILED";
            }

            if (wspace->state.test.th[1].state == TEST_VALID) {
                wspace->state.test.testResults.state1 = "PASSED";
            } else {
                wspace->state.test.testResults.state1 = "FAILED";
            }

            if ((wspace->state.test.th[0].state == TEST_VALID) &&
                (wspace->state.test.th[1].state == TEST_VALID)) {
                wspace->state.test.testResults.background = CLEAR_COLOR_RGB(16, 224, 16);
                wspace->state.test.testResults.title      = "Porator PASSED";
                wspace->state.test.testResults.button     = "PUT NEXT";
                wspace->state.test.testResults.is_rbutton_active = false;
                wspace->state.test.notification           = SuccessNotification;
            } else {
                if (wspace->state.test.count < configGetRetryCount()) {
                    wspace->state.test.testResults.background = CLEAR_COLOR_RGB(224, 224, 16);
                    wspace->state.test.testResults.title      = "Repeat Test";
                    wspace->state.test.testResults.button     = "REPEAT";
                    wspace->state.test.testResults.is_rbutton_active = true;
                    wspace->state.test.notification           = ConfusedNotification;
                } else {
                    wspace->state.test.testResults.background = CLEAR_COLOR_RGB(224, 16, 16);
                    wspace->state.test.testResults.title      = "Porator FAILED";
                    wspace->state.test.testResults.button     = "PUT NEXT";
                    wspace->state.test.testResults.is_rbutton_active = false;
                    wspace->state.test.notification           = FailNotification;
                }
            }
            wspace->state.test.testResults.is_bbutton_active = false;
            buzzerMelody(wspace->state.test.notification);
            screenTestResults(&wspace->state);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touchEvent =
                (const struct touchEvent *)event;

            switch (touchEvent->tag) {
                case 'R' : {

                    return (ES_STATE_TRANSITION(stateTestInProgress));
                }
                default: {
                    
                    return (ES_STATE_IGNORED());
                }
            }
        }
        case EVT_PDETECT_RELEASE : {

            return (ES_STATE_TRANSITION(stateTestResultReleased));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateTestResultReleased(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            wspace->state.test.testResults.rawMax0Value = wspace->state.test.th[0].rawMaxValue;
            wspace->state.test.testResults.rawMax1Value = wspace->state.test.th[1].rawMaxValue;
            wspace->state.test.testResults.button       = "PUT NEXT";
            wspace->state.test.testResults.is_bbutton_active = true;
            wspace->state.test.testResults.is_rbutton_active = false;

            if (wspace->state.test.th[0].state == TEST_VALID) {
                wspace->state.test.testResults.state0 = "PASSED";
            } else {
                wspace->state.test.testResults.state0 = "FAILED";
            }

            if (wspace->state.test.th[1].state == TEST_VALID) {
                wspace->state.test.testResults.state1 = "PASSED";
            } else {
                wspace->state.test.testResults.state1 = "FAILED";
            }

            if ((wspace->state.test.th[0].state == TEST_VALID) &&
                (wspace->state.test.th[1].state == TEST_VALID)) {
                wspace->state.test.testResults.background = CLEAR_COLOR_RGB(16, 224, 16);
                wspace->state.test.testResults.title      = "Porator PASSED";
            } else {
                wspace->state.test.testResults.background = CLEAR_COLOR_RGB(224, 16, 16);
                wspace->state.test.testResults.title      = "Porator FAILED";
            }
            screenTestResults(&wspace->state);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touchEvent =
                (const struct touchEvent *)event;

            switch (touchEvent->tag) {
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateMain));
                }
                default: {

                    return (ES_STATE_IGNORED());
                }
            }
        }
        case EVT_PDETECT_PRESS : {

            return (ES_STATE_TRANSITION(stateTestResultReady));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateTestResultReady(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            wspace->state.test.testResults.rawMax0Value = wspace->state.test.th[0].rawMaxValue;
            wspace->state.test.testResults.rawMax1Value = wspace->state.test.th[1].rawMaxValue;
            wspace->state.test.testResults.button       = "TEST NEXT";
            wspace->state.test.testResults.is_rbutton_active = true;
            wspace->state.test.testResults.is_bbutton_active = false;

            if (wspace->state.test.th[0].state == TEST_VALID) {
                wspace->state.test.testResults.state0 = "PASSED";
            } else {
                wspace->state.test.testResults.state0 = "FAILED";
            }

            if (wspace->state.test.th[1].state == TEST_VALID) {
                wspace->state.test.testResults.state1 = "PASSED";
            } else {
                wspace->state.test.testResults.state1 = "FAILED";
            }

            if ((wspace->state.test.th[0].state == TEST_VALID) &&
                (wspace->state.test.th[1].state == TEST_VALID)) {
                wspace->state.test.testResults.background = CLEAR_COLOR_RGB(16, 224, 16);
                wspace->state.test.testResults.title      = "Porator PASSED";
                
            } else {
                wspace->state.test.testResults.background = CLEAR_COLOR_RGB(224, 16, 16);
                wspace->state.test.testResults.title      = "Porator FAILED";
            }
            screenTestResults(&wspace->state);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {

            return (ES_STATE_IGNORED());
        }
        case EVT_PDETECT_RELEASE : {

            return (ES_STATE_TRANSITION(stateTestResultReleased));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSettings(void * space, const esEvent * event) {
    (void)space;

    switch (event->id) {
        case ES_ENTRY: {
            screenSettings();

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touchEvent = (const struct touchEvent *)event;

            switch (touchEvent->tag) {
                case 'A' : {

                    return (ES_STATE_TRANSITION(stateSettingsAbout));
                }
                case 'U' : {
#if (CONFIG_ALLWAYS_ASK_PASSWD == 0)
                    struct appUser user;

                    appUserGetCurrent(&user);

                    if (user.id == APPUSER_ADMINISTRATOR_ID) {
                        return (ES_STATE_TRANSITION(stateSettingsAdmin));
                    } else {
                        return (ES_STATE_TRANSITION(stateSettingsAuthorize));
                    }
#else
                    return (ES_STATE_TRANSITION(stateSettingsAuthorize));
#endif
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

static esAction stateSettingsAbout(void * space, const esEvent * event) {
    (void)space;

    switch (event->id) {
        case ES_ENTRY: {
            screenSettingsAbout();

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touchEvent = (const struct touchEvent *)event;

            if (touchEvent->tag == 'B') {

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

static esAction stateSettingsAuthorize(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            screenSettingsAuth();
            wspace->state.settingsAuthorize.numOfCharactes = configPasswordLength();
            wspace->state.settingsAuthorize.counter        = 0u;

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG :{
            const struct touchEvent * touchEvent = (const struct touchEvent *)event;
            uint8_t     key;

            key = touchEvent->tag;

            switch (key) {
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
                                appUserSetCurrent(APPUSER_ADMINISTRATOR_ID);

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

static esAction stateSettingsAdmin(void * space, const esEvent * event) {
    (void)space;

    switch (event->id) {
        case ES_ENTRY: {
            screenSettingsAdmin();

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touchEvent = (const struct touchEvent *)event;

            switch (touchEvent->tag) {
                case 'S' : {

                    return (ES_STATE_TRANSITION(stateSettingsCalibSens));
                }
                case 'R' : {

                    return (ES_STATE_TRANSITION(stateSettingsClock));
                }
                case 'L' : {

                    return (ES_STATE_TRANSITION(stateSettingsCalibLcd));
                }
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateSettings));
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

static esAction stateSettingsCalibLcd(void * space, const esEvent * event) {
    struct wspace * wspace = space;

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
            wspace->state.progress.title       = "Touch Calibration";
            wspace->state.progress.description = "Saving data...";
            wspace->state.progress.background  = 0u;
            wspace->state.progress.timeout     = 1000u;
            wspace->state.progress.nextState   = ES_STATE_TRANSITION(stateSettingsAdmin);

            return (ES_STATE_TRANSITION(stateProgress));
        }
        default: {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSettingsCalibSens(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            screenSettingsCalibSensor();

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touchEvent = (const struct touchEvent *)event;

            switch (touchEvent->tag) {
                case 'L' : {

                    return (ES_STATE_TRANSITION(stateSettingsCalibSensL));
                }
                case 'H' : {

                    return (ES_STATE_TRANSITION(stateSettingsCalibSensH));
                }
                case 'R' : {
                    bool        isSuccessful;

                    isSuccessful = configSetTh0RawVacuum(configGetTh0DefaultRawVacuum());

                    if (!isSuccessful) {

                        return (ES_STATE_TRANSITION(stateSettingsAdmin));
                    }
                    isSuccessful = configSetTh0Timeout(configGetTh0DefaultTimeout());

                    if (!isSuccessful) {

                        return (ES_STATE_TRANSITION(stateSettingsAdmin));
                    }
                    isSuccessful = configSetTh1RawVacuum(configGetTh1DefaultRawVacuum());

                    if (!isSuccessful) {

                        return (ES_STATE_TRANSITION(stateSettingsAdmin));
                    }
                    isSuccessful = configSetTh1Timeout(configGetTh1DefaultTimeout());

                    if (!isSuccessful) {

                        return (ES_STATE_TRANSITION(stateSettingsAdmin));
                    }
                    wspace->state.progress.title       = "Calibrate Sensor";
                    wspace->state.progress.description = "Saving data...";
                    wspace->state.progress.background  = 0u;
                    wspace->state.progress.timeout     = 1000u;
                    wspace->state.progress.nextState   = ES_STATE_TRANSITION(stateSettingsCalibSens);

                    return (ES_STATE_TRANSITION(stateProgress));
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

static esAction stateSettingsCalibSensL(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            wspace->state.calibSensZHL.vacuumTarget = 5;
            wspace->state.calibSensZHL.rawFullScale = wspace->rawIdleVacuum;
            wspace->state.calibSensZHL.rawVacuum    = min(getDutRawValue(), wspace->rawIdleVacuum);
            screenSettingsCalibSensorZLH(&wspace->state);
            appTimerStart(&wspace->refresh, ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                SETTINGS_SENSZLH_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT : {
            appTimerCancel(&wspace->refresh);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touchEvent = (const struct touchEvent *)event;

            switch (touchEvent->tag) {
                case 'S' : {
                    bool        isSaved;

                    isSaved = false;

                    if (wspace->state.calibSensZHL.rawVacuum < wspace->rawIdleVacuum) {
                        uint32_t    rawVacuum;
                    
                        rawVacuum = wspace->rawIdleVacuum - wspace->state.calibSensZHL.rawVacuum;

                        if (configSetTh0RawVacuum(rawVacuum) == true) {
                            isSaved = true;
                        }
                    }

                    if (!isSaved) {
                        /*
                         * TODO: SAVE FAILED
                         */
                    }
                    wspace->state.progress.title       = "Calibrate Sensor";
                    wspace->state.progress.description = "Saving data...";
                    wspace->state.progress.background  = 0u;
                    wspace->state.progress.timeout     = 1000u;
                    wspace->state.progress.nextState   = ES_STATE_TRANSITION(stateSettingsCalibSens);

                    return (ES_STATE_TRANSITION(stateProgress));
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
            wspace->state.calibSensZHL.rawVacuum = min(getDutRawValue(), wspace->state.calibSensZHL.rawFullScale);
            screenSettingsCalibSensorZLH(&wspace->state);
            appTimerStart(&wspace->refresh, ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                SETTINGS_SENSZLH_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSettingsCalibSensH(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            wspace->state.calibSensZHL.vacuumTarget = 10;
            wspace->state.calibSensZHL.rawFullScale = wspace->rawIdleVacuum;
            wspace->state.calibSensZHL.rawVacuum    = min(getDutRawValue(), wspace->rawIdleVacuum);
            screenSettingsCalibSensorZLH(&wspace->state);
            appTimerStart(&wspace->refresh, ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS), SETTINGS_SENSZLH_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT : {
            appTimerCancel(&wspace->refresh);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touchEvent = (const struct touchEvent *)event;

            switch (touchEvent->tag) {
                case 'S' : {
                    bool        isSaved;

                    isSaved = false;

                    if (wspace->state.calibSensZHL.rawVacuum < wspace->rawIdleVacuum) {
                        uint32_t    rawVacuum;

                        rawVacuum = wspace->rawIdleVacuum - wspace->state.calibSensZHL.rawVacuum;

                        if (rawVacuum > configGetTh0RawVacuum()) {
                            if (configSetTh1RawVacuum(rawVacuum) == true) {
                                isSaved = true;
                            }
                        }
                    }

                    if (!isSaved) {
                        /*
                         * TODO: Handle this failure
                         */
                    }
                    wspace->state.progress.title       = "Calibrate Sensor";
                    wspace->state.progress.description = "Saving data...";
                    wspace->state.progress.background  = 0u;
                    wspace->state.progress.timeout     = 1000u;
                    wspace->state.progress.nextState   = ES_STATE_TRANSITION(stateSettingsCalibSens);

                    return (ES_STATE_TRANSITION(stateProgress));
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
            wspace->state.calibSensZHL.rawVacuum = min(getDutRawValue(), wspace->state.calibSensZHL.rawFullScale);
            screenSettingsCalibSensorZLH(&wspace->state);
            appTimerStart(&wspace->refresh, ES_VTMR_TIME_TO_TICK_MS(CONFIG_MAIN_REFRESH_MS),
                SETTINGS_SENSZLH_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSettingsClock(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY: {
            wspace->state.settingsClock.focus = 0;
            appTimeGet(&wspace->state.settingsClock.time);
            screenSettingsClock(&wspace->state);

            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touchEvent = (const struct touchEvent *)event;

            switch (touchEvent->tag) {
                case '>' : {
                    if (wspace->state.settingsClock.focus == 6u) {
                        wspace->state.settingsClock.focus = 0u;
                    } else {
                        wspace->state.settingsClock.focus++;
                    }
                    break;
                }
                case '<' : {
                    if (wspace->state.settingsClock.focus == 0u) {
                        wspace->state.settingsClock.focus = 6u;
                    } else {
                        wspace->state.settingsClock.focus--;
                    }
                    break;
                }
                case '+' : {
                    switch (wspace->state.settingsClock.focus) {
                        case CLOCK_YEAR : {
                            wspace->state.settingsClock.time.year++;
                            break;
                        }
                        case CLOCK_MONTH : {
                            wspace->state.settingsClock.time.month++;
                            break;
                        }
                        case CLOCK_DAY : {
                            wspace->state.settingsClock.time.day++;
                            break;
                        }
                        case CLOCK_AMPM : {
                            if (wspace->state.settingsClock.time.daySelector == APPTIME_AM) {
                                wspace->state.settingsClock.time.daySelector = APPTIME_PM;
                            } else {
                                wspace->state.settingsClock.time.daySelector = APPTIME_AM;
                            }
                            break;
                        }
                        case CLOCK_HOUR : {
                            wspace->state.settingsClock.time.hour++;
                            break;
                        }
                        case CLOCK_MINUTE : {
                            wspace->state.settingsClock.time.minute++;
                            break;
                        }
                        case CLOCK_SECOND : {
                            wspace->state.settingsClock.time.second++;
                            break;
                        }
                    }
                    break;
                }
                case '-' : {
                    switch (wspace->state.settingsClock.focus) {
                        case CLOCK_YEAR : {
                            wspace->state.settingsClock.time.year--;
                            break;
                        }
                        case CLOCK_MONTH : {
                            wspace->state.settingsClock.time.month--;
                            break;
                        }
                        case CLOCK_DAY : {
                            wspace->state.settingsClock.time.day--;
                            break;
                        }
                        case CLOCK_AMPM : {
                            if (wspace->state.settingsClock.time.daySelector == APPTIME_AM) {
                                wspace->state.settingsClock.time.daySelector = APPTIME_PM;
                            } else {
                                wspace->state.settingsClock.time.daySelector = APPTIME_AM;
                            }
                            break;
                        }
                        case CLOCK_HOUR : {
                            wspace->state.settingsClock.time.hour--;
                            break;
                        }
                        case CLOCK_MINUTE : {
                            wspace->state.settingsClock.time.minute--;
                            break;
                        }
                        case CLOCK_SECOND : {
                            wspace->state.settingsClock.time.second--;
                            break;
                        }
                    }
                    break;
                }
                case 'S' : {
                    appTimeSet(&wspace->state.settingsClock.time);
                    wspace->state.progress.title       = "Clock";
                    wspace->state.progress.description = "Saving data...";
                    wspace->state.progress.background  = 0u;
                    wspace->state.progress.timeout     = 1000u;
                    wspace->state.progress.nextState   = ES_STATE_TRANSITION(stateSettingsAdmin);

                    return (ES_STATE_TRANSITION(stateProgress));
                }
                case 'B' : {

                    return (ES_STATE_TRANSITION(stateSettingsAdmin));
                }
                default : {
                    ;
                }
            }
            appTimeRestrict(&wspace->state.settingsClock.time);
            screenSettingsClock(&wspace->state);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateExport(void * space, const esEvent * event) {
    (void)space;

    switch (event->id) {
        case ES_ENTRY: {

            return (ES_STATE_HANDLED());
        }
        case ES_INIT: {
            uint32_t            numOfLogs;

            appDataLogNumberOfEntries(&numOfLogs);

            if (numOfLogs == 0) {

                return (ES_STATE_TRANSITION(stateExportNoData));
            } else if (isUsbDetected()) {

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

static esAction stateExportNoData(void * space, const esEvent * event) {
    (void)space;

    switch (event->id) {
        case ES_ENTRY: {
            screenExportNoData();
            
            return (ES_STATE_HANDLED());
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touchEvent = (const struct touchEvent *)event;

            if (touchEvent->tag == 'B') {

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

static esAction stateExportInsert(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY: {
            appTimerStart(&wspace->refresh, ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS), EXPORT_INSERT_REFRESH_);
            screenExportInsert();

            return (ES_STATE_HANDLED());
        }
        case EXPORT_INSERT_REFRESH_: {

            if (isUsbDetected()) {

                return (ES_STATE_TRANSITION(stateExportMount));
            } else {
                appTimerStart(&wspace->refresh, ES_VTMR_TIME_TO_TICK_MS(CONFIG_TOUCH_REFRESH_MS),
                    EXPORT_INSERT_REFRESH_);

                return (ES_STATE_HANDLED());
            }
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touchEvent = (const struct touchEvent *)event;
            
            if (touchEvent->tag == 'B') {

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

static esAction stateExportMount(void * space, const esEvent * event) {
    (void)space;

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

static esAction stateExportChoose(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY: {
            struct appDataLog           dataLog;
            uint32_t                    numOfLogs;
            uint8_t                     begin;
            uint8_t                     end;

            appDataLogNumberOfEntries(&numOfLogs);
            appDataLogLoad(numOfLogs, &dataLog);
            wspace->state.exportChoose.end[EXPORT_DAY]     = dataLog.timestamp.day;
            wspace->state.exportChoose.end[EXPORT_MONTH]   = dataLog.timestamp.month;
            wspace->state.exportChoose.end[EXPORT_YEAR]    = dataLog.timestamp.year;
            appDataLogLoad(0, &dataLog);
            wspace->state.exportChoose.begin[EXPORT_DAY]   = dataLog.timestamp.day;
            wspace->state.exportChoose.begin[EXPORT_MONTH] = dataLog.timestamp.month;
            wspace->state.exportChoose.begin[EXPORT_YEAR]  = dataLog.timestamp.year;
            wspace->state.exportChoose.focus               = 0;
            screenExportChoose(&wspace->state);
            appTimerStart(&wspace->refresh, ES_VTMR_TIME_TO_TICK_MS(100),
                    EXPORT_CHOOSE_REFRESH_);

            return (ES_STATE_HANDLED());
        }
        case EXPORT_CHOOSE_REFRESH_: {

            if (isUsbDetected() == false) {
                
                return (ES_STATE_TRANSITION(stateMain));
            } else {
                appTimerStart(&wspace->refresh, ES_VTMR_TIME_TO_TICK_MS(100),
                    EXPORT_CHOOSE_REFRESH_);
                
                return (ES_STATE_HANDLED());
            }
        }
        case EVT_TOUCH_TAG : {
            const struct touchEvent * touchEvent = (const struct touchEvent *)event;

            switch (touchEvent->tag) {
                case 'B': {

                    return (ES_STATE_TRANSITION(stateMain));
                }
                case '>' : {
                    if (wspace->state.exportChoose.focus == 5u) {
                        wspace->state.exportChoose.focus = 0u;
                    }
                    break;
                }
                case '<' : {
                    if (wspace->state.exportChoose.focus == 0u) {
                        wspace->state.exportChoose.focus = 5u;
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

static esAction stateExportSaving(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY: {
            screenExportSaving();
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(2000), WAKEUP_TIMEOUT_);

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

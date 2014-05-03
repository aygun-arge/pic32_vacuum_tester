
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
#include "app_timer.h"
#include "main.h"

#include "logo.h"
#include "app_buzzer.h"
#include "gobject.h"
#include "gtheme.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CONFIG_GOBJECT_HEAP_SIZE        4096

#define CONFIG_PRE_TEST_MS              500
#define CONFIG_TEST_CANCEL_MS           5000
#define CONFIG_TEST_FAIL_MS             5000
#define CONFIG_TEST_OVERVIEW_MS         5000
#define CONFIG_TEST_REFRESH_MS          20
#define CONFIG_MAIN_REFRESH_MS          20

#define DEF_VACUUM_UNIT                 "mmHg"

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
    entry(stateWaitGO,              TOP)                                        \
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

/*--      Type,             x,      y,      hAlign,             vAlign,             size,   color, property */
#define WELCOME(entry)                                                                                  \
    entry(GO_BACKGROUND,    0,      0,      0,                  0,                  0,      GO_COLOR_WHITE,     NULL)   \
    entry(GO_TEXT,          20,     20,     GO_H_ALIGN_CENTER,  GO_V_ALIGN_CENTER,  0,      GO_COLOR_WHITE,     NULL)   \


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
    struct appTimer     timeout;
    struct appTimer     refresh;
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
static esAction stateWaitGO             (struct wspace *, const esEvent *);
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

static const struct gObjectDescriptor WelcomePage[] = {
    WELCOME(EXPAND_PAGE_TABLE)
};

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


/*--  End of SUPPORT  --------------------------------------------------------*/

static esAction stateInit(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_INIT: {
            static esMem gObjectHeapMem = ES_MEM_INITIALIZER();                 /* STATIC: exists during execution                          */
            void *      buffer;

            wspace->retry = 10u;
            appTimerInit(&wspace->timeout);
            appTimerInit(&wspace->refresh);
            esMemAlloc(&StaticMem, CONFIG_GOBJECT_HEAP_SIZE, &buffer);          /* Allocate memory for gObject heap manager                 */
            esMemInit(
                &esGlobalHeapMemClass,
                &gObjectHeapMem,
                buffer,
                CONFIG_GOBJECT_HEAP_SIZE,
                0);                                                             /* Set-up heap memory                                       */
            initGobjectModule(&gObjectHeapMem, Gui);

            return (ES_STATE_TRANSITION(stateWelcome));
        }
        case GO_READY : {

            return (ES_STATE_TRANSITION(stateWelcome));
        }
        default: {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateWelcome(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY: {
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

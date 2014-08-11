
#include "app_config.h"
#include "app_storage.h"

#define APP_CONFIG_SIGNATURE            0xdadcbeefu

#define CONFIG_DEF_RAW_IDLE_VACUUM      270
#define CONFIG_DEF_TH0_TIMEOUT          500
#define CONFIG_DEF_TH0_RAW_VACUUM       70
#define CONFIG_DEF_TH1_TIMEOUT          1000
#define CONFIG_DEF_TH1_RAW_VACUUM       84
#define CONFIG_DEF_RETRY_COUNT          2
#define CONFIG_DEF_PASSWORD             "1248"

struct config {
    struct th {
        uint32_t        time;
        uint32_t        rawVacuum;
    }                   th[2];
    char                password[4];
};

static struct storageSpace * Storage;

const struct storageEntry ConfigStorage = {
    APP_CONFIG_SIGNATURE,
    sizeof(struct config),
    &Storage
};

static void appConfigReset(struct config * config) {
    config->th[0].time      = CONFIG_DEF_TH0_TIMEOUT;
    config->th[0].rawVacuum = CONFIG_DEF_TH0_RAW_VACUUM;
    config->th[1].time      = CONFIG_DEF_TH1_TIMEOUT;
    config->th[1].rawVacuum = CONFIG_DEF_TH1_RAW_VACUUM;
    config->password[0]     = CONFIG_DEF_PASSWORD[0];
    config->password[1]     = CONFIG_DEF_PASSWORD[1];
    config->password[2]     = CONFIG_DEF_PASSWORD[2];
    config->password[3]     = CONFIG_DEF_PASSWORD[3];
}

void initAppConfig(void) {
    struct config       config;

    if (storageRead(Storage, &config) != ES_ERROR_NONE) {
        appConfigReset(&config);
        storageWrite(Storage, &config);
    }
}

bool configSetTh0Timeout(uint32_t timeoutMs) {
    struct config       config;

    if (storageRead(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }
    config.th[0].time = timeoutMs;
    
    if (storageWrite(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }

    return (true);
SPACE_FAILURE:

    return (false);
}

bool configSetTh0RawVacuum(uint32_t rawVacuum) {
    struct config       config;

    if (storageRead(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }
    config.th[0].rawVacuum = rawVacuum;

    if (storageWrite(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }

    return (true);
SPACE_FAILURE:

    return (false);
}

bool configSetTh1Timeout(uint32_t timeoutMs) {
    struct config       config;

    if (storageRead(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }
    config.th[1].time = timeoutMs;

    if (storageWrite(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }

    return (true);
SPACE_FAILURE:

    return (false);
}

bool configSetTh1RawVacuum(uint32_t rawVacuum) {
    struct config       config;

    if (storageRead(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }
    config.th[1].rawVacuum = rawVacuum;

    if (storageWrite(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }

    return (true);
SPACE_FAILURE:

    return (false);
}

uint32_t configGetTh0Timeout(void) {
    struct config       config;

    storageRead(Storage, &config);

    return (config.th[0].time);
}

uint32_t configGetTh0DefaultTimeout(void) {

    return (CONFIG_DEF_TH0_TIMEOUT);
}

uint32_t configGetTh0RawVacuum(void) {
    struct config       config;

    storageRead(Storage, &config);

    return (config.th[0].rawVacuum);
}

uint32_t configGetTh0DefaultRawVacuum(void) {

    return (CONFIG_DEF_TH0_RAW_VACUUM);
}

uint32_t configGetTh1Timeout(void) {
    struct config       config;

    storageRead(Storage, &config);

    return (config.th[1].time);
}

uint32_t configGetTh1DefaultTimeout(void) {

    return (CONFIG_DEF_TH1_TIMEOUT);
}

uint32_t configGetTh1RawVacuum(void) {
    struct config       config;

    storageRead(Storage, &config);

    return (config.th[1].rawVacuum);
}

uint32_t configGetTh1DefaultRawVacuum(void) {

    return (CONFIG_DEF_TH1_RAW_VACUUM);
}

uint32_t configGetRetryCount(void)
{
    return (CONFIG_DEF_RETRY_COUNT);
}

bool configIsPasswordCharValid(char character, uint8_t position) {
    struct config       config;

    storageRead(Storage, &config);

    if (config.password[position] == character) {

        return (true);
    } else {

        return (false);
    }
}

uint32_t configPasswordLength(void) {

    return (sizeof(((struct config *)0)->password));
}


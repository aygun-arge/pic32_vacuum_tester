
#include "app_config.h"
#include "app_storage.h"

#define APP_CONFIG_SIGNATURE            0xdadbbeefu

#define CONFIG_DEF_RAW_IDLE_VACUUM      270
#define CONFIG_DEF_TH0_TIMEOUT          500
#define CONFIG_DEF_TH0_RAW_VACUUM       70
#define CONFIG_DEF_TH1_TIMEOUT          1000
#define CONFIG_DEF_TH1_RAW_VACUUM       84
#define CONFIG_DEF_PASSWORD             "1248"

struct config {
    uint32_t            rawIdleVacuum;
    struct test {
        uint32_t        timeoutMs;
        uint32_t        rawVacuum;
    }                   test[2];
    char                password[4];
};

static struct storageSpace * Storage;

const struct storageEntry ConfigStorage = {
    APP_CONFIG_SIGNATURE,
    sizeof(struct config),
    &Storage
};

static void appConfigReset(struct config * config) {
    config->rawIdleVacuum     = CONFIG_DEF_RAW_IDLE_VACUUM;
    config->test[0].timeoutMs = CONFIG_DEF_TH0_TIMEOUT;
    config->test[0].rawVacuum = CONFIG_DEF_TH0_RAW_VACUUM;
    config->test[1].timeoutMs = CONFIG_DEF_TH1_TIMEOUT;
    config->test[1].rawVacuum = CONFIG_DEF_TH1_RAW_VACUUM;
    config->password[0]       = CONFIG_DEF_PASSWORD[0];
    config->password[1]       = CONFIG_DEF_PASSWORD[1];
    config->password[2]       = CONFIG_DEF_PASSWORD[2];
    config->password[3]       = CONFIG_DEF_PASSWORD[3];
}

void initAppConfig(void) {
    struct config       config;

    if (storageRead(Storage, &config) != ES_ERROR_NONE) {
        appConfigReset(&config);
        storageWrite(Storage, &config);
    }
}

bool configSetRawIdleVacuum(uint32_t rawVacuum) {
    struct config       config;

    if (storageRead(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }
    config.rawIdleVacuum = rawVacuum;

    if (storageWrite(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }

    return (true);
SPACE_FAILURE:

    return (false);
}

bool configSetFirstThTimeout(uint32_t timeoutMs) {
    struct config       config;

    if (storageRead(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }
    config.test[0].timeoutMs = timeoutMs;
    
    if (storageWrite(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }

    return (true);
SPACE_FAILURE:

    return (false);
}

bool configSetFirstThRawVacuum(uint32_t rawVacuum) {
    struct config       config;

    if (storageRead(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }
    config.test[0].rawVacuum = rawVacuum;

    if (storageWrite(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }

    return (true);
SPACE_FAILURE:

    return (false);
}

bool configSetSecondThTimeout(uint32_t timeoutMs) {
    struct config       config;

    if (storageRead(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }
    config.test[1].timeoutMs = timeoutMs;

    if (storageWrite(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }

    return (true);
SPACE_FAILURE:

    return (false);
}

bool configSetSecondThRawVacuum(uint32_t rawVacuum) {
    struct config       config;

    if (storageRead(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }
    config.test[1].rawVacuum = rawVacuum;

    if (storageWrite(Storage, &config) != ES_ERROR_NONE) {
        goto SPACE_FAILURE;
    }

    return (true);
SPACE_FAILURE:

    return (false);
}

uint32_t configGetRawIdleVacuum(void) {
    struct config       config;

    storageRead(Storage, &config);

    return (config.rawIdleVacuum);
}

uint32_t configGetFirstThTimeout(void) {
    struct config       config;

    storageRead(Storage, &config);

    return (config.test[0].timeoutMs);
}

uint32_t configGetFirstThRawVacuum(void) {
    struct config       config;

    storageRead(Storage, &config);

    return (config.test[0].rawVacuum);
}

uint32_t configGetSecondThTimeout(void) {
    struct config       config;

    storageRead(Storage, &config);

    return (config.test[1].timeoutMs);
}

uint32_t configGetSecondThRawVacuum(void) {
    struct config       config;

    storageRead(Storage, &config);

    return (config.test[1].rawVacuum);
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

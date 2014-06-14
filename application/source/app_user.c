
#include "app_user.h"


static const char * const GlobalUserName[] = {
    "administrator",
    "operator"
};

static uint32_t GlobalUserId;

void appUserSetCurrent(uint32_t id) {
    GlobalUserId = id;
}

void appUserGetCurrent(struct appUser * user) {
    user->id   = GlobalUserId;
    user->name = GlobalUserName[GlobalUserId];
}



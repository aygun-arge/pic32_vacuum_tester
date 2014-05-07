
#include "app_config.h"
#include "Compiler.h"

static char Password[4] = "0123";

uint32_t configGetFirstThTimeout(void) {

    return (500);
}

uint32_t configGetFirstThRawVacuum(void) {

    return (70);
}

uint32_t configGetSecondThTimeout(void) {

    return (1000);
}

uint32_t configGetSecondThValue(void) {

    return (84);
}

bool configIsPasswordCharValid(char character, uint8_t position) {

    if (Password[position] == character) {

        return (true);
    } else {

        return (false);
    }
}

uint32_t configPasswordLength(void) {

    return (sizeof(Password));
}

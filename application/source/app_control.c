
#include "app_control.h"

#include "app_psensor.h"
#include "app_motor.h"

enum appControlState {
    APP_CONTROL_INIT,
    APP_CONTROL_WAIT_TRIGGER,
    APP_CONTROL_WAIT_USER,
    APP_CONTROL_TEST_DUT
};

void initControlModule(void) {
    
}

void appControl(void) {
    static enum appControlState state;

    switch (state) {
        case APP_CONTROL_INIT: {
            state = APP_CONTROL_WAIT_TRIGGER;

            break;
        }
        case APP_CONTROL_WAIT_TRIGGER: {

            if (isDutDetected() == true) {
                state = APP_CONTROL_WAIT_USER;
            }

            break;
        }
        case APP_CONTROL_WAIT_USER: {
            motorEnable();
            state = APP_CONTROL_TEST_DUT;

            break;
        }
        case APP_CONTROL_TEST_DUT : {
            
            if (isDutDetected() == false) {
                motorDisable();
                state = APP_CONTROL_WAIT_TRIGGER;
            }

            break;
        }
    }
}
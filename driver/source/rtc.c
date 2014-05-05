
#include "driver/rtc.h"
#include "driver/i2c.h"
#include "base/error.h"


#define REG_CONTROL_1                   0x00
#define REG_CONTROL_INT                 0x01
#define REG_CONTROL_INT_FLAG            0x02
#define REG_CONTROL_STATUS              0x03
#define REG_CONTROL_RESET               0x04

#define REG_SECONDS                     0x08
#define REG_MINUTES                     0x09
#define REG_HOURS                       0x0a
#define REG_DAYS                        0x0b
#define REG_WEEKDAYS                    0x0c
#define REG_MONTHS                      0x0d
#define REG_YEARS                       0x0e

#define REG_ALARM_SECONDS               0x10
#define REG_ALARM_MINUTE                0x11
#define REG_ALARM_HOUR                  0x12
#define REG_ALARM_DAYS                  0x13
#define REG_ALARM_WEEKDAYS              0x14
#define REG_ALARM_MONTHS                0x15
#define REG_ALARM_YEAR                  0x16

#define REG_ALARM_TIMER_LOW             0x18
#define REG_ALARM_TIMER_HIGH            0x19

#define REG_TEMPERATURE                 0x20

#define CONTROL_1_WE                    (0x1u << 0)
#define CONTROL_1_TE                    (0x1u << 1)
#define CONTROL_1_TAR                   (0x1u << 2)
#define CONTROL_1_EERE                  (0x1u << 3)
#define CONTROL_1_SRON                  (0x1u << 4)
#define CONTROL_1_TD0                   (0x1u << 5)
#define CONTROL_1_TD1                   (0x1u << 6)
#define CONTROL_1_CLK_INT               (0x1u << 7)

#define CONTROL_INT_AIE                 (0x1u << 0)
#define CONTROL_INT_TIE                 (0x1u << 1)
#define CONTROL_INT_V1IE                (0x1u << 2)
#define CONTROL_INT_V2IE                (0x1u << 3)
#define CONTROL_INT_SRIE                (0x1u << 4)

#define CONTROL_INT_FLAG_AF             (0x1u << 0)
#define CONTROL_INT_FLAG_TF             (0x1u << 1)
#define CONTROL_INT_FLAG_V1IF           (0x1u << 2)
#define CONTROL_INT_FLAG_V2IF           (0x1u << 3)
#define CONTROL_INT_FLAG_SRF            (0x1u << 4)

#define CONTROL_STATUS_V1F              (0x1u << 2)
#define CONTROL_STATUS_V2F              (0x1u << 3)
#define CONTROL_STATUS_SR               (0x1u << 4)
#define CONTROL_STATUS_PON              (0x1u << 5)
#define CONTROL_STATUS_EEBUSY           (0x1u << 7)

#define CONTROL_RESET_SYSR              (0x1u << 4)

#define RTC_SLAVE_ADDRESS               0xac
#define RTC_READ_CMD                    (RTC_SLAVE_ADDRESS & ~0x1u)
#define RTC_WRITE_CMD                   (RTC_SLAVE_ADDRESS |  0x1u)

static struct i2cHandle RtcI2c;

static esError rtcReadArray(uint8_t address, uint8_t * data, size_t size) {
    i2cStart(&RtcI2c);

    if (i2cWrite(&RtcI2c, RTC_WRITE_CMD) == false) {
        goto FAILURE;
    }

    if (i2cWrite(&RtcI2c, address) == false) {
        goto FAILURE;
    }
    i2cStop(&RtcI2c);
    i2cStart(&RtcI2c);

    if (i2cWrite(&RtcI2c, RTC_READ_CMD) == false) {
        goto FAILURE;
    }
    size--;

    if (size > 7) {
        size = 7;
    }

    if (size != 0) {
        i2cReadArray(&RtcI2c, data, size);
    }
    data[size] = i2cRead(&RtcI2c);
    i2cNack(&RtcI2c);
    i2cStop(&RtcI2c);

    return (ES_ERROR_NONE);
FAILURE:
    i2cStop(&RtcI2c);

    return (ES_ERROR_DEVICE_FAIL);
}

static esError rtcRead(uint8_t address, uint8_t * data) {

    return (rtcReadArray(address, data, 1));
}

void initRtcDriver(void) {
    uint8_t             reg;

    struct i2cConfig config = {
        &I2C1,
        I2C_ADDRESS_7BIT,
        100000,
        0
    };
    i2cOpen(&RtcI2c, &config);
}

void termRtcDriver(void) {
    i2cClose(&RtcI2c);
}

bool isRtcActive(void) {
    return (true);
}

void rtcGetTime(struct rtcTime * time) {
    time->year   = 2014;
    time->month  = 4;
    time->day    = 23;
    time->hour   = 21;
    time->minute = 53;
    time->second = 16;
}


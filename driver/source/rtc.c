
#include "driver/rtc.h"
#include "driver/i2c.h"
#include "base/error.h"
#include "base/debug.h"
#include "arch/intr.h"
#include "driver/gpio.h"
#include <string.h>
#include <xc.h>

#define CONFIG_DEFAULT_YEAR             2014
#define CONFIG_DEFAULT_MONTH            1
#define CONFIG_DEFAULT_DAY              1
#define CONFIG_DEFAULT_HOUR             0
#define CONFIG_DEFAULT_MINUTE           0
#define CONFIG_DEFAULT_SECOND           0

#define CONFIG_RTC_INT_PORT             &GpioA
#define CONFIG_RTC_INT_PIN              7

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

#define HOURS_12_24_MODE                (0x1u << 6)
#define HOURS_AM_PM                     (0x1u << 5)

#define RTC_SLAVE_ADDRESS               0xac
#define RTC_READ_CMD                    (RTC_SLAVE_ADDRESS |  0x1u)
#define RTC_WRITE_CMD                   (RTC_SLAVE_ADDRESS & ~0x1u)

struct rtcTimeRegisters {
    uint8_t             seconds;
    uint8_t             minutes;
    uint8_t             hours;
    uint8_t             days;
    uint8_t             weekdays;
    uint8_t             months;
    uint8_t             years;
};

static const ES_MODULE_INFO_CREATE("RTC", "Real Time Clock", "Nenad Radulovic");

static struct i2cHandle RtcI2c;

static struct rtcTime CurrentTime;

static esError rtcReadArray(uint8_t address, uint8_t * data, size_t size) {
    ES_REQUIRE(ES_API_RANGE, size < 8);
    ES_REQUIRE(ES_API_RANGE, size != 0);

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

static esError rtcWriteArray(uint8_t address, const uint8_t * data, size_t size) {
    ES_REQUIRE(ES_API_RANGE, size < 8);

    i2cStart(&RtcI2c);

    if (i2cWrite(&RtcI2c, RTC_WRITE_CMD) == false) {
        goto FAILURE;
    }

    if (i2cWrite(&RtcI2c, address) == false) {
        goto FAILURE;
    }

    if (i2cWriteArray(&RtcI2c, data, size) == false) {
        goto FAILURE;
    }
    i2cStop(&RtcI2c);

    return (ES_ERROR_NONE);
FAILURE:
    i2cStop(&RtcI2c);

    return (ES_ERROR_DEVICE_FAIL);
}

static esError rtcRead(uint8_t address, uint8_t * data) {

    return (rtcReadArray(address, data, 1));
}

static esError rtcWrite(uint8_t address, uint8_t data) {

    return (rtcWriteArray(address, &data, 1));
}

static uint8_t bcdToBin(uint8_t data) {
    uint8_t             retval;

    retval  = ((data >> 0) & 0xfu) * 1u;
    retval += ((data >> 4) & 0xfu) * 10u;

    return (retval);
}

static uint8_t binToBcd(uint8_t data) {
    uint8_t             retval;

    retval  = ((data / 10) & 0xfu) << 4;
    retval |= ((data % 10) & 0xfu) << 0;

    return (retval);
}

void initRtcDriver(void) {
    uint8_t             reg;
    struct rtcTimeRegisters regs;
    struct i2cConfig config = {
        &I2C1,
        I2C_ADDRESS_7BIT,
        100000,
        0
    };
    i2cOpen(&RtcI2c, &config);

    if (rtcRead(REG_CONTROL_STATUS, &reg) != ES_ERROR_NONE) {
        goto FAILURE;
    }

    if (reg & CONTROL_STATUS_PON) {
        reg &= ~CONTROL_STATUS_PON;

        if (rtcWrite(REG_CONTROL_STATUS, reg) != ES_ERROR_NONE) {
            goto FAILURE;
        }
        CurrentTime.year   = CONFIG_DEFAULT_YEAR;
        CurrentTime.month  = CONFIG_DEFAULT_MONTH;
        CurrentTime.day    = CONFIG_DEFAULT_DAY;
        CurrentTime.hour   = CONFIG_DEFAULT_HOUR;
        CurrentTime.minute = CONFIG_DEFAULT_MINUTE;
        CurrentTime.second = CONFIG_DEFAULT_SECOND;

        if (rtcSetTime(&CurrentTime) != ES_ERROR_NONE) {
            goto FAILURE;
        }
    }
    *(CONFIG_RTC_INT_PORT)->tris   |= (0x1u << CONFIG_RTC_INT_PIN);
    *(CONFIG_RTC_INT_PORT)->change |= (0x1u << CONFIG_RTC_INT_PIN);

#define CHANGE_INT_GPIOA                (0x1u << 13)
#define CHANGE_PRIO_GPIOA               (0x7u << 18)
#define CHANGE_SUBPRIO_GPIOA            (0x3u << 16)

#define CHANGE_INT_GPIOB                (0x1u << 14)
#define CHANGE_PRIO_GPIOB               (0x7u << 18)
#define CHANGE_SUBPRIO_GPIOB            (0x3u << 16)

#define CHANGE_INT_GPIOC                (0x1u << 15)
#define CHANGE_PRIO_GPIOC               (0x7u << 18)
#define CHANGE_SUBPRIO_GPIOC            (0x3u << 16)

    if (CONFIG_RTC_INT_PORT == &GpioA) {
        IFS1CLR       = CHANGE_INT_GPIOA;
        IPC8bits.CNIP = CONFIG_INTR_MAX_ISR_PRIO;
        IPC8bits.CNIS = 0;
        IEC1SET       = CHANGE_INT_GPIOA;
    } else if (CONFIG_RTC_INT_PORT == &GpioB) {
        IFS1CLR       = CHANGE_INT_GPIOB;
        IPC8bits.CNIP = CONFIG_INTR_MAX_ISR_PRIO;
        IPC8bits.CNIS = 0;
        IEC1SET       = CHANGE_INT_GPIOB;
    } else if (CONFIG_RTC_INT_PORT == &GpioC) {
        IFS1CLR       = CHANGE_INT_GPIOC;
        IPC8bits.CNIP = CONFIG_INTR_MAX_ISR_PRIO;
        IPC8bits.CNIS = 0;
        IEC1SET       = CHANGE_INT_GPIOC;
    }
    
    if (rtcWrite(REG_CONTROL_INT_FLAG, 0) != ES_ERROR_NONE) {
        goto FAILURE;
    }
    reg = CONTROL_INT_TIE;

    if (rtcWrite(REG_CONTROL_INT, reg) != ES_ERROR_NONE) {
        goto FAILURE;
    }
    reg = CONTROL_1_WE | CONTROL_1_TE | CONTROL_1_TAR | CONTROL_1_TD0;

    if (rtcWrite(REG_CONTROL_1, reg) != ES_ERROR_NONE) {
        goto FAILURE;
    }
    memset(&regs, 0, sizeof(regs));

    if (rtcWriteArray(REG_ALARM_SECONDS, (const uint8_t *)&regs, sizeof(regs)) != ES_ERROR_NONE) {
        goto FAILURE;
    }

    if (rtcGetTime(&CurrentTime) != ES_ERROR_NONE) {
        goto FAILURE;
    }

    return;
FAILURE:
    CurrentTime.year   = CONFIG_DEFAULT_YEAR;
    CurrentTime.month  = CONFIG_DEFAULT_MONTH;
    CurrentTime.day    = CONFIG_DEFAULT_DAY;
    CurrentTime.hour   = CONFIG_DEFAULT_HOUR;
    CurrentTime.minute = CONFIG_DEFAULT_MINUTE;
    CurrentTime.second = CONFIG_DEFAULT_SECOND;
}

void termRtcDriver(void) {
    i2cClose(&RtcI2c);
}

bool isRtcActive(void) {
    return (true);
}

esError rtcSetTime(const struct rtcTime * time) {
    struct rtcTimeRegisters regs;

    regs.years   = binToBcd(time->year - 2000u);
    regs.months  = binToBcd(time->month);
    regs.days    = binToBcd(time->day);
    regs.hours   = binToBcd(time->hour);
    regs.minutes = binToBcd(time->minute);
    regs.seconds = binToBcd(time->second);

    if (rtcWriteArray(REG_SECONDS, (const uint8_t *)&regs, sizeof(regs)) != ES_ERROR_NONE) {
        goto FAILURE;
    }

    return (ES_ERROR_NONE);
FAILURE:

    return (ES_ERROR_DEVICE_FAIL);
}

esError rtcGetTime(struct rtcTime * time) {
    
    struct rtcTimeRegisters regs;

    if (rtcReadArray(REG_SECONDS, (uint8_t *)&regs, sizeof(regs)) != ES_ERROR_NONE) {
        goto FAILURE;
    }
    time->year   = (uint16_t)bcdToBin(regs.years) + 2000u;
    time->month  = bcdToBin(regs.months);
    time->day    = bcdToBin(regs.days);
    time->hour   = bcdToBin(regs.hours);
    time->minute = bcdToBin(regs.minutes);
    time->second = bcdToBin(regs.seconds);

    return (ES_ERROR_NONE);
FAILURE:

    return (ES_ERROR_DEVICE_FAIL);
}

void __ISR(_CHANGE_NOTICE_VECTOR) changeNoticeHandler(void) {

    if (*(CONFIG_RTC_INT_PORT)->status & (0x1u << CONFIG_RTC_INT_PIN)) {
        uint32_t        port;

        port = *(CONFIG_RTC_INT_PORT)->port;

        if (port & (0x1u << CONFIG_RTC_INT_PIN)) {
            rtcGetTime(&CurrentTime);
        }

        if (CONFIG_RTC_INT_PORT == &GpioA) {
            IFS1CLR       = CHANGE_INT_GPIOA;
        } else if (CONFIG_RTC_INT_PORT == &GpioB) {
            IFS1CLR       = CHANGE_INT_GPIOB;
        } else if (CONFIG_RTC_INT_PORT == &GpioC) {
            IFS1CLR       = CHANGE_INT_GPIOC;
        }
    }
}
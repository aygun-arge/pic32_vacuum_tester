/*
 * File:    spi.c
 * Author:  nenad
 * Details: GPIO
 *
 * Created on February 17, 2014, 1:41 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "arch/intr.h"
#include "plat/critical.h"
#include <xc.h>
#include <sys/attribs.h>
#include <stdbool.h>

#include "driver/gpio.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CNCON_ON                        (0x1u << 15)

#define CHANGE_INT_GPIOA                (0x1u << 13)
#define CHANGE_PRIO_GPIOA               (0x7u << 18)
#define CHANGE_SUBPRIO_GPIOA            (0x3u << 16)

#define CHANGE_INT_GPIOB                (0x1u << 14)
#define CHANGE_PRIO_GPIOB               (0x7u << 18)
#define CHANGE_SUBPRIO_GPIOB            (0x3u << 16)

#define CHANGE_INT_GPIOC                (0x1u << 15)
#define CHANGE_PRIO_GPIOC               (0x7u << 18)
#define CHANGE_SUBPRIO_GPIOC            (0x3u << 16)

/*======================================================  LOCAL DATA TYPES  ==*/

struct change_slot
{
    void                     (* handler)(void);
    uint32_t                    bitwise_pin;
    uint32_t                    port_no;
    bool                        is_enabled;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static struct change_slot g_change_slot[CONFIG_MAX_CHANGE_HANDLERS];

/*======================================================  GLOBAL VARIABLES  ==*/

const struct gpio GpioA = {
    .port     = &PORTA,
    .tris     = &TRISA,
    .lat      = &LATA,
    .set      = &LATASET,
    .clr      = &LATACLR,
    .invert   = &LATAINV,
    .od       = &ODCA,
    .change   = &CNENA,
    .status   = &CNSTATA,
    .pullup   = &CNPUA,
    .pulldown = &CNPDA,
    .ansel    = &ANSELA
};

const struct gpio GpioB = {
    .port     = &PORTB,
    .tris     = &TRISB,
    .lat      = &LATB,
    .set      = &LATBSET,
    .clr      = &LATBCLR,
    .invert   = &LATBINV,
    .od       = &ODCB,
    .change   = &CNENB,
    .status   = &CNSTATB,
    .pullup   = &CNPUB,
    .pulldown = &CNPDB,
    .ansel    = &ANSELB
};

const struct gpio GpioC = {
    .port     = &PORTC,
    .tris     = &TRISC,
    .lat      = &LATC,
    .set      = &LATCSET,
    .clr      = &LATCCLR,
    .invert   = &LATCINV,
    .od       = &ODCC,
    .change   = &CNENC,
    .status   = &CNSTATC,
    .pullup   = &CNPUC,
    .pulldown = &CNPDC,
    .ansel    = &ANSELC
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void initPort(const struct gpio * gpio) {
    *(gpio->port)     = 0u;
    *(gpio->tris)     = 0xffffffffu;
    *(gpio->lat)      = 0u;
    *(gpio->od)       = 0u;
    *(gpio->change)   = 0u;
    *(gpio->pullup)   = 0u;
    *(gpio->pulldown) = 0u;
    *(gpio->ansel)    = 0u;
}

static const struct gpio * port_no_to_gpio(uint32_t num)
{
#if (((__PIC32_FEATURE_SET__ >= 100) && (__PIC32_FEATURE_SET__ <= 299)))
    static const struct gpio * num_to_gpio [] =
    {
        &GpioA,
        &GpioB,
        &GpioC
    };
    
    return (num_to_gpio[num]);
#endif
}

static uint32_t gpio_to_port_no(const struct gpio * gpio)
{
    uint32_t                    port_no;

    for (port_no = 0u; port_no < GPIO_NUM_OF_PORTS; port_no++) {
        if (gpio == port_no_to_gpio(port_no)) {
            return (port_no);
        }
    }
    return (-1);
}

static bool is_this_port_empty(uint32_t port_no)
{
    const struct gpio *         gpio = port_no_to_gpio(port_no);

    if (*(gpio->change) == 0u) {
        return (true);
    } else {
        return (false);
    }
}

static void find_and_exec(uint32_t port_no)
{
    uint32_t                    count;
    struct change_slot *        slot;

    for (count = 0u; count < CONFIG_MAX_CHANGE_HANDLERS; count++) {
        slot = &g_change_slot[count];

        if (slot->handler) {
            if (slot->is_enabled && (slot->port_no == port_no)) {
                slot->handler();
            }
        }
    }
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initGpioDriver(
    void) {
#if (((__PIC32_FEATURE_SET__ >= 100) && (__PIC32_FEATURE_SET__ <= 299)))
    /*
     * Setup all pins as digital IO with push-pull mode
     */
    initPort(&GpioA);
    initPort(&GpioB);
    initPort(&GpioC);
#endif
}

struct change_slot * gpio_request_slot(const struct gpio * gpio, uint32_t pin, void (* handler)(void))
{
    struct change_slot *        slot;
    uint32_t                    count;
    esIntrCtx                   intr_ctx;

    ES_CRITICAL_LOCK_ENTER(&intr_ctx);
    for (slot = NULL, count = 0; count < CONFIG_MAX_CHANGE_HANDLERS; count++) {
        if (g_change_slot[count].handler == NULL) {
            slot = &g_change_slot[count];
            slot->handler     = handler;
            slot->bitwise_pin = (0x1u << pin);
            slot->port_no     = gpio_to_port_no(gpio);
            slot->is_enabled  = false;

            break;
        }
    }
    ES_CRITICAL_LOCK_EXIT(intr_ctx);

    return (slot);
}

void gpio_release_slot(struct change_slot * slot)
{

}

void gpio_change_enable(struct change_slot * slot)
{
    esIntrCtx                   intr_ctx;

    slot->is_enabled = true;
    ES_CRITICAL_LOCK_ENTER(&intr_ctx);

    if (is_this_port_empty(slot->port_no)) {
        switch (slot->port_no) {
            case 0 : {
                IEC1CLR       = CHANGE_INT_GPIOA;
                CNCONASET     = CNCON_ON;
                IPC8bits.CNIP = CONFIG_INTR_MAX_ISR_PRIO;
                IPC8bits.CNIS = 0;
                IEC1SET       = CHANGE_INT_GPIOA;

                break;
            }
            case 1 : {
                IEC1CLR       = CHANGE_INT_GPIOB;
                CNCONBSET     = CNCON_ON;
                IPC8bits.CNIP = CONFIG_INTR_MAX_ISR_PRIO;
                IPC8bits.CNIS = 0;
                IEC1SET       = CHANGE_INT_GPIOB;

                break;
            }
            case 2 : {
                IEC1CLR       = CHANGE_INT_GPIOC;
                CNCONCSET     = CNCON_ON;
                IPC8bits.CNIP = CONFIG_INTR_MAX_ISR_PRIO;
                IPC8bits.CNIS = 0;
                IEC1SET       = CHANGE_INT_GPIOC;

                break;
            }
            default : {
                /* Nothing */
            }
        }
    }
    *(port_no_to_gpio(slot->port_no)->change) |= slot->bitwise_pin;
    ES_CRITICAL_LOCK_EXIT(intr_ctx);
}

void gpio_change_disable(struct change_slot * slot)
{
    esIntrCtx                   intr_ctx;

    slot->is_enabled = false;
    ES_CRITICAL_LOCK_ENTER(&intr_ctx);
    *(port_no_to_gpio(slot->port_no)->change) &= ~slot->bitwise_pin;

    if (is_this_port_empty(slot->port_no)) {
        switch (slot->port_no) {
            case 0 : {
                IEC1CLR = CHANGE_INT_GPIOA;

                break;
            }
            case 1 : {
                IEC1CLR = CHANGE_INT_GPIOB;

                break;
            }
            case 2 : {
                IEC1CLR = CHANGE_INT_GPIOC;

                break;
            }
            default : {
                /* Nothing */
            }
        }
    }
    ES_CRITICAL_LOCK_EXIT(intr_ctx);
}

void __ISR(_CHANGE_NOTICE_VECTOR) changeNotice(void) {
    volatile uint32_t   port;
    uint32_t            intFlag;

    intFlag = IEC1 & IFS1;
    port = *GpioA.port;

    if (intFlag & CHANGE_INT_GPIOA) {
        find_and_exec(0);
        IFS1CLR = CHANGE_INT_GPIOA;
    }
    port = *GpioB.port;

    if (intFlag & CHANGE_INT_GPIOB) {
        find_and_exec(1);
        IFS1CLR = CHANGE_INT_GPIOB;
    }
    port = *GpioC.port;

    if (intFlag & CHANGE_INT_GPIOC) {
        find_and_exec(2);
        IFS1CLR = CHANGE_INT_GPIOC;
    }
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of gpio.c
 ******************************************************************************/


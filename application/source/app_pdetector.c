/*
 * app_pdetector.c
 * This file is part of vacuum tester
 *
 * Copyright (C) 2014 - Nenad Radulović
 *
 * vacuum tester is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * vacuum tester is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vacuum tester. If not, see <http://www.gnu.org/licenses/>.
 */

#include "app_pdetector.h"
#include "driver/gpio.h"
#include "vtimer/vtimer.h"
#include "eds/epa.h"

#define CONFIG_PDETECTOR_PORT           &GpioA
#define CONFIG_PDETECTOR_PIN            8
#define CONFIG_TIMEOUT_MS               20
#define CONFIG_CONSUMER                 Gui

static esVTimer timeout;

static void timeout_handler(void * arg)
{
    esEvent *           notify;
    esError             error;

    (void)arg;
    ES_ENSURE(error = esEventCreateI(sizeof(struct event_debounce), EVENT_PDETECT, &notify);

    if (!error) {
        struct event_debounce * notify_;

        if (gpioRead(CONFIG_PDETECTOR_PORT) & CONFIG_PDETECTOR_PIN) {
            notify_->state = PDETECT_PRESENT;
        } else {
            notify_->state = PDETECT_NOT_PRESENT;
        }
        ES_ENSURE(esEpaSendEventI(Gui, notify_));

        gpioChangeDisableHandler
    }
}

static void debounce_handler(void)
{
    gpioChangeDisableHandler(CONFIG_PDETECTOR_PORT);
    esVTimerStartI(&timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_TIMEOUT_MS), timeout_handler, NULL);
}

void app_pdetector_init(void) 
{
    esVTimerInit(&timeout);
    gpioSetAsInput(CONFIG_PDETECTOR_PORT, CONFIG_PDETECTOR_PORT);
    gpioChangeSetHandler(CONFIG_PDETECTOR_PORT, (0x1u << CONFIG_PDETECTOR_PIN), debounce_handler);
    
}

/**
 * This file is part of the wiFred wireless model railroading clock project
 * Copyright (C) 2018  Heiko Rosemann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 *
 * This file provides functions for handling the internal clock and driving the external clock.
 */

#ifndef _CLOCK_HANDLING_H_
#define _CLOCK_HANDLING_H_

#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <stdint.h>
#include <stdbool.h>

#include "config.h"

// pins on which the clock outputs are located
#define CLOCK1_PIN 12
#define CLOCK2_PIN 14

// second clock output - disabled if commented out
#define CLOCK3_PIN 5
#define CLOCK4_PIN 4

#ifdef CLOCK3_PIN
  #define NUM_CLOCKS 2
#else
  #define NUM_CLOCKS 1
#endif

// maximum value for which times are considered to be the same
#define CLOCK_DELTA 10

extern uint8_t clockPulseLength;
extern uint8_t clockMaxRate;
extern int8_t clockOffset;

typedef struct
{
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
  uint8_t rate10;
  Ticker * secondTicker;
  Ticker * resetTicker;
  uint8_t pin1;
  uint8_t pin2;
  uint8_t edgeCounter;
} clockInfo;

extern clockInfo ourTime[NUM_CLOCKS];
extern clockInfo networkTime;
extern clockInfo startupTime;

typedef struct
{
  bool automatic;
  char * name;
  uint16_t port;
} serverInfo;

extern serverInfo clockServer;

extern char * automaticServer;
extern IPAddress automaticServerIP;

void initClock(void);

void clockHandler(void);

#endif

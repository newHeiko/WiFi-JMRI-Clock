/**
 * This file is part of the wiClock wireless model railroading fastclock project
 * Copyright (C) 2018-2021 Heiko Rosemann
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

// maximum value for which times are considered to be the same (ourTime may be ahead of network time, not the other way round!)
#define CLOCK_DELTA 10

typedef struct
{
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
  uint16_t rate10;
  Ticker * secondTicker;
} clockTime;

enum clockModes { CLOCK_MODE_SECOND, CLOCK_MODE_MINUTE, CLOCK_MODE_SWEEP16SECOND, CLOCK_MODE_SWEEP16MINUTE };

typedef struct
{
  Ticker * resetTicker;
  uint8_t pin1;
  uint8_t pin2;
  uint8_t tickCounter;
  uint8_t tickPerIncrement;
  bool nextEdgeHigh;
  clockModes clockMode;
  uint16_t clockPulseLength;
  uint16_t clockMaxTickFrequency;
} clockInfo;

extern clockTime ourTime[NUM_CLOCKS];
extern clockInfo clockHW[NUM_CLOCKS];
extern clockTime networkTime;
extern clockTime startupTime[NUM_CLOCKS];

extern volatile bool flagNewTime;

typedef struct
{
  bool automatic;
  char * name;
  uint16_t port;
} serverInfo;

extern serverInfo clockServer;

extern char * automaticServer;
extern IPAddress automaticServerIP;

/**
 * Initialize clock hardware / timers
 */
void initClock(void);

/**
 * Handles new time and key presses
 */
void handleNewTime(void);

/**
 * Connect to wiThrottle server
 */
void clockConnect(void);

/**
 * Register with wiThrottle server
 */
void clockServerRegister(void);

/**
 * Check for and handle data from wiThrottle server
 */
void clockHandler(void);

/**
 * If clock rate is larger than maximum, limit to maximum
 */
void limitClockRate(clockTime * c, uint16_t maximum);

#endif

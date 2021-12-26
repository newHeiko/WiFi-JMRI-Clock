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
 * This file provides functions for measuring the battery voltage and shutting
 * down the system if the voltage is too low.
 * 
 * A 100kOhm/47kOhm voltage divider at the ADC input pin is required for proper
 * voltage readings.
 */

#include <ESP8266WiFi.h>

#include "lowbat.h"

/**
 * Number of ADC samples to take for a single new voltage value
 */
#define NUM_AD_SAMPLES 16

/**
 * Periodically check battery voltage and react if falling below the above defined thresholds
 */
void lowBatteryHandler(void)
{
  #if NUM_AD_SAMPLES > 64
  #warning "Increase data type size of buffer for more than 64 samples"
  #endif
  static uint16_t buffer = 0;
  static uint8_t index = 0;

  static uint32_t lastRead = 0;

  if(millis() > lastRead)
  {
    lastRead += 8000 / NUM_AD_SAMPLES;

    buffer += analogRead(A0);
    index++;
    if(index == NUM_AD_SAMPLES)
    {
      // calculate battery voltage in millivolts ( / 47 * 147 to account for for hardware voltage divider)
      uint32_t value = buffer / NUM_AD_SAMPLES * 147 * 1000 / 47 / 1024;
      if(value < LOW_BATTERY_MILLIVOLTS)
      {
        lowBattery = true;
      }
      else
      {
        lowBattery = false;
        emptyBattery = false;
      }
      if(value < EMPTY_BATTERY_MILLIVOLTS)
      {
        emptyBattery = true;
      }
      batteryVoltage = value;
      buffer = index = 0;
    }
  }
}

/**
 * Set to true when the device receives a BLOW message, false when receiving a BOK message
 */
bool lowBattery;

/**
 * Set to true when the device receives a BEMPTY message
 */
bool emptyBattery;

/**
 * Battery voltage in milliVolt
 */
uint16_t batteryVoltage;

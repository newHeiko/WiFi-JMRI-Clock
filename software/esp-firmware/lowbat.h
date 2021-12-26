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
 * This file provides functions for measuring the battery voltage and shutting
 * down the system if the voltage is too low.
 * 
 * A 100kOhm/47kOhm voltage divider at the ADC input pin is required for proper
 * voltage readings.
 */

#ifndef _LOWBAT_H_
#define _LOWBAT_H_

/**
 * Battery voltage for the device to detect low battery status
 * Will change behaviour of device
 */
#define LOW_BATTERY_MILLIVOLTS 2100
/**
 * Battery voltage for the device to shut down (deep sleep, never wakeup)
 */
#define EMPTY_BATTERY_MILLIVOLTS 1800

/**
 * Periodically check battery voltage and react if falling below the above defined thresholds
 */
void lowBatteryHandler(void);

/**
 * Set to true when the device receives a BLOW message, false when receiving a BOK message
 */
extern bool lowBattery;

/**
 * Set to true when the device receives a BEMPTY message
 */
extern bool emptyBattery;

/**
 * Battery voltage in milliVolt (capped at approx. 3V)
 */
extern uint16_t batteryVoltage;

#endif

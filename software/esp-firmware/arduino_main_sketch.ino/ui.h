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
 * This file provides functions to set/delete the status LED
 * and to read in an input button
 */

#ifndef _UI_H_
#define _UI_H_

#define LED_PIN 16
const byte KEY_PIN[4] = { 0, 4, 13, 16 };

enum keys { KEY_CONFIG, KEY_HOUR, KEY_MINUTE, KEY };

/**
 * Setup pullup resistor, pin output mode, callback ticker etc. at startup
 */
void initUI(void);

/**
 * Call to set new LED values
 * 
 * @param onTime: on time in milliseconds
 * @param dutyCycle: total cycle time in milliseconds
 */
void setLED(uint16_t onTime, uint16_t dutyCycle);

/**
 * Get state of input button
 * 
 * @returns true if input button is pressed (pin value is low)
 */
bool getInputState(keys key);

/**
 * Get input state changes from input button
 * 
 * @returns true if button has been pressed since last call
 */
bool getInputPressed(keys key);

#endif

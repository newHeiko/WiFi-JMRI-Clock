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
 * This file provides functions to set/delete the status LED
 * and to read in an input button
 */

#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "ui.h"

Ticker ledDutyCycle;
Ticker ledOnTime;

Ticker debounceInput;

bool inputState[3] = { false, false, false };
bool inputPressed[3] = { false, false, false };

uint16_t m_onTime;
uint16_t m_dutyCycle;

/**
 * Get state of input buttons
 * 
 * @param the key to query
 * @return true if input button is pressed (pin value is low)
 */
bool getInputState(keys key)
{
  return inputState[key];
}

/**
 * Get input state changes from input button
 * 
 * @param the key to query
 * @returns true if button has been pressed since last call
 */
bool getInputPressed(keys key)
{
  if(inputPressed[key])
  {
    inputPressed[key] = false;
    return true;
  }
  return false;
}

/**
 * Callback function for debouncing keys
 */
void debounceInputCallback(void)
{
  static uint8_t counter[3] = {0, 0, 0};

  for(uint8_t index = KEY_CONFIG; index <= KEY_HOUR; index++)
  {
    if(digitalRead(KEY_PIN[index]) == LOW && inputState[index] == false)
    {
      if(counter[index] >= 4)
      {
        inputState[index] = true;
        inputPressed[index] = true;
        counter[index] = 0;
      }
      else
      {
        counter[index]++;
      }
    }
    else if(digitalRead(KEY_PIN[index]) == HIGH && inputState[index] == true)
    {
      if (counter[index] >= 4)
      {
        inputState[index] = false;
        counter[index] = 0;
      }
      else
      {
        counter[index]++;
      }
    }
    else
    {
      counter[index] = 0;
    }
  }
}

/**
 * Callback to turn off LED (at end of onTime)
 */
void turnLEDoff(void)
{
  digitalWrite(LED_PIN, HIGH);
}

/**
 * Callback to turn on LED (at beginning of duty cycle)
 */
void turnLEDon(void)
{
  digitalWrite(LED_PIN, LOW);
  if(m_onTime < m_dutyCycle)
  {
    ledOnTime.once_ms(10 * m_onTime, turnLEDoff);
  }
}

/**
 * Call to set new LED values
 * 
 * @param onTime: on time in 10 millisecond increments
 * @param dutyCycle: total cycle time in 10 millisecond increments
 */
void setLED(uint16_t onTime, uint16_t dutyCycle)
{
  if(onTime == m_onTime && dutyCycle == m_dutyCycle)
  {
    return;
  }
  if(onTime == 0)
  {
    ledDutyCycle.detach();
    ledOnTime.detach();
    digitalWrite(LED_PIN, HIGH);
  }
  else if(dutyCycle == 0)
  {
    ledDutyCycle.detach();
    ledOnTime.detach();
    digitalWrite(LED_PIN, LOW);
  }
  else
  {
    ledDutyCycle.attach_ms(10 * dutyCycle, turnLEDon);
    m_onTime = onTime;
    m_dutyCycle = dutyCycle;
  }
}

/**
 * Setup pullup resistor, pin output mode, callback ticker etc. at startup
 */
void initUI(void)
{
  for(uint8_t index = KEY_CONFIG; index <= KEY_HOUR; index++)
  {
    pinMode(KEY_PIN[index], INPUT_PULLUP);
  }
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  debounceInput.attach_ms(10, debounceInputCallback);
}

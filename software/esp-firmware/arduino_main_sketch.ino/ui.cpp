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

#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "ui.h"

Ticker ledDutyCycle;
Ticker ledOnTime;

Ticker debounceInput;

bool inputState = false;
bool inputPressed = false;

uint16_t m_onTime;
uint16_t m_dutyCycle;

/**
 * Get state of input button
 * 
 * @returns true if input button is pressed (pin value is low)
 */
bool getInputState(void)
{
  return inputState;
}

/**
 * Get input state changes from input button
 * 
 * @returns true if button has been pressed since last call
 */
bool getInputPressed(void)
{
  if(inputPressed)
  {
    inputPressed = false;
    return true;
  }
  return false;
}

/**
 * Callback function for debouncing keys
 */
void debounceInputCallback(void)
{
  static uint8_t counter = 0;

  if(digitalRead(KEY_PIN) == LOW && inputState == false)
  {
    if(counter >= 4)
    {
      inputState = true;
      inputPressed = true;
      counter = 0;
    }
    else
    {
      counter++;
    }
  }
  else if(digitalRead(KEY_PIN) == HIGH && inputState == true)
  {
    if (counter >= 4)
    {
      inputState = false;
      counter = 0;
    }
    else
    {
      counter++;
    }
  }
  else
  {
    counter = 0;
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
  pinMode(KEY_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  debounceInput.attach_ms(10, debounceInputCallback);
}

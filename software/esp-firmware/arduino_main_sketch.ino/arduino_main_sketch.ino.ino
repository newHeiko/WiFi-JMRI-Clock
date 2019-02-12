/**
 * This file is part of the wiFred wireless model railroading throttle project
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
 * This file ties everything together to initialize the hardware and
 * form the main loop.
 * 
 * This project was made for ESP12E/ESP12F with Arduino ESP version 2.4.1
 * Board settings: Generic ESP8266 Module, 160MHz, nodemcu, 26MHz, 80MHz, QIO, 
 * 4M (1M SPIFFS), 2, v2 Lower Memory, 
 * Serial, None, Only Sketch, 115200 on /dev/ttyUSB0
 */

#include "wifi.h"
#include "config.h"
#include "lowbat.h"
#include "stateMachine.h"

// #define DEBUG

state wiFredState = STATE_STARTUP;
uint32_t stateTimeout = UINT32_MAX;

// temporary getInputState
bool getInputState(uint8_t input)
{
  return false;
}


void setup() {
// put your setup code here, to run once:

  initConfig();
  initClock();
  
  Serial.begin(115200);
  Serial.setTimeout(10);
  #ifdef DEBUG
  Serial.setDebugOutput(true);
  #else
  Serial.setDebugOutput(false);
  #endif
  delay(100);

  initWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  handleWiFi();
  clockHandler();

  static uint32_t test = 0;

#ifdef DEBUG
  if(test < millis())
  {
    test = millis() + 5000;
    Serial.println(ESP.getFreeHeap());
  }
#endif

  switch(wiFredState)
  {
    case STATE_STARTUP:
      if(!clockActive || getInputState(0) == true || getInputState(1) == true || getInputState(2) == true || getInputState(3) == true)
      {
        initWiFiSTA();
        switchState(STATE_CONNECTING, 60 * 1000);
      }
      else
      {
        initWiFiAP();
        switchState(STATE_CONFIG_AP);
      }
      break;
      
    case STATE_CONNECTING:
      if(WiFi.status() == WL_CONNECTED)
      {
        switchState(STATE_CONNECTED);
      }
      else if(millis() > stateTimeout)
      {
        initWiFiAP();
        switchState(STATE_CONFIG_AP);
      }
      break;

    case STATE_CONNECTED:
      if(getInputState(0) == false && getInputState(1) == false && getInputState(2) == false && getInputState(3) == false)
      {
        if(clockActive)
        {
          initWiFiConfigSTA();
          switchState(STATE_CONFIG_STATION_WAITING, 30 * 1000);
        }
        else
        {
          switchState(STATE_LOWPOWER_WAITING, 30 * 1000);
        }
        break;
      }

      if(WiFi.status() != WL_CONNECTED)
      {
        initWiFiSTA();
        switchState(STATE_CONNECTING, 30 * 1000);
      }
      break;

    case STATE_CONFIG_STATION_WAITING:
      if(millis() > stateTimeout)
      {
        shutdownWiFiSTA();
        switchState(STATE_LOWPOWER);
        break;
      }
    // break;
    // intentional fall-through
    case STATE_CONFIG_STATION:
      if(clockActive && (getInputState(0) == true || getInputState(1) == true || getInputState(2) == true || getInputState(3) == true))
      {
        shutdownWiFiConfigSTA();
        switchState(STATE_CONNECTED);
      }
      break;

    case STATE_CONFIG_STATION_COMING:
      if(millis() > stateTimeout)
      {
         initWiFiConfigSTA();
         switchState(STATE_CONFIG_STATION);
      }
      break;

    case STATE_LOWPOWER_WAITING:
      if(millis() > stateTimeout)
      {
        shutdownWiFiSTA();
        switchState(STATE_LOWPOWER);
      }
    // break;
    // intentional fall-through
    case STATE_LOWPOWER:
      if(getInputState(0) == true || getInputState(1) == true || getInputState(2) == true || getInputState(3) == true)
      {
         switchState(STATE_STARTUP);
      }
      break;
      
    case STATE_CONFIG_AP:
    // no way to get out of here except for restart
      break;
  }
}

void switchState(state newState, uint32_t timeout)
{
  wiFredState = newState;
  if(timeout == UINT32_MAX)
  {
    stateTimeout = timeout;
  }
  else
  {
    stateTimeout = millis() + timeout;
  }
}


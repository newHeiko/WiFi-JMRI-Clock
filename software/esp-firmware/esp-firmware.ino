/**
 * This file is part of the wiClock wireless model railroading fastclock project
 * Copyright (C) 2018-2023 Heiko Rosemann
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
 * This project was made for ESP12E/ESP12F with Arduino ESP version 2.5.2
 * Board settings: Generic ESP8266 Module, 80MHz, Flash, Disabled, ck, 26MHz,
 * 40MHz, DOUT (compatible), 4M (1M SPIFFS), 2, v1.4 Higher Bandwidth, 
 * Disabled, None, Only Sketch, 115200 on /dev/ttyUSB0
 */
/* Boards:
 * - Generic ESP8266 Module * esp8266:esp8266:generic:led=2,baud=115200,xtal=80,CrystalFreq=26,eesz=4M1M,FlashMode=dout,FlashFreq=40,ResetMethod=nodtr_nosync,dbg=Disabled,lvl=None____,ip=hb2f,vt=flash,exception=disabled,stacksmash=disabled,wipe=none,sdk=nonosdk_191122,ssl=basic,mmu=3232,non32xfer=fast,CONSOLEBAUD=115200
 */

#include "wifi.h"
#include "config.h"
#include "lowbat.h"
#include "stateMachine.h"
#include "ui.h"

// #define DEBUG

state wiFredState = STATE_STARTUP;
uint32_t stateTimeout = UINT32_MAX;

void setup() {
// put your setup code here, to run once:

  initUI();
  Serial.begin(115200);
  Serial.setTimeout(10);
  initConfig();
  initClock();
  
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
  handleNewTime();
  lowBatteryHandler();

#ifdef DEBUG
  static uint32_t test = 0;

  if(test < millis())
  {
    test = millis() + 5000;
    Serial.println(ESP.getFreeHeap());
  }
#endif

  if(emptyBattery)
  {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    ESP.deepSleep(0);
  }
  
  switch(wiFredState)
  {
    case STATE_STARTUP:
      setLED(100, 200);
      switchState(STATE_CONNECTING, 60 * 1000);
      initWiFiSTA();
      break;
      
    case STATE_CONNECTING:
      setLED(100, 200);
      if(WiFi.status() == WL_CONNECTED)
      {
        setLED(25, 50);
        initMDNS();
        switchState(STATE_CONNECTED, 60 * 1000);
      }
      else if(millis() > stateTimeout)
      {
        setLED(190, 200);
        initWiFiAP();
        switchState(STATE_CONFIG_AP);
      }
      break;

    case STATE_CONNECTED:
    case STATE_SERVERCONN:
    case STATE_ONLINE:
      if(getInputPressed(KEY_CONFIG) == true)
      {
        setLED(100, 100);
        initWiFiConfigSTA();
        switchState(STATE_CONFIG_STATION_WAITING, 120 * 1000);
        break;
      }      

      if(WiFi.status() != WL_CONNECTED)
      {
        switchState(STATE_STARTUP);
        break;
      }
      switch(wiFredState)
      {
        case STATE_CONNECTED:
          clockConnect();
          break;
        case STATE_SERVERCONN:
          clockServerRegister();
          break;
        case STATE_ONLINE:
          clockHandler();
          break;
        default:
          break;
      }
      break;

    case STATE_CONFIG_STATION_WAITING:
      setLED(100, 100);
      if(millis() > stateTimeout || getInputPressed(KEY_CONFIG) == true)
      {
        setLED(25, 50);
        shutdownWiFiConfigSTA();
        switchState(STATE_CONNECTED);
      }
      break;

    case STATE_CONFIG_STATION:
      if(getInputPressed(KEY_CONFIG) == true)
      {
        setLED(25, 50);
        shutdownWiFiConfigSTA();
        switchState(STATE_CONNECTED);
      }

      if(WiFi.status() != WL_CONNECTED)
      {
        switchState(STATE_STARTUP);
      }

      break;

    case STATE_CONFIG_AP:
    // no way to get out of here except for restart
      setLED(190, 200);
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

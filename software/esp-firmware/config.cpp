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
 * This file provides functions for handling configuration settings in
 * non-volatile memory.
 */

#include <stdbool.h>
#include <FS.h>
#include <ArduinoJson.h>

#include "config.h"
#include "wifi.h"
#include "clockHandling.h"

char * throttleName;

/*
 * Read all configuration from EEPROM
 */
void initConfig(void)
{
  uint8_t mac[6];
  WiFi.macAddress(mac);
  String tN = "wiClock-" + String(mac[0], 16) + String(mac[5], 16);
  throttleName = strdup(tN.c_str());

  clockServer.automatic = true;
  clockServer.name = strdup("undef");
  clockServer.port = 12080;

  for(uint8_t i=0; i<NUM_CLOCKS; i++)
  {
    startupTime[i].hours = 5;
    startupTime[i].minutes = 30;
    startupTime[i].seconds = 0;
    startupTime[i].rate10 = 10;
    
    clockHW[i].clockMaxTickFrequency = 6;
    clockHW[i].clockPulseLength = 40;
    clockHW[i].minuteMode = false;
  }
  clockOffset = 2;
  
  if(!SPIFFS.begin())
  {
    return;
  }
  
  DynamicJsonDocument doc(256);

// read device name from SPIFFS
  if(File f = SPIFFS.open(FN_NAME, "r"))
  {
    if(!deserializeJson(doc, f))
    {
      const char * s = doc[FIELD_NAME_NAME];
      if(s != nullptr)
      {
        free(throttleName);
        throttleName = strdup(s);
      }
    }
    f.close();
  }

// read server configuration/information from SPIFFS
  if(File f = SPIFFS.open(FN_SERVER, "r"))
  {
    if(!deserializeJson(doc, f))
    {
      const char * s = doc[FIELD_SERVER_NAME];
      if(s != nullptr)
      {
        free(clockServer.name);
        clockServer.name = strdup(s);
      }
      uint16_t p = doc[FIELD_SERVER_PORT];
      if(p != 0)
      {
        clockServer.port = p;
      }
      if(doc.containsKey(FIELD_SERVER_AUTOMATIC))
      {
        clockServer.automatic = doc[FIELD_SERVER_AUTOMATIC];
      }
    }
    f.close();
  }

// read startup time from SPIFFS - global settings from old system
  if(File f = SPIFFS.open(FN_STARTUP_TIME, "r"))
  {
    if(!deserializeJson(doc, f))
    {
      if( doc.containsKey(FIELD_STARTUP_HOUR) && doc.containsKey(FIELD_STARTUP_MINUTE) 
       && doc.containsKey(FIELD_STARTUP_SECOND) && doc.containsKey(FIELD_STARTUP_RATE) )
      {
        for(uint8_t i = 0; i<NUM_CLOCKS; i++)
        {
          startupTime[i].hours = doc[FIELD_STARTUP_HOUR];
          startupTime[i].minutes = doc[FIELD_STARTUP_MINUTE];
          startupTime[i].seconds = doc[FIELD_STARTUP_SECOND];
          startupTime[i].rate10 = doc[FIELD_STARTUP_RATE];
        }
      }
    }
    f.close();
  }

// read startup time from SPIFFS - per-clock settings from new system
  for(uint8_t i = 0; i<NUM_CLOCKS; i++)
  {
    char filename[sizeof(FN_STARTUP_TIME + 1)];
    snprintf(filename, sizeof(FN_STARTUP_TIME + 1), "%d%s", i, FN_STARTUP_TIME);
    if(File f = SPIFFS.open(filename, "r"))
    {
      if(!deserializeJson(doc, f))
      {
        if( doc.containsKey(FIELD_STARTUP_HOUR) && doc.containsKey(FIELD_STARTUP_MINUTE) 
         && doc.containsKey(FIELD_STARTUP_SECOND) && doc.containsKey(FIELD_STARTUP_RATE) )
        {
          startupTime[i].hours = doc[FIELD_STARTUP_HOUR];
          startupTime[i].minutes = doc[FIELD_STARTUP_MINUTE];
          startupTime[i].seconds = doc[FIELD_STARTUP_SECOND];
          startupTime[i].rate10 = doc[FIELD_STARTUP_RATE];
        }
      }
      f.close();
    }
  }

// read clock configuration from SPIFFS - global settings from old system
  if(File f = SPIFFS.open(FN_CLOCK_CONFIG, "r"))
  {
    if(!deserializeJson(doc, f))
    {
      if(doc.containsKey(FIELD_CLOCK_OFFSET))
      {
        clockOffset = doc[FIELD_CLOCK_OFFSET];
      }
      for(uint8_t i = 0; i<NUM_CLOCKS; i++)
      {
        if(doc.containsKey(FIELD_CLOCK_MAXRATE))
        {
          clockHW[i].clockMaxTickFrequency = doc[FIELD_CLOCK_MAXRATE];
        }
        if(doc.containsKey(FIELD_CLOCK_PULSELENGTH))
        {
          clockHW[i].clockPulseLength = doc[FIELD_CLOCK_PULSELENGTH];
        }
        if(doc.containsKey(FIELD_CLOCK_MINUTEMODE))
        {
          clockHW[i].minuteMode = doc[FIELD_CLOCK_MINUTEMODE];
        }
      }
    }
    f.close();
  }

// read clock configuration from SPIFFS - per-clock settings from new system
  for(uint8_t i = 0; i<NUM_CLOCKS; i++)
  {
    char filename[sizeof(FN_CLOCK_CONFIG + 1)];
    snprintf(filename, sizeof(FN_CLOCK_CONFIG + 1), "%d%s", i, FN_CLOCK_CONFIG);
    if(File f = SPIFFS.open(filename, "r"))
    {
      if(!deserializeJson(doc, f))
      {
        if(doc.containsKey(FIELD_CLOCK_MAXRATE))
        {
          clockHW[i].clockMaxTickFrequency = doc[FIELD_CLOCK_MAXRATE];
        }
        if(doc.containsKey(FIELD_CLOCK_PULSELENGTH))
        {
          clockHW[i].clockPulseLength = doc[FIELD_CLOCK_PULSELENGTH];
        }
        if(doc.containsKey(FIELD_CLOCK_MINUTEMODE))
        {
          clockHW[i].minuteMode = doc[FIELD_CLOCK_MINUTEMODE];
        }
      }
      f.close();
    }
  }

// read as many wifi configurations as there are available from SPIFFS  
  String filename;

  for(uint8_t i = 0; true; i++)
  {
    // create new filename
    filename = String(FN_WIFI_STUB) + i + ".txt";
    // check if it exists, exit loop if not
    if(!SPIFFS.exists(filename))
    {
      break;
    }

    if(File f = SPIFFS.open(filename, "r"))
    {
      if(!deserializeJson(doc, f))
      {
        wifiAPEntry newAP;
        newAP.ssid = strdup(doc[FIELD_WIFI_SSID] | "");
        newAP.key = strdup(doc[FIELD_WIFI_PSK] | "");
  
        if(strcmp(newAP.ssid, ""))
        {
          apList.push_back(newAP);
        }
        else
        {
          free(newAP.ssid);
          free(newAP.key);
        }
      }
      f.close();
    }
  }

  SPIFFS.end();
}

void saveClockServer()
{
  if(!SPIFFS.begin())
  {
    return;
  }

  DynamicJsonDocument doc(256);

  doc[FIELD_SERVER_NAME] = clockServer.name;
  doc[FIELD_SERVER_PORT] = clockServer.port;
  doc[FIELD_SERVER_AUTOMATIC] = clockServer.automatic;

  if(File f = SPIFFS.open(FN_SERVER, "w"))
  {
    serializeJson(doc, f);
    f.close();
  }

  SPIFFS.end();
}

void saveClockStartup(uint8_t clockID)
{
  if(clockID >= NUM_CLOCKS)
  {
    return;
  }

  if(!SPIFFS.begin())
  {
    return;
  }

  DynamicJsonDocument doc(256);

  doc[FIELD_STARTUP_HOUR] = startupTime[clockID].hours;
  doc[FIELD_STARTUP_MINUTE] = startupTime[clockID].minutes;
  doc[FIELD_STARTUP_SECOND] = startupTime[clockID].seconds;
  doc[FIELD_STARTUP_RATE] = startupTime[clockID].rate10;

  char filename[sizeof(FN_STARTUP_TIME + 1)];
  snprintf(filename, sizeof(FN_STARTUP_TIME + 1), "%d%s", clockID, FN_STARTUP_TIME);

  if(File f = SPIFFS.open(filename, "w"))
  {
    serializeJson(doc, f);
    f.close();
  }
  
  SPIFFS.end();
}

void saveClockConfig(uint8_t clockID)
{
  if(!SPIFFS.begin())
  {
    return;
  }

  DynamicJsonDocument doc(256);

  if(clockID < NUM_CLOCKS)
  {
    doc[FIELD_CLOCK_MAXRATE] = clockHW[clockID].clockMaxTickFrequency;
    doc[FIELD_CLOCK_PULSELENGTH] = clockHW[clockID].clockPulseLength;
    doc[FIELD_CLOCK_MINUTEMODE] = clockHW[clockID].minuteMode;
  }
  doc[FIELD_CLOCK_OFFSET] = clockOffset;

  char filename[sizeof(FN_CLOCK_CONFIG + 1)];
  if(clockID < NUM_CLOCKS)
  {
    snprintf(filename, sizeof(FN_CLOCK_CONFIG + 1), "%d%s", clockID, FN_CLOCK_CONFIG);
  }
  else
  {
    strncpy(filename, FN_CLOCK_CONFIG, sizeof(filename));
  }
   
  if(File f = SPIFFS.open(filename, "w"))
  {
    serializeJson(doc, f);
    f.close();
  }
  SPIFFS.end();
}

void saveGeneralConfig()
{
  if(!SPIFFS.begin())
  {
    return;
  }

  DynamicJsonDocument doc(256);

  doc[FIELD_NAME_NAME] = throttleName;

  if(File f = SPIFFS.open(FN_NAME, "w"))
  {
    serializeJson(doc, f);
    f.close();
  }

  SPIFFS.end();
}

void saveWiFiConfig()
{
  if(!SPIFFS.begin())
  {
    return;
  }
  
  DynamicJsonDocument doc(256);

  String filename;

  for(uint8_t i = 0; ; i++)
  {
    // create new filename
    filename = String(FN_WIFI_STUB) + i + ".txt";
    // check if it exists, exit loop if not
    if(!SPIFFS.exists(filename))
    {
      break;
    }
    SPIFFS.remove(filename);
  }

  uint8_t i = 0;
  for(std::vector<wifiAPEntry>::iterator it = apList.begin(); it != apList.end(); it++, i++)
  {
    // create new filename
    filename = String(FN_WIFI_STUB) + i + ".txt";

    doc[FIELD_WIFI_SSID] = it->ssid;
    doc[FIELD_WIFI_PSK] = it->key;

    if(File f = SPIFFS.open(filename, "w"))
    {
      serializeJson(doc, f);
      f.close();
    }
    doc.clear();
  }

  SPIFFS.end();
}

void deleteAllConfig()
{
  SPIFFS.format();
}

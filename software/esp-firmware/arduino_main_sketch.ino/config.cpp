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
 * This file provides functions for handling configuration settings in
 * non-volatile memory.
 */

#include <stdbool.h>

#include "config.h"
#include "wifi.h"
#include "clockHandling.h"

bool wifiSaved = false;
bool clockSaved = false;

char throttleName[NAME_CHARS];

void eepromReadBlock(uint8_t * dest, const uint16_t src, size_t length)
{
  uint16_t local_src;
  for(local_src = src; local_src < src + length; dest++, local_src++)
  {
    *dest = EEPROM.read(local_src);
  }
}

void eepromWriteBlock(const uint16_t dest, uint8_t * src, size_t length)
{
  uint16_t local_dest;
  for(local_dest = dest; local_dest < dest + length; local_dest++, src++)
  {
    EEPROM.write(local_dest, *src);
  }
}

/*
 * Read all configuration from EEPROM
 */
void initConfig(void)
{
  EEPROM.begin(512);

  // check for valid EEPROM data
  uint8_t idClock = EEPROM.read(ID_CLOCK);

// initialize to default values
  memcpy(wlan.ssid, "undef", sizeof("undef"));
  memcpy(wlan.key, "undef", sizeof("undef"));
  memcpy(throttleName, "undef", sizeof("undef"));
  memcpy(clockServer.name, "undef", sizeof("undef"));
  clockServer.port = 12080;

  startupTime.hours = 5;
  startupTime.minutes = 30;
  startupTime.seconds = 0;
  startupTime.rate10 = 10;
  clockMaxRate = 10;
  clockPulseLength = 40;
  clockOffset = 2;

  
  if(idClock == EEPROM_VALID) // general EEPROM data is valid
  {
    eepromReadBlock((uint8_t *) wlan.ssid, WLAN_SSID, sizeof(wlan.ssid)/sizeof(wlan.ssid[0]));
    wlan.ssid[sizeof(wlan.ssid)/sizeof(wlan.ssid[0]) - 1] = '\0';
    eepromReadBlock((uint8_t *) wlan.key, WLAN_KEY, sizeof(wlan.key)/sizeof(wlan.key[0]));
    wlan.key[sizeof(wlan.key)/sizeof(wlan.key[0]) - 1] = '\0';
    eepromReadBlock((uint8_t *) throttleName, NAME, sizeof(throttleName)/sizeof(throttleName[0]));
    throttleName[sizeof(throttleName)/sizeof(throttleName[0]) - 1] = '\0';
    wifiSaved = true;
    clockActive = (bool) EEPROM.read(CLOCK_ACTIVE);
    clockMaxRate = EEPROM.read(CLOCK_MAX_RATE);
    clockPulseLength = EEPROM.read(CLOCK_PULSE_LENGTH);
    clockOffset = (int8_t) EEPROM.read(CLOCK_OFFSET);
    eepromReadBlock((uint8_t *) &startupTime, CLOCK_STARTUP, sizeof(startupTime));
    eepromReadBlock((uint8_t *) &clockServer, CLOCK_SERVER, sizeof(clockServer));
    clockSaved = true;
  }
}

void saveClockConfig()
{
  EEPROM.write(CLOCK_ACTIVE, clockActive);
  EEPROM.write(CLOCK_MAX_RATE, clockMaxRate);
  EEPROM.write(CLOCK_PULSE_LENGTH, clockPulseLength);
  EEPROM.write(CLOCK_OFFSET, clockOffset);

  eepromWriteBlock(CLOCK_STARTUP, (uint8_t *) &startupTime, sizeof(startupTime));
  eepromWriteBlock(CLOCK_SERVER, (uint8_t *) &clockServer, sizeof(clockServer));

  clockSaved = true;
  
  if(wifiSaved)
  {
    EEPROM.write(ID_CLOCK, EEPROM_VALID);
  }
  EEPROM.commit();
}

void saveGeneralConfig()
{
  eepromWriteBlock(WLAN_SSID, (uint8_t *) wlan.ssid, sizeof(wlan.ssid)/sizeof(wlan.ssid[0]));
  eepromWriteBlock(WLAN_KEY, (uint8_t *) wlan.key, sizeof(wlan.key)/sizeof(wlan.key[0]));
  eepromWriteBlock(NAME, (uint8_t *) throttleName, sizeof(throttleName)/sizeof(throttleName[0]));

  wifiSaved = true;
  if(clockSaved)
  {
    EEPROM.write(ID_CLOCK, EEPROM_VALID);
  }
  EEPROM.commit();
}

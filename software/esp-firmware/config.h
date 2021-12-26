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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <EEPROM.h>
#include <stdbool.h>
#include <stdint.h>
#include "wifi.h"
#include "clockHandling.h"

// Filenames and field names on SPIFFS
#define FN_STARTUP_TIME "startupTime.txt"
#define FIELD_STARTUP_HOUR "hh"
#define FIELD_STARTUP_MINUTE "mm"
#define FIELD_STARTUP_SECOND "ss"
#define FIELD_STARTUP_RATE "rate"

#define FN_CLOCK_CONFIG "clockConfig.txt"
#define FIELD_CLOCK_MAXRATE "maxRate"
#define FIELD_CLOCK_PULSELENGTH "pulseLength"
#define FIELD_CLOCK_OFFSET "offset"
#define FIELD_CLOCK_MINUTEMODE "minutemode"

#define FN_SERVER "server.txt"
#define FIELD_SERVER_NAME "name"
#define FIELD_SERVER_PORT "port"
#define FIELD_SERVER_AUTOMATIC "automatic"

#define FN_NAME "name.txt"
#define FIELD_NAME_NAME "name"

#define FN_WIFI_STUB "wifi"
#define FIELD_WIFI_SSID "ssid"
#define FIELD_WIFI_PSK "key"

/**
 * A user-given name for this device
 */
extern char * throttleName;

/**
 * Read all configuration from SPIFFS
 */
void initConfig(void);

/**
 * Check for slide switch settings to enter configuration mode and
 * handle configuration requests
 */
void configHandler(void);

/**
 * Save clock server settings
 */
void saveClockServer();

/**
 * Save clock startup time and rate
 * 
 * @param clockID The ID of the clock that shall be saved (0-based)
 */
void saveClockStartup(uint8_t clockID);

/**
 * Save clock configuration
 * 
 * @param clockID The ID of the clock that shall be saved (0-based)
 */
void saveClockConfig(uint8_t clockID);

/**
 * Save general device configuration
 */
void saveGeneralConfig();

/**
 * Save WiFi configuration
 */
void saveWiFiConfig();

/**
 * Reformat configuration filesystem
 * Resets everything to factory defaults
 */
void deleteAllConfig();

#endif

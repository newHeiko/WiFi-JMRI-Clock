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
 * This file handles connecting and reconnecting to a wireless network as well
 * as providing a webserver for configuration of the device and status readout.
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>

#include "wifi.h"
#include "clockHandling.h"
#include "config.h"
#include "lowbat.h"
#include "Ticker.h"
#include "stateMachine.h"
#include "gitVersion.h"

// #define DEBUG

std::vector<wifiAPEntry> apList;

ESP8266WiFiMulti wifiMulti;

ESP8266WebServer server(80);
DNSServer dnsServer;
ESP8266HTTPUpdateServer updater;

void readString(char * dest, size_t maxLength, String input)
{
  strncpy(dest, input.c_str(), maxLength - 1);
  dest[maxLength - 1] = '\0';
}

void handleWiFi(void)
{
  server.handleClient();
  switch (wiFredState)
  {
    case STATE_CONFIG_AP:
      dnsServer.processNextRequest();
    // intentional fall-through

    case STATE_CONNECTED:
    case STATE_SERVERCONN:
    case STATE_ONLINE:
    case STATE_CONFIG_STATION:
    case STATE_CONFIG_STATION_WAITING:
      MDNS.update();
      break;

    case STATE_STARTUP:
    case STATE_CONNECTING:
      wifiMulti.run();
      break;
  }
}

/**
   Stop listening on <throttleName>.local, start listening on config.local
*/
void initWiFiConfigSTA(void)
{
  MDNS.removeService(NULL, "http", "tcp");
  MDNS.setHostname("config");
  MDNS.addService("http", "tcp", 80);
}

/**
   Stop listening on (MDNS) config.local, start listening on <throttleName>.local
*/
void shutdownWiFiConfigSTA(void)
{
  char * hostName = strdup(throttleName);

  for (char * src = throttleName, * dst = hostName; *src != 0;)
  {
    if (isalnum(*src))
    {
      *dst++ = *src++;
    }
    else
    {
      src++;
    }
    *dst = 0;
  }

#ifdef DEBUG
  Serial.println(String("Add MDNS ") + hostName + " on throttle name " + throttleName);
#endif

  MDNS.removeService(NULL, "http", "tcp");
  MDNS.setHostname(hostName);
  MDNS.addService("http", "tcp", 80);
}

void initWiFiSTA(void)
{
  // count the number of available networks
  uint32_t numNetworks = 0;
  WiFi.mode(WIFI_STA);

  for (std::vector<wifiAPEntry>::iterator it = apList.begin() ; it != apList.end(); it++)
  {
    if (!it->disabled)
    {
      wifiMulti.addAP(it->ssid, it->key);
      numNetworks++;
    }
  }
  if (numNetworks == 0)
  {
    initWiFiAP();
    switchState(STATE_CONFIG_AP);
  }
}

void initMDNS(void)
{
  char * hostName = strdup(throttleName);

  for (char * src = throttleName, * dst = hostName; *src != 0;)
  {
    if (isalnum(*src))
    {
      *dst++ = *src++;
    }
    else
    {
      src++;
    }
    *dst = 0;
  }

#ifdef DEBUG
  Serial.println(String("Add MDNS ") + hostName + " on throttle name " + throttleName);
#endif

  MDNS.begin(hostName);
  MDNS.addService("http", "tcp", 80);
}

void initWiFiAP(void)
{
  // open an AP for configuration if connection failed
  WiFi.disconnect();
  WiFi.mode(WIFI_AP_STA);
  uint8_t mac[6];
  WiFi.macAddress(mac);
  String ssid = "wiClock-config" + String(mac[0], 16) + String(mac[5], 16);
#ifdef DEBUG
  Serial.println();
  Serial.print("Not connected, setting up AP at ");
  Serial.println(ssid);
#endif
  WiFi.softAP(ssid.c_str());

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", WiFi.softAPIP());

  MDNS.begin("config");
  MDNS.addService("http", "tcp", 80);
}

void writeMainPage()
{
  if (wiFredState == STATE_CONFIG_STATION_WAITING)
  {
    switchState(STATE_CONFIG_STATION);
  }

  // check if this is a "set general configuration" request
  if (server.hasArg("throttleName"))
  {
    free(throttleName);
    throttleName = strdup(server.arg("throttleName").c_str());
#ifdef DEBUG
    Serial.print("New throttleName: ");
    Serial.println(throttleName);
#endif
    saveGeneralConfig();
  }

  // check if this is a "manually add WiFi network" request
  if (server.hasArg("wifiSSID"))
  {
    wifiAPEntry newAP;

    newAP.ssid = strdup(server.arg("wifiSSID").c_str());
    if (strcmp(newAP.ssid, "") == 0)
    {
      free(newAP.ssid);
    }
    else
    {
      newAP.key = strdup(server.arg("wifiKEY").c_str());
      apList.push_back(newAP);

      saveWiFiConfig();
    }
  }

  // check if this is a "remove WiFi network" request
  // will remove all networks with this SSID
  if (server.hasArg("remove"))
  {
    for (std::vector<wifiAPEntry>::iterator it = apList.begin() ; it != apList.end(); )
    {
      if (strcmp(it->ssid, server.arg("remove").c_str()) == 0)
      {
        free(it->ssid);
        free(it->key);
        it = apList.erase(it);
      }
      else
      {
        it++;
      }
    }
    saveWiFiConfig();
  }

  // check if this is a "disable WiFi network" request
  // will disable all networks with this SSID
  if (server.hasArg("disable"))
  {
    for (std::vector<wifiAPEntry>::iterator it = apList.begin() ; it != apList.end(); ++it)
    {
      if (strcmp(it->ssid, server.arg("disable").c_str()) == 0)
      {
        it->disabled = true;
      }
    }
    saveWiFiConfig();
  }

  // check if this is an "enable WiFi network" request
  // will enable all networks with this SSID
  if (server.hasArg("enable"))
  {
    for (std::vector<wifiAPEntry>::iterator it = apList.begin() ; it != apList.end(); ++it)
    {
      if (strcmp(it->ssid, server.arg("enable").c_str()) == 0)
      {
        it->disabled = false;
      }
    }
    saveWiFiConfig();
  }

  // check if this is a "set clock server" request
  if (server.hasArg("clock.serverName") && server.hasArg("clock.serverPort"))
  {
    free(clockServer.name);
    clockServer.name = strdup(server.arg("clock.serverName").c_str());
    clockServer.port = server.arg("clock.serverPort").toInt();
    clockServer.automatic = server.hasArg("clock.automatic");

    if (!clockServer.automatic)
    {
      free(automaticServer);
      automaticServer = nullptr;
    }

    saveClockServer();
  }

  // check if this is a "set clock startup time" request
  if (server.hasArg("clock.startUp") && server.hasArg("clock.startupRate") && server.hasArg("clock.ID"))
  {
    String startupString = server.arg("clock.startUp");
    alignas(4) unsigned int hours, minutes, seconds;
    uint8_t id = server.arg("clock.ID").toInt();

    if (sscanf(startupString.c_str(), "%u:%u:%u", &hours, &minutes, &seconds) == 3)
    {
      if (hours < 24 && minutes < 60 && seconds < 60 && id < NUM_CLOCKS)
      {
        startupTime[id].hours = (uint8_t) hours % 12;
        startupTime[id].minutes = (uint8_t) minutes;
        startupTime[id].seconds = (uint8_t) seconds;
      }
    }
    if (id < NUM_CLOCKS)
    {
      startupTime[id].rate10 = (uint16_t) 10 * server.arg("clock.startupRate").toFloat();
      switch(clockHW[id].clockMode)
      {
        case CLOCK_MODE_SECOND:
          limitClockRate(&(startupTime[id]), 10 * clockHW[id].clockMaxTickFrequency);
          break;

        case CLOCK_MODE_MINUTE:
          limitClockRate(&(startupTime[id]), 10 * 60 * clockHW[id].clockMaxTickFrequency);
          break;

        case CLOCK_MODE_SWEEP16SECOND:
          limitClockRate(&(startupTime[id]), 10 * clockHW[id].clockMaxTickFrequency / 16);
          break;

        case CLOCK_MODE_SWEEP16MINUTE:
          limitClockRate(&(startupTime[id]), 10 * 60 * clockHW[id].clockMaxTickFrequency / 16);
          break;
      }          
      saveClockStartup(id);
    }
  }

  // check if this is a "set clock startup time" request
  if (server.hasArg("clock.maxClockRate") && server.hasArg("clock.pulseLength") && server.hasArg("clock.ID"))
  {
    uint8_t id = server.arg("clock.ID").toInt();
    if (id < NUM_CLOCKS)
    {
      clockHW[id].clockMaxTickFrequency = server.arg("clock.maxClockRate").toInt();
      clockHW[id].clockPulseLength = server.arg("clock.pulseLength").toInt();
      clockHW[id].clockMode = (clockModes) server.arg("clock.Mode").toInt();

      saveClockConfig(id);
    }
  }

  // check if this is a "set clock time" request
  if (server.hasArg("clock.time") && server.hasArg("clock.ID"))
  {
    uint8_t i = server.arg("clock.ID").toInt();
    if (i < NUM_CLOCKS)
    {
      String newTime = server.arg("clock.time");
      alignas(4) unsigned int hours, minutes, seconds;

      if (sscanf(newTime.c_str(), "%u:%u:%u", &hours, &minutes, &seconds) == 3)
      {
        if (hours < 24 && minutes < 60 && seconds < 60)
        {
          ourTime[i].hours = (uint8_t) hours % 12;
          ourTime[i].minutes = (uint8_t) minutes;
          ourTime[i].seconds = (uint8_t) seconds;

          flagNewTime = true;
        }
      }
    }
  }

  char timeString[12];

  String resp = String("<!DOCTYPE HTML>\r\n")
                + "<html><head><title>wiClock configuration page</title></head>\r\n"
                + "<body><h1>wiClock configuration page</h1>\r\n";

  resp        += String("<hr>General configuration and status<hr>\r\n")
                + "<form action=\"index.html\" method=\"get\"><table border=0>"
                + "<tr><td>Device name:</td><td><input type=\"text\" name=\"throttleName\" value=\"" + throttleName + "\"></td><td><input type=\"submit\" value=\"Save name\"></td></tr></table></form>\r\n";
  
  snprintf(timeString, sizeof(timeString) / sizeof(timeString[0]), "%02d:%02d:%02d", networkTime.hours, networkTime.minutes, networkTime.seconds);
  
  resp        += String("<table border=0><tr><td>Network time: </td><td>") + timeString + "</td><td>and rate: </td><td>" + networkTime.rate10 / 10 + "</td></tr>\r\n"
                 + "<tr><td>Battery voltage: </td><td>" + batteryVoltage + " mV</td><td>" + (lowBattery ? "Battery LOW" : "" ) + "</td><td></td></tr>\r\n"
                 + "<tr><td>Firmware revision:</td><td colspan=3>" + REV + "</td></tr></table>\r\n"
                 + "<table><tr><td>Active WiFi network SSID:</td><td>" + (WiFi.isConnected() ? WiFi.SSID() : "not connected") + "</td></tr>"
                 + "<tr><td>Signal strength:</td><td>" + (WiFi.isConnected() ? (String) WiFi.RSSI() + "dB" : "not connected") + "</td></tr></table>\r\n"
                 + "<hr>";

  for (uint8_t i = 0; i < NUM_CLOCKS; i++)
  {
    snprintf(timeString, sizeof(timeString) / sizeof(timeString[0]), "%02d:%02d:%02d", ourTime[i].hours, ourTime[i].minutes, ourTime[i].seconds);

    resp        += String("<form action=\"index.html\" method=\"get\"><table border=0>")
                   + "<tr><th colspan=2>Clock number: " + (i + 1) + "</th></tr>\r\n"
                   + "<tr><td>System time: </td><td><input type=\"text\" name=\"clock.time\" value=\"" + timeString + "\">"
                   + "<input type=\"hidden\" name=\"clock.ID\" value=\"" + i + "\">"
                   + "<input type=\"submit\" value=\"Set Time\"></td></tr>\r\n"
                   + "<tr><td>Clock rate " + (i + 1) + ": </td><td>" + (ourTime[i].rate10 / 10.0) + "</td></tr></table></form>\r\n";

    snprintf(timeString, sizeof(timeString) / sizeof(timeString[0]), "%02d:%02d:%02d", startupTime[i].hours, startupTime[i].minutes, startupTime[i].seconds);

    resp        += String("<form action=\"index.html\" method=\"get\"><table border=0>")
                   + "<tr><td>Startup time (format: H:M:S):</td><td><input type=\"text\" name=\"clock.startUp\" value=\"" + timeString + "\">"
                   + "<input type=\"hidden\" name=\"clock.ID\" value=\"" + i + "\"></td></tr>"
                   + "<tr><td>Startup clock rate:</td><td><input type=\"text\" name=\"clock.startupRate\" value=\"" + startupTime[i].rate10 / 10.0 + "\"></td></tr>"
                   + "<tr><td colspan=2><input type=\"submit\" value=\"Save startup time and rate\"></td></tr></table></form>"
                   + "<form action=\"index.html\" method=\"get\"><table border=0>"
                   + "<tr><td>Maximum clock tick frequency:</td><td><input type=\"text\" name=\"clock.maxClockRate\" value=\"" + clockHW[i].clockMaxTickFrequency + "\">"
                   + "<input type=\"hidden\" name=\"clock.ID\" value=\"" + i + "\"></td></tr>"
                   + "<tr><td>Pulse length for clock (milliseconds):</td><td><input type=\"text\" name=\"clock.pulseLength\" value=\"" + clockHW[i].clockPulseLength + "\"></td></tr>"
                   + "<tr><td>Clock mode:</td><td><select id=\"clock.Mode\" name=\"clock.Mode\">"
                   + "<option value=\"" + CLOCK_MODE_SECOND + "\"" + (clockHW[i].clockMode == CLOCK_MODE_SECOND ? " selected " : "" ) + ">One tick per second</option>"
                   + "<option value=\"" + CLOCK_MODE_MINUTE + "\"" + (clockHW[i].clockMode == CLOCK_MODE_MINUTE ? " selected " : "" ) + ">One tick per minute</option>"
                   + "<option value=\"" + CLOCK_MODE_SWEEP16SECOND + "\"" + (clockHW[i].clockMode == CLOCK_MODE_SWEEP16SECOND ? " selected " : "" ) + ">Sweeping hand: 16 ticks per second</option>"
                   + "<option value=\"" + CLOCK_MODE_SWEEP16MINUTE + "\"" + (clockHW[i].clockMode == CLOCK_MODE_SWEEP16MINUTE ? " selected " : "" ) + ">Sweeping hand: 16 ticks per minute</option>"
                   + "</select></td></tr>"
                   + "<tr><td colspan=2><input type=\"submit\" value=\"Save clock configuration\"></td></tr></table></form>\r\n";
  }

  uint32_t numNetworks = 0;

  resp        += String("<hr>WiFi configuration<hr>\r\n")
                 + "<table border=0><tr><td colspan=3><a href=scanWifi.html>Scan for networks</a></td></tr>"
                 + "<tr><td colspan=3>Known and enabled WiFi networks:</td></tr>";
  for (std::vector<wifiAPEntry>::iterator it = apList.begin() ; it != apList.end(); ++it)
  {
    if (!it->disabled)
    {
      resp += String("<tr><td>Network name: ") + it->ssid + "</td>"
              + "<td><form action=\"index.html\" method=\"get\"><input type=\"hidden\" name=\"remove\" value=\"" + it->ssid + "\"><input type=\"submit\" value=\"Remove Network\"></form></td>"
              + "<td><form action=\"index.html\" method=\"get\"><input type=\"hidden\" name=\"disable\" value=\"" + it->ssid + "\"><input type=\"submit\" value=\"Disable Network\"></form></td></tr>\r\n";
      numNetworks++;
    }
  }

  if (numNetworks == 0)
  {
    resp += String("<tr><td colspan=3>None.</td></tr>");
  }
  numNetworks = 0;

  resp += String("<tr><td colspan=3>Known but disabled WiFi networks:</td></tr>");

  for (std::vector<wifiAPEntry>::iterator it = apList.begin() ; it != apList.end(); ++it)
  {
    if (it->disabled)
    {
      resp += String("<tr><td>Network: ") + it->ssid + "</td>"
              + "<td><form action=\"index.html\" method=\"get\"><input type=\"hidden\" name=\"remove\" value=\"" + it->ssid + "\"><input type=\"submit\" value=\"Remove Network\"></form></td>"
              + "<td><form action=\"index.html\" method=\"get\"><input type=\"hidden\" name=\"enable\" value=\"" + it->ssid + "\"><input type=\"submit\" value=\"Enable Network\"></form></td></tr>\r\n";
      numNetworks++;
    }
  }

  if (numNetworks == 0)
  {
    resp += String("<tr><td colspan=3>None.</td></tr>");
  }

  resp        += String("<form action=\"index.html\" method=\"get\"><tr>")
                 + "<td>New SSID: <input type=\"text\" name=\"wifiSSID\"></td>"
                 + "<td>New PSK: <input type=\"text\" name=\"wifiKEY\"></td>"
                 + "<td><input type = \"submit\" value=\"Manually add network\"></td>"
                 +  "</tr></form></table>\r\n";

  resp        += String("<a href=restart.html>Restart wiClock to enable new WiFi settings</a>\r\n")
                 + " WiFi settings will not be active until restart.\r\n";

  resp        += String("<hr>Clock server configuration<hr>")
                 + "<form action=\"index.html\" method=\"get\"><table border=0>"
                 + "<tr><td>Clock server and port: </td>"
                 + "<td>http://<input type=\"text\" name=\"clock.serverName\" value=\"" + clockServer.name + "\">:<input type=\"text\" name=\"clock.serverPort\" value=\"" + clockServer.port + "\">/json/time</td></tr>"
                 + "<tr><td>Find server automatically through Zeroconf/Bonjour?</td><td><input type=\"checkbox\" name=\"clock.automatic\"" + (clockServer.automatic ? " checked" : "") + ">"
                 + "Using http://" + (clockServer.automatic && automaticServer != nullptr ? automaticServer : clockServer.name) + ":" + clockServer.port + "/json/time</td></tr>"
                 + "<tr><td colspan=2><input type=\"submit\" value=\"Save clock server settings\"></td></tr></table></form>";

  resp        += String("<hr>wiClock system<hr>\r\n")
                 + "<a href=resetConfig.html>Reset wiClock to factory defaults</a>\r\n"
                 + "<a href=update>Update wiClock firmware</a>\r\n"
                 + "</body></html>";


  server.send(200, "text/html", resp);
}

void scanWifi()
{
  uint32_t n = WiFi.scanNetworks();

  String resp = String("<!DOCTYPE HTML>\r\n")
                + "<html><head><title>Scan for WiFi networks</title></head>"
                + "<body><h1>Results of WiFi scan</h1>"
                + "<table border=0>";

  if (n == 0)
  {
    resp += String("<tr><td>No WiFi networks found. Reload to repeat scan.</td></tr>");
  }

  for (uint32_t i = 0; i < n; i++)
  {
    resp += String("<form action=\"index.html\" method = \"get\">")
            + "<tr><td>" + WiFi.SSID(i) + "<input type=\"hidden\" name=\"wifiSSID\" value=\"" + WiFi.SSID(i) + "\"></td>"
            + "<td>Enter PSK here if required: <input type=\"text\" name=\"wifiKEY\"></td>"
            + "<td><input type=\"submit\" value=\"Add network\"></td></tr></form>";
  }

  resp += String("</table>")
          + "<a href=index.html>Return to main page</a></body></html>";

  server.send(200, "text/html", resp);
}

void restartESP()
{
  MDNS.removeService(NULL, "http", "tcp");
  ESP.restart();
}

void resetESP()
{
  if (server.hasArg("reallyReset"))
  {
    deleteAllConfig();
    restartESP();
  }
  else
  {
    String resp = String("<!DOCTYPE HTML>\r\n")
                  + "<html><head><title>Reset wiClock to factory defaults</title></head>\r\n"
                  + "<body><h1>Reset wiClock to factory defaults?</h1>\r\n"
                  + "<table border=0>"
                  + "<tr><td><a href=\"/resetConfig.html?reallyReset=on\">Yes, really reset the wiClock to factory defaults</a></td></tr>\r\n"
                  + "<tr><td><a href=\"/index.html\">No, return to configuration page</a></td></tr>\r\n"
                  + "</table>\r\n"
                  + "</body></html>";
    server.send(200, "text/html", resp);
  }
}

void initWiFi(void)
{
  server.on("/", writeMainPage);
  server.on("/scanWifi.html", scanWifi);
  server.on("/restart.html", restartESP);
  server.on("/resetConfig.html", resetESP);
  server.onNotFound(writeMainPage);

  updater.setup(&server);

  // start configuration webserver
  server.begin();
}

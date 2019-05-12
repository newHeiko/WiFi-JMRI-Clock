/**
   This file is part of the wiFred wireless model railroading clock project
   Copyright (C) 2018  Heiko Rosemann

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>

   This file handles connecting and reconnecting to a wireless network as well
   as providing a webserver for configuration of the device and status readout.
*/

#include <ESP8266WiFi.h>
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

// #define DEBUG

t_wlan wlan;

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
  switch(wiFredState)
  {
    case STATE_CONFIG_AP:
      dnsServer.processNextRequest();
      // intentional fall-through

    case STATE_CONNECTED:
    case STATE_CONFIG_STATION:
    case STATE_CONFIG_STATION_WAITING:
      MDNS.update();
      break;

    case STATE_STARTUP:
    case STATE_CONNECTING:
      break;
  }
}

void initWiFiConfigSTA(void)
{
  // stop listening on <throttleName>.local, start listening on config.local
  MDNS.removeService(NULL, "http", "tcp");
  MDNS.setHostname("config");
  MDNS.addService("http", "tcp", 80);
}

void shutdownWiFiConfigSTA(void)
{
  // stop listening on config.local, start listening on <throttleName>.local
  
  char hostName[NAME_CHARS];
  
  for(char * src = throttleName, * dst = hostName; *src != 0;)
    {
      if(isalnum(*src))
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
  WiFi.disconnect();
  WiFi.config(0u, 0u, 0u);

  WiFi.mode(WIFI_STA);

#ifdef DEBUG
  Serial.print("Attempting connection to ");
  Serial.print(wlan.ssid);
  Serial.print(" with key ");
  Serial.println(wlan.key);
#endif

  WiFi.begin(wlan.ssid, wlan.key);
}

void initMDNS(void)
{
  char hostName[NAME_CHARS];
  
  for(char * src = throttleName, * dst = hostName; *src != 0;)
    {
      if(isalnum(*src))
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
  WiFi.mode(WIFI_AP);
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

  // check if this is a "set configuration" request
  if (server.hasArg("throttleName") && server.hasArg("wifi.ssid") && server.hasArg("wifi.key"))
  {
    readString(throttleName, sizeof(throttleName) / sizeof(throttleName[0]), server.arg("throttleName"));
    readString(wlan.ssid, sizeof(wlan.ssid) / sizeof(wlan.ssid[0]), server.arg("wifi.ssid"));
    readString(wlan.key, sizeof(wlan.key) / sizeof(wlan.key[0]), server.arg("wifi.key"));

    saveGeneralConfig();
  }

  String resp = String("<!DOCTYPE HTML>\r\n")
                + "<html><head><title>wiClock configuration page</title></head>\r\n"
                + "<body><h1>General configuration</h1>\r\n"
                + "<form action=\"index.html\" method=\"get\"><table border=0>"
                + "<tr><td>Throttle name (max " + String(sizeof(throttleName) / sizeof(throttleName[0]) - 1) + " chars):</td><td><input type=\"text\" name=\"throttleName\" value=\"" + throttleName + "\"></td></tr>"
                + "<tr><td>WiFi SSID (max " + String(sizeof(wlan.ssid) / sizeof(wlan.ssid[0]) - 1) + " chars):</td><td><input type=\"text\" name=\"wifi.ssid\" value=\"" + wlan.ssid + "\"></td></tr>"
                + "<tr><td>WiFi PSK (max " + String(sizeof(wlan.key) / sizeof(wlan.key[0]) - 1) + " chars):</td><td><input type=\"text\" name=\"wifi.key\" value=\"" + wlan.key + "\"></td></tr>"
                + "<tr><td colspan=2><input type=\"submit\"></td></tr></table></form>\r\n";


  resp        += String("<hr>Clock configuration<hr>\r\n")
                 + "<a href=clock.html>Clock configuration subpage</a>\r\n";

  resp        += String("<hr>Restart system to enable new WiFi settings<hr>\r\n")
                 + "<a href=restart.html>Restart system to enable new WiFi settings</a>\r\n"
                 + "<hr><hr>Status page<hr>\r\n"
                 + "<a href=status.html>wiClock status subpage</a>\r\n"
                 + "<hr><hr>Update firmware<hr>\r\n"
                 + "<a href=update>Update wiClock firmware</a>\r\n"
                 + "</body></html>";
  server.send(200, "text/html", resp);
}

void writeClockPage()
{
#ifdef DEBUG
  Serial.println("Clock page");
#endif
  // check if this is a "set configuration" request
  if (server.hasArg("clock.serverName") && server.hasArg("clock.serverPort") && server.hasArg("clock.startUp"))
  {
    readString(clockServer.name, sizeof(clockServer.name) / sizeof(clockServer.name[0]), server.arg("clock.serverName"));
    clockServer.port = server.arg("clock.serverPort").toInt();
    String startupString = server.arg("clock.startUp");
    alignas(4) unsigned int hours, minutes, seconds;

    if (sscanf(startupString.c_str(), "%u:%u:%u", &hours, &minutes, &seconds) == 3)
    {
      if (hours < 24 && minutes < 60 && seconds < 60)
      {
        startupTime.hours = (uint8_t) hours % 12;
        startupTime.minutes = (uint8_t) minutes;
        startupTime.seconds = (uint8_t) seconds;
      }
    }
    clockOffset = server.arg("clock.offset").toInt();
    clockMaxRate = server.arg("clock.maxClockRate").toInt();
    startupTime.rate10 = (uint8_t) 10 * server.arg("clock.startupRate").toFloat();
    if (startupTime.rate10 > 10 * clockMaxRate)
    {
      startupTime.rate10 = 10 * clockMaxRate;
    }
    clockPulseLength = server.arg("clock.pulseLength").toInt();

    saveClockConfig();
  }


  char startupString[9];
  snprintf(startupString, sizeof(startupString) / sizeof(startupString[0]), "%02d:%02d:%02d", startupTime.hours, startupTime.minutes, startupTime.seconds);

  String resp = String("<!DOCTYPE HTML>\r\n")
                + "<html><head><title>wiFred configuration page</title></head>\r\n"
                + "<body><h1>Clock configuration</h1>\r\n"
                + "<form action=\"clock.html\" method=\"get\"><table border=0>"
                + "<tr><td>Clock server and port: </td>"
                + "<td>http://<input type=\"text\" name=\"clock.serverName\" value=\"" + clockServer.name + "\">:<input type=\"text\" name=\"clock.serverPort\" value=\"" + clockServer.port + "\">/json/time</td></tr>"
                + "<tr><td>Startup time (format: H:M:S):</td><td><input type=\"text\" name=\"clock.startUp\" value=\"" + startupString + "\"></td></tr>"
                + "<tr><td>Clock offset from UTC (hours):</td><td><input type=\"text\" name=\"clock.offset\" value=\"" + clockOffset + "\"></td></tr>"
                + "<tr><td>Startup clock rate:</td><td><input type=\"text\" name=\"clock.startupRate\" value=\"" + startupTime.rate10 / 10.0 + "\"></td></tr>"
                + "<tr><td>Maximum clock rate:</td><td><input type=\"text\" name=\"clock.maxClockRate\" value=\"" + clockMaxRate + "\"></td></tr>"
                + "<tr><td>Pulse length for clock (milliseconds):</td><td><input type=\"text\" name=\"clock.pulseLength\" value=\"" + clockPulseLength + "\"></td></tr>"
                + "<tr><td><input type=\"submit\"></td><td><a href=\"/\">Back to main configuration page (unsaved data will be lost)</a></td></tr></table></form>\r\n"
                + "</body></html>";
  server.send(200, "text/html", resp);
}

void writeStatusPage()
{
  char timeString[9];
  snprintf(timeString, sizeof(timeString) / sizeof(timeString[0]), "%02d:%02d:%02d", ourTime.hours, ourTime.minutes, ourTime.seconds);
  String resp = String("<!DOCTYPE HTML>\r\n")
                + "<html><head><title>wiFred status page</title></head>\r\n"
                + "<body><h1>wiFred status</h1>\r\n"
                + "<table border=0>"
                + "<tr><td>Battery voltage: </td><td>" + batteryVoltage + " mV" + (lowBattery ? " Battery LOW" : "" ) + "</td></tr>\r\n"
                + "<tr><td>System time: </td><td>" + timeString + "</td></tr>\r\n";
  snprintf(timeString, sizeof(timeString) / sizeof(timeString[0]), "%02d:%02d:%02d", networkTime.hours, networkTime.minutes, networkTime.seconds);
  resp        += String("<tr><td>Network time: </td><td>") + timeString + "</td></tr>\r\n"
                 + "<tr><td>Clock rate: </td><td>" + ourTime.rate10 / 10.0 + "</td></tr>\r\n"
                 + "<tr><td colspan=2><a href=\"/\">Back to main configuration page</a></td></tr></table>\r\n"
                 + "</body></html>";
  server.send(200, "text/html", resp);
}

void restartESP()
{
  ESP.restart();
}

void initWiFi(void)
{
  server.on("/", writeMainPage);
  server.on("/clock.html", writeClockPage);
  server.on("/status.html", writeStatusPage);
  server.on("/restart.html", restartESP);
  server.onNotFound(writeMainPage);

  updater.setup(&server);

  // start configuration webserver
  server.begin();
}

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
      wifiMulti.run();
      break;
  }
}

/**
 * Stop listening on <throttleName>.local, start listening on config.local
 */
void initWiFiConfigSTA(void)
{
  MDNS.removeService(NULL, "http", "tcp");
  MDNS.setHostname("config");
  MDNS.addService("http", "tcp", 80);
}

/**
 * Stop listening on (MDNS) config.local, start listening on <throttleName>.local
 */
void shutdownWiFiConfigSTA(void)
{
  char * hostName = strdup(throttleName);
  
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
  WiFi.mode(WIFI_STA);

  for(std::vector<wifiAPEntry>::iterator it = apList.begin() ; it != apList.end(); it++)
  {
    wifiMulti.addAP(it->ssid, it->key);
  }
}

void initMDNS(void)
{
  char * hostName = strdup(throttleName);
  
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
    if(strcmp(newAP.ssid, "") == 0)
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
    for(std::vector<wifiAPEntry>::iterator it = apList.begin() ; it != apList.end(); )
    {
      if(strcmp(it->ssid, server.arg("remove").c_str()) == 0)
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

  // check if this is a "set clock server" request
  if (server.hasArg("clock.serverName") && server.hasArg("clock.serverPort"))
  {
    free(clockServer.name);
    clockServer.name = strdup(server.arg("clock.serverName").c_str());
    clockServer.port = server.arg("clock.serverPort").toInt();
    clockServer.automatic = server.hasArg("clock.automatic");

    if(!clockServer.automatic)
    {
      free(automaticServer);
      automaticServer = nullptr;
    }

    saveClockServer();
  }

  // check if this is a "set clock startup time" request
  if (server.hasArg("clock.startUp") && server.hasArg("clock.startupRate"))
  {
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
    startupTime.rate10 = (uint8_t) 10 * server.arg("clock.startupRate").toFloat();
    if (startupTime.rate10 > 10 * clockMaxRate)
    {
      startupTime.rate10 = 10 * clockMaxRate;
    }

    saveClockStartup();
  }

  // check if this is a "set clock startup time" request
  if (server.hasArg("clock.offset") && server.hasArg("clock.maxClockRate") && server.hasArg("clock.pulseLength"))
  {
    clockOffset = server.arg("clock.offset").toInt();
    clockMaxRate = server.arg("clock.maxClockRate").toInt();
    clockPulseLength = server.arg("clock.pulseLength").toInt();

    saveClockConfig();
  }

  char timeString[9];

  String resp = String("<!DOCTYPE HTML>\r\n")
                + "<html><head><title>wiClock configuration page</title></head>\r\n"
                + "<body><h1>wiClock configuration page</h1>\r\n"
                + "<hr>General configuration<hr>\r\n"
                + "<form action=\"index.html\" method=\"get\"><table border=0>"
                + "<tr><td>Throttle name:</td><td><input type=\"text\" name=\"throttleName\" value=\"" + throttleName + "\"></td></tr>"
                + "<tr><td colspan=2><input type=\"submit\" value=\"Save name\"></td></tr></table></form>\r\n"
                + "<hr>WiFi configuration<hr>\r\n"
                + "<table border=0><tr><td>Active WiFi network SSID:</td><td>" + (WiFi.isConnected() ? WiFi.SSID() : "not connected") + "</td><td><a href=scanWifi.html>Scan for networks</a></td></tr>"
                + "<tr><td colspan=3>Known WiFi networks:</td></tr>";
  for(std::vector<wifiAPEntry>::iterator it = apList.begin() ; it != apList.end(); ++it)
  {
    resp += String("<tr><td>SSID: ") + it->ssid + "</td><td>PSK: " + it->key + "</td>"
          + "<td><form action=\"index.html\" method=\"get\"><input type=\"hidden\" name=\"remove\" value=\"" + it->ssid + "\"><input type=\"submit\" value=\"Remove SSID\"></form></td></tr>\r\n";
  }

  resp        += String("<form action=\"index.html\" method=\"get\"><tr>")
                 + "<td>New SSID: <input type=\"text\" name=\"wifiSSID\"></td>"
                 + "<td>New PSK: <input type=\"text\" name=\"wifiKEY\"></td>"
                 + "<td><input type = \"submit\" value=\"Manually add network\"></td>"
                 +  "</tr></form></table>\r\n";

  resp        += String("<a href=restart.html>Restart wiClock to enable new WiFi settings</a>\r\n")
                 + " WiFi settings will not be active until restart.\r\n";

  snprintf(timeString, sizeof(timeString) / sizeof(timeString[0]), "%02d:%02d:%02d", startupTime.hours, startupTime.minutes, startupTime.seconds);

  resp        += String("<hr>Clock configuration<hr>")
                 + "<table border=0><form action=\"index.html\" method=\"get\">"
                 + "<tr><td>Clock server and port: </td>"
                 + "<td>http://<input type=\"text\" name=\"clock.serverName\" value=\"" + clockServer.name + "\">:<input type=\"text\" name=\"clock.serverPort\" value=\"" + clockServer.port + "\">/json/time</td></tr>"
                 + "<tr><td>Find server automatically through Zeroconf/Bonjour?</td><td><input type=\"checkbox\" name=\"clock.automatic\"" + (clockServer.automatic ? " checked" : "") + ">"
                 + "Using http://" + (clockServer.automatic && automaticServer != nullptr ? automaticServer : clockServer.name) + ":" + clockServer.port + "/json/time</td></tr>"
                 + "<tr><td colspan=2><input type=\"submit\" value=\"Save clock server settings\"</td></tr></form>"
                 + "<form action=\"index.html\" method=\"get\">"
                 + "<tr><td>Startup time (format: H:M:S):</td><td><input type=\"text\" name=\"clock.startUp\" value=\"" + timeString + "\"></td></tr>"
                 + "<tr><td>Startup clock rate:</td><td><input type=\"text\" name=\"clock.startupRate\" value=\"" + startupTime.rate10 / 10.0 + "\"></td></tr>"
                 + "<tr><td colspan=2><input type=\"submit\" value=\"Save startup time and rate\"</td></tr></form>"
                 + "<form action=\"index.html\" method=\"get\">"
                 + "<tr><td>Clock offset from UTC (hours):</td><td><input type=\"text\" name=\"clock.offset\" value=\"" + clockOffset + "\"></td></tr>"
                 + "<tr><td>Maximum clock rate:</td><td><input type=\"text\" name=\"clock.maxClockRate\" value=\"" + clockMaxRate + "\"></td></tr>"
                 + "<tr><td>Pulse length for clock (milliseconds):</td><td><input type=\"text\" name=\"clock.pulseLength\" value=\"" + clockPulseLength + "\"></td></tr>"
                 + "<tr><td conspan=2><input type=\"submit\" value=\"Save clock configuration\"></td></tr></table></form>\r\n";

  snprintf(timeString, sizeof(timeString) / sizeof(timeString[0]), "%02d:%02d:%02d", ourTime[0].hours, ourTime[0].minutes, ourTime[0].seconds);
  resp        += String("<hr>wiClock status<hr>\r\n")
                 + "<table border=0>"
                 + "<tr><td>Battery voltage: </td><td>" + batteryVoltage + " mV" + (lowBattery ? " Battery LOW" : "" ) + "</td></tr>\r\n"
                 + "<tr><td>System time: </td><td>" + timeString + "</td></tr>\r\n";
  
  snprintf(timeString, sizeof(timeString) / sizeof(timeString[0]), "%02d:%02d:%02d", networkTime.hours, networkTime.minutes, networkTime.seconds);
  resp        += String("<tr><td>Network time: </td><td>") + timeString + "</td></tr>\r\n"
                 + "<tr><td>Clock rate: </td><td>" + ourTime[0].rate10 / 10.0 + "</td></tr></table>\r\n"
                 + "<hr>wiClock system<hr>\r\n"
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

  if(n == 0)
  {
    resp += String("<tr><td>No WiFi networks found. Reload to repeat scan.</td></tr>");
  }
  
  for(uint32_t i = 0; i < n; i++)
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
  if(server.hasArg("reallyReset"))
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

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
 * This file provides functions for handling the internal clock and driving the external clock.
 */

#include <string.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>

#include "clockHandling.h"
#include "config.h"
#include "lowbat.h"
#include "stateMachine.h"
#include "ui.h"

uint8_t clockPulseLength = 40;
uint8_t clockMaxRate = 10;
int8_t clockOffset;

clockInfo ourTime[NUM_CLOCKS];
clockInfo networkTime;
clockInfo startupTime;
serverInfo clockServer;
char * automaticServer;
IPAddress automaticServerIP;

volatile bool flagGetTime = true;
volatile bool flagNewTime = false;

Ticker realSecond;

void plusOneSecond(clockInfo * input)
{
  input->seconds++;
  if(input->seconds >= 60)
  {
    input->seconds = 0;
    input->minutes++;
  }
  if(input->minutes >= 60)
  {
    input->minutes = 0;
    input->hours++;
  }
  if(input->hours >= 12)
  {
    input->hours = 0;
  }
}

void resetClockOutputs(clockInfo * clockID)
{
  digitalWrite(clockID->pin1, HIGH);
  digitalWrite(clockID->pin2, HIGH);
}

void secondTickHandler(void)
{
  flagGetTime = true;
}

void setClockOutputs(clockInfo * clockID)
{
  clockID->edgeCounter++;
  clockID->edgeCounter %= 4;
  if(clockID->edgeCounter == 0)
  {
    digitalWrite(clockID->pin1, LOW);
    clockID->resetTicker-> once_ms(clockPulseLength, resetClockOutputs, clockID);
  }
  else if( (clockID->edgeCounter == 2 && !lowBattery) || (clockID->edgeCounter == 3 && lowBattery) )
  {
    digitalWrite(clockID->pin2, LOW);
    clockID->resetTicker->once_ms(clockPulseLength, resetClockOutputs, clockID);
  }
  if(clockID->edgeCounter % 2 == 0)
  {
    plusOneSecond(clockID);
    flagNewTime = true;
  }
}

void networkSecondHandler(void)
{
  plusOneSecond(&networkTime);
  flagNewTime = true;
}

void initClock(void)
{
  for(uint8_t i=0; i<NUM_CLOCKS; i++)
  {
    memcpy(&ourTime[i], &startupTime, sizeof(clockInfo));
    ourTime[i].secondTicker = new Ticker();
    ourTime[i].resetTicker = new Ticker();
    ourTime[i].hours %= 12;
  }

  ourTime[0].pin1 = CLOCK1_PIN;
  ourTime[0].pin2 = CLOCK2_PIN;
#ifdef CLOCK3_PIN
  ourTime[1].pin1 = CLOCK3_PIN;
  ourTime[1].pin2 = CLOCK4_PIN;
#endif

  memcpy(&networkTime, &startupTime, sizeof(clockInfo));
  networkTime.hours %= 12;
  networkTime.secondTicker = new Ticker();

  for(uint8_t i=0; i<NUM_CLOCKS; i++)
  {
    pinMode(ourTime[i].pin1, OUTPUT);
    pinMode(ourTime[i].pin2, OUTPUT);
    digitalWrite(ourTime[i].pin1, HIGH);
    digitalWrite(ourTime[i].pin2, HIGH);
  }
  
  realSecond.attach(1.0, secondTickHandler);

  for(uint8_t i=0; i<NUM_CLOCKS; i++)
  {
    if(ourTime[i].rate10 > 10 * clockMaxRate)
    {
      ourTime[i].rate10 = 10 * clockMaxRate;
    }

    if(ourTime[i].rate10 != 0)
    {
      ourTime[i].secondTicker->attach(10.0 / ourTime[i].rate10 / 2, setClockOutputs, &ourTime[i]);
    }
    else
    {
      ourTime[i].secondTicker->detach();
    }

    if(networkTime.rate10 != 0)
    {
      networkTime.secondTicker->attach(10.0 / networkTime.rate10, networkSecondHandler);
    }
    else
    {
      networkTime.secondTicker->detach();
    }
  }
}

void clockHandler(void)
{
    // try to get time from network if we are connected to WLAN
    if(flagGetTime && wiFredState == STATE_CONNECTED)
    {
      static WiFiClient client;

      if(client.available())
      {
          String line = client.readStringUntil('\n');
          // is this the data line we are looking for?
          if(line.indexOf("{\"type\":\"time\"") != -1)
          {
            // turn off LED at first nicely received response
            setLED(0, 100);
            flagGetTime = false;

            client.flush();

            clockInfo temp;
            size_t pos;
            pos = line.indexOf("T");
            temp.hours = line.substring(pos + 1).toInt();
            temp.minutes = line.substring(pos + 4).toInt();
            temp.seconds = line.substring(pos + 7).toInt();

            pos = line.indexOf("rate");
            temp.rate10 = (uint8_t) (line.substring(pos + sizeof("rate") + 1).toFloat() * 10);

            pos = line.indexOf("state");
            uint8_t state = line.substring(pos + sizeof("state") + 1).toInt();
            if(state == 4)
            {
              temp.rate10 = 0;
            }

            if(temp.hours < 24 && temp.minutes < 60 && temp.seconds < 60)
            {
              temp.hours += clockOffset;
              temp.hours %= 12;
              // only change networkSecond timer if rate has changed to avoid glitches
              if(temp.rate10 != networkTime.rate10)
              {
                if(temp.rate10 != 0)
                {
                  networkTime.secondTicker->attach(10.0 / temp.rate10, networkSecondHandler);
                }
                else
                {
                  networkTime.secondTicker->detach();
                }
              }
              networkTime.seconds = temp.seconds;
              networkTime.minutes = temp.minutes;
              networkTime.hours = temp.hours;
              networkTime.rate10 = temp.rate10;
              flagNewTime = true;
            }
          }
      }
      else if(client.connected())
      {
        client.setNoDelay(true);
        client.setTimeout(10);
        client.print(String("GET /json/time") + " HTTP/1.1\r\n" +
             "Host: " + (clockServer.automatic && automaticServer != nullptr ? automaticServer : clockServer.name) + "\r\n" +
             "Connection: close\r\n" +
             "\r\n"
            );
      }
      else
      {
        if(clockServer.automatic && automaticServer != nullptr)
        {
#ifdef DEBUG
          Serial.println("Trying to connect to automatic server...");
#endif
          client.connect(automaticServerIP, clockServer.port);
        }
        else if(!clockServer.automatic)
        {
          client.connect(clockServer.name, clockServer.port);
        }
      }
    }

    if(clockServer.automatic && automaticServer == nullptr && flagGetTime 
      && (wiFredState == STATE_CONNECTED || wiFredState == STATE_CONFIG_AP) )
    {
#ifdef DEBUG
      Serial.println("Looking for automatic server");
      Serial.println("Installing service query");
#endif
      uint32_t n = MDNS.queryService("http", "tcp");
      for(uint32_t i = 0; i < n; i++)
      {
#ifdef DEBUG
        Serial.print(String("Hostname: ") + MDNS.hostname(i) + " IP ");
        Serial.print(MDNS.IP(i));
        Serial.println(String(" port ") + MDNS.port(i));
#endif
        if(MDNS.port(i) == clockServer.port)
        {
          automaticServer = strdup(MDNS.hostname(i).c_str());
          automaticServerIP = MDNS.IP(i);
          MDNS.removeQuery();
          break;          
        }
      }
      flagGetTime = false;
    }
    
    if(flagNewTime)
    {
      // reset flag
      flagNewTime = false;

      for(uint8_t i=0; i<NUM_CLOCKS; i++)
      {
        // keep old rate in case it is not changed
        uint8_t oldRate = ourTime[i].rate10;
        // calculate network-local time difference and re-adjust local clock rate if required
        int32_t delta = networkTime.hours * 3600 + networkTime.minutes * 60 + networkTime.seconds 
                      - (ourTime[i].hours * 3600 + ourTime[i].minutes * 60 + ourTime[i].seconds);
        // roll over after twelve hours and make sure it's inside [0...12 hours]
        delta %= (12 * 3600);
        if(delta < 0)
        {
          delta += 12 * 3600;
        }

        // simple cases first
        // if we are already at same speed and close enough, keep doing it
        if( (delta < CLOCK_DELTA || delta > 12 * 3600 - CLOCK_DELTA) && networkTime.rate10 == ourTime[i].rate10)
        {
          // do nothing
        }
        // if we are into a smaller window, set same rate (if we can run as fast)
        else if( (delta < CLOCK_DELTA / 2 || delta > 12 * 3600 - CLOCK_DELTA / 2) && networkTime.rate10 <= clockMaxRate * 10)
        {
          ourTime[i].rate10 = networkTime.rate10;
        }
        // otherwise check if it will be faster to wait for network time or to play catch-up
        
        // network clock not moving, so we need to catch up
        else if(networkTime.rate10 == 0)
        {
          ourTime[i].rate10 = clockMaxRate * 10;
        }
        // network clock faster or same than we can run at all, so stop clock
        // (if same, will start running again once network clock has caught up)
        else if(networkTime.rate10 >= clockMaxRate * 10)
        {
          ourTime[i].rate10 = 0;
        }
        else
        {
          uint8_t maxRateDelta = clockMaxRate * 10 - networkTime.rate10;
          uint16_t catchupTime = delta / maxRateDelta;
          uint16_t waitTime = (12 * 3600 - delta) / networkTime.rate10;
          if(waitTime <= catchupTime)
          {
            ourTime[i].rate10 = 0;
          }
          else
          {
            ourTime[i].rate10 = clockMaxRate * 10;
          }
        }

        if(ourTime[i].rate10 != oldRate)
        {
          if(ourTime[i].rate10 != 0)
          {
            ourTime[i].secondTicker->attach(10.0 / ourTime[i].rate10 / 2, setClockOutputs, &ourTime[i]);
          }
          else
          {
            ourTime[i].secondTicker->detach();
          }
        }
      }
  }
}

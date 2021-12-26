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

clockTime ourTime[NUM_CLOCKS];
clockInfo clockHW[NUM_CLOCKS];
clockTime networkTime;
clockTime startupTime[NUM_CLOCKS];

int8_t clockOffset;

serverInfo clockServer;
char * automaticServer;
IPAddress automaticServerIP;

volatile bool flagGetTime = true;
volatile bool flagNewTime = false;

Ticker realSecond;

void plusOneSecond(clockTime * input)
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
  flagNewTime = true;
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
  if(clockID->nextEdgeHigh)
  {
    digitalWrite(clockID->pin1, LOW);
  }
  else
  {
    digitalWrite(clockID->pin2, LOW);
  }
  clockID->nextEdgeHigh = !clockID->nextEdgeHigh;
  clockID->resetTicker-> once_ms(clockID->clockPulseLength, resetClockOutputs, clockID);
}

void everyHardwareHalfSecond(uint8_t clockID)
{
  clockHW[clockID].tickCounter++;
  if(clockHW[clockID].minuteMode)
  {
    clockHW[clockID].tickCounter %= 2;
    if(clockHW[clockID].tickCounter == 0)
    {
      plusOneSecond(&ourTime[clockID]);
      if(ourTime[clockID].seconds == 0)
      {
        setClockOutputs(&clockHW[clockID]);
      }
    }
  }
  else
  {
    clockHW[clockID].tickCounter %= 4;
    if(  (clockHW[clockID].tickCounter == 0)
      || (clockHW[clockID].tickCounter == 2 && !lowBattery)
      || (clockHW[clockID].tickCounter == 3 && lowBattery) )
    {
      plusOneSecond(&ourTime[clockID]);
      setClockOutputs(&clockHW[clockID]);
    }
  }
}

void setClockRate(uint8_t i)
{ 
  if(ourTime[i].rate10 > 10 * clockHW[i].clockMaxTickFrequency && !clockHW[i].minuteMode)
  {
    ourTime[i].rate10 = 10 * clockHW[i].clockMaxTickFrequency;
  }

  if(ourTime[i].rate10 > 10 * 60 * clockHW[i].clockMaxTickFrequency && clockHW[i].minuteMode)
  {
    ourTime[i].rate10 = 10 * 60 * clockHW[i].clockMaxTickFrequency;
  }

  if(ourTime[i].rate10 != 0)
  {
    ourTime[i].secondTicker->attach(10.0 / ourTime[i].rate10 / 2, everyHardwareHalfSecond, i);
  }
  else
  {
    ourTime[i].secondTicker->detach();
  }
}

void setClockRate(void)
{
  for(uint8_t i=0; i<NUM_CLOCKS; i++)
  {
    setClockRate(i);
  }
}

void initClock(void)
{
  for(uint8_t i=0; i<NUM_CLOCKS; i++)
  {
    memcpy(&ourTime[i], &startupTime, sizeof(clockTime));
    ourTime[i].secondTicker = new Ticker();
    ourTime[i].hours %= 12;

    clockHW[i].resetTicker = new Ticker();
  }

  clockHW[0].pin1 = CLOCK1_PIN;
  clockHW[0].pin2 = CLOCK2_PIN;
#ifdef CLOCK3_PIN
  clockHW[1].pin1 = CLOCK3_PIN;
  clockHW[1].pin2 = CLOCK4_PIN;
#endif

  for(uint8_t i=0; i<NUM_CLOCKS; i++)
  {
    pinMode(clockHW[i].pin1, OUTPUT);
    pinMode(clockHW[i].pin2, OUTPUT);
    digitalWrite(clockHW[i].pin1, HIGH);
    digitalWrite(clockHW[i].pin2, HIGH);
  }
  
  setClockRate();

  memcpy(&networkTime, &startupTime, sizeof(clockTime));
  networkTime.hours %= 12;
  networkTime.secondTicker = new Ticker();

  if(networkTime.rate10 != 0)
  {
    networkTime.secondTicker->attach(10.0 / networkTime.rate10, plusOneSecond, &networkTime);
  }
  else
  {
    networkTime.secondTicker->detach();
  }

  realSecond.attach(1.0, secondTickHandler);
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

            clockTime temp;
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
                  networkTime.secondTicker->attach(10.0 / temp.rate10, plusOneSecond, &networkTime);
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

    if(getInputPressed(KEY_MINUTE))
    {
      for(uint8_t i=0; i<NUM_CLOCKS; i++)
      {
        ourTime[i].minutes--;
        while(ourTime[i].minutes >= 60)
        {
          ourTime[i].minutes += 60;
          ourTime[i].hours--;
          while(ourTime[i].hours >= 12)
          {
            ourTime[i].hours += 12;    
          }
        }
      }
      flagNewTime = true;
    }
    
    if(getInputPressed(KEY_HOUR))
    {
      for(uint8_t i=0; i<NUM_CLOCKS; i++)
      {
        ourTime[i].hours--;
        while(ourTime[i].hours >= 12)
        {
          ourTime[i].hours += 12;    
        }
      }
      flagNewTime = true;
    }

    if(flagNewTime)
    {
      // reset flag
      flagNewTime = false;

      for(uint8_t i=0; i<NUM_CLOCKS; i++)
      {
        // keep old rate in case it is not changed
        uint16_t oldRate = ourTime[i].rate10;
        // calculate network-local time difference and re-adjust local clock rate if required
        int32_t delta = networkTime.hours * 3600 + networkTime.minutes * 60 + networkTime.seconds 
                      - (ourTime[i].hours * 3600 + ourTime[i].minutes * 60 + ourTime[i].seconds);
        // roll over after twelve hours and make sure it's inside [0...12 hours]
        delta %= (12 * 3600);
        if(delta < 0)
        {
          delta += 12 * 3600;
        }

        uint32_t maxDelta = CLOCK_DELTA;
        uint16_t clockMaxRate = clockHW[i].clockMaxTickFrequency;

        // in minute mode, use one second window only and adapt maximum clock rate accordingly
        if(clockHW[i].minuteMode)
        {
          clockMaxRate *= 60;
        }

        // simple cases first
        // if we are already at same speed and close enough, keep doing it
        if( (delta < maxDelta || delta > 12 * 3600 - maxDelta) && networkTime.rate10 == ourTime[i].rate10)
        {
          // do nothing
        }
        // if we are into a smaller window, set same rate (if we can run as fast)
        else if( (delta < maxDelta || delta > 12 * 3600 - maxDelta) && networkTime.rate10 <= clockMaxRate * 10)
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
          uint16_t maxRateDelta = clockMaxRate * 10 - networkTime.rate10;
          uint16_t catchupTime = delta / maxRateDelta;
          uint16_t waitTime = (12 * 3600 - delta) / networkTime.rate10;
          if(waitTime <= catchupTime)
          {
            ourTime[i].rate10 = 0;
          }
          else if(5 * delta + networkTime.rate10 < clockMaxRate * 10)
          {
            ourTime[i].rate10 = networkTime.rate10 + 5 * delta;
          }
          else
          {
            ourTime[i].rate10 = clockMaxRate * 10;
          }
        }

        if(ourTime[i].rate10 != oldRate)
        {
          setClockRate(i);
        }
      }
  }
}

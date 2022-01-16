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

serverInfo clockServer;
char * automaticServer;
IPAddress automaticServerIP;

WiFiClient client;

volatile bool flagNewTime = false;

void setClockOutputs(clockInfo * clockID);

void resetClockOutputs(clockInfo * clockID)
{
  digitalWrite(clockID->pin1, HIGH);
  digitalWrite(clockID->pin2, HIGH);
  if(clockID->tickPerIncrement > 0)
  {
    clockID->tickPerIncrement--;
    clockID->resetTicker->once_ms(clockID->clockPulseLength, setClockOutputs, clockID);
  }
}

void setClockOutputs(clockInfo * clockID)
{
  if (clockID->nextEdgeHigh)
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

void plusOneSecond(clockTime * input)
{
  input->seconds++;
  if (input->seconds >= 60)
  {
    input->seconds = 0;
    input->minutes++;
  }
  if (input->minutes >= 60)
  {
    input->minutes = 0;
    input->hours++;
  }
  if (input->hours >= 12)
  {
    input->hours = 0;
  }
  flagNewTime = true;
}

void everyHardwareHalfSecond(uint8_t clockID)
{
  clockHW[clockID].tickCounter++;
  if(clockHW[clockID].clockMode == CLOCK_MODE_SECOND)
  {
    clockHW[clockID].tickCounter %= 4;
    if (  (clockHW[clockID].tickCounter == 0)
          || (clockHW[clockID].tickCounter == 2 && !lowBattery)
          || (clockHW[clockID].tickCounter == 3 && lowBattery) )
    {
      plusOneSecond(&ourTime[clockID]);
      setClockOutputs(&clockHW[clockID]);
    }
  }
  else
  {
    clockHW[clockID].tickCounter %= 2;
    if (clockHW[clockID].tickCounter == 0)
    {
      plusOneSecond(&ourTime[clockID]);
      if(clockHW[clockID].clockMode == CLOCK_MODE_SWEEP16SECOND)
      {
        clockHW[clockID].tickPerIncrement = 16 - 1;
        setClockOutputs(&clockHW[clockID]);
      }
      else if(ourTime[clockID].seconds == 0)
      {
        if(clockHW[clockID].clockMode == CLOCK_MODE_SWEEP16MINUTE)
        {
          clockHW[clockID].tickPerIncrement = 16 - 1;
        }
        setClockOutputs(&clockHW[clockID]);
      }
    }
  }
}

/**
 * If clock rate is larger than maximum, limit to maximum
 */
void limitClockRate(clockTime * c, uint16_t maximum)
{
  if(c->rate10 > maximum)
  {
    c->rate10 = maximum;
  }
}

void setClockRate(uint8_t i)
{
  switch(clockHW[i].clockMode)
  {
    case CLOCK_MODE_SECOND:
      limitClockRate(&(ourTime[i]), 10 * clockHW[i].clockMaxTickFrequency);
      break;

    case CLOCK_MODE_MINUTE:
      limitClockRate(&(ourTime[i]), 10 * 60 * clockHW[i].clockMaxTickFrequency);
      break;

    case CLOCK_MODE_SWEEP16SECOND:
      limitClockRate(&(ourTime[i]), 10 * clockHW[i].clockMaxTickFrequency / 16);
      break;

    case CLOCK_MODE_SWEEP16MINUTE:
      limitClockRate(&(ourTime[i]), 10 * 60 * clockHW[i].clockMaxTickFrequency / 16);
      break;
    }

  // absolute clock rate limit is 5000 -> 1ms ticker interval @ two ticks per second 
  limitClockRate(&(ourTime[i]), 5000);

  // for sweeping second clock, higher ticker rate is required
  if(clockHW[i].clockMode == CLOCK_MODE_SWEEP16SECOND)
  {
    limitClockRate(&(ourTime[i]), 5000 / 16);
  }
  
  if (ourTime[i].rate10 != 0)
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
  for (uint8_t i = 0; i < NUM_CLOCKS; i++)
  {
    setClockRate(i);
  }
}

/**
 * Handles new time and key presses
 */
void handleNewTime(void)
{
  if (getInputPressed(KEY_MINUTE))
  {
    for (uint8_t i = 0; i < NUM_CLOCKS; i++)
    {
      ourTime[i].minutes--;
      while (ourTime[i].minutes >= 60)
      {
        ourTime[i].minutes += 60;
        ourTime[i].hours--;
        while (ourTime[i].hours >= 12)
        {
          ourTime[i].hours += 12;
        }
      }
    }
    flagNewTime = true;
  }

  if (getInputPressed(KEY_HOUR))
  {
    for (uint8_t i = 0; i < NUM_CLOCKS; i++)
    {
      ourTime[i].hours--;
      while (ourTime[i].hours >= 12)
      {
        ourTime[i].hours += 12;
      }
    }
    flagNewTime = true;
  }

  if (flagNewTime)
  {
    // reset flag
    flagNewTime = false;

    for (uint8_t i = 0; i < NUM_CLOCKS; i++)
    {
      // keep old rate in case it is not changed
      uint16_t oldRate = ourTime[i].rate10;
      // calculate network-local time difference and re-adjust local clock rate if required
      int32_t delta = networkTime.hours * 3600 + networkTime.minutes * 60 + networkTime.seconds
                      - (ourTime[i].hours * 3600 + ourTime[i].minutes * 60 + ourTime[i].seconds);
      // roll over after twelve hours and make sure it's inside [0...12 hours]
      delta %= (12 * 3600);
      if (delta < 0)
      {
        delta += 12 * 3600;
      }

      uint32_t maxDelta = CLOCK_DELTA;
      uint16_t clockMaxRate;
      
      switch(clockHW[i].clockMode)
      {
        case CLOCK_MODE_SECOND:
        default:
          clockMaxRate = clockHW[i].clockMaxTickFrequency;
          break;

        case CLOCK_MODE_MINUTE:
          clockMaxRate = clockHW[i].clockMaxTickFrequency * 60;
          break;

        case CLOCK_MODE_SWEEP16SECOND:
          clockMaxRate = clockHW[i].clockMaxTickFrequency / 16.0;
          break;

        case CLOCK_MODE_SWEEP16MINUTE:
          clockMaxRate = clockHW[i].clockMaxTickFrequency * 60 / 16.0;
          break;
      }

      // simple cases first
      // if we are already at same speed and close enough, keep doing it
      if ( (delta == 0 || delta > 12 * 3600 - maxDelta) && networkTime.rate10 == ourTime[i].rate10)
      {
        // do nothing
      }
      // if we are into a smaller window, set same rate (if we can run as fast)
      else if ( (delta == 0 || delta > 12 * 3600 - maxDelta) && networkTime.rate10 <= clockMaxRate * 10)
      {
        ourTime[i].rate10 = networkTime.rate10;
      }
      
      // otherwise check if it will be faster to wait for network time or to play catch-up

      // network clock not moving, so we need to catch up
      else if (networkTime.rate10 == 0)
      {
        // unless we are only a little bit ahead
        if(delta > 12 * 3600 - 2 * maxDelta)
        {
          ourTime[i].rate10 = 0;
        }
        else
        {
          ourTime[i].rate10 = clockMaxRate * 10;
        }
      }
      // network clock faster or same than we can run at all, so stop clock
      // (if same, will start running again once network clock has caught up)
      else if (networkTime.rate10 >= clockMaxRate * 10)
      {
        ourTime[i].rate10 = 0;
      }
      else
      {
        uint16_t maxRateDelta = clockMaxRate * 10 - networkTime.rate10;
        uint16_t catchupTime = delta / maxRateDelta;
        uint16_t waitTime = (12 * 3600 - delta) / networkTime.rate10;
        if (waitTime <= catchupTime)
        {
          ourTime[i].rate10 = 0;
        }
        else if (5 * delta + networkTime.rate10 < clockMaxRate * 10)
        {
          ourTime[i].rate10 = networkTime.rate10 + 5 * delta;
        }
        else
        {
          ourTime[i].rate10 = clockMaxRate * 10;
        }
      }

      if (ourTime[i].rate10 != oldRate)
      {
        setClockRate(i);
      }
    }
  }
}

void initClock(void)
{
  for (uint8_t i = 0; i < NUM_CLOCKS; i++)
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

  for (uint8_t i = 0; i < NUM_CLOCKS; i++)
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

  if (networkTime.rate10 != 0)
  {
    networkTime.secondTicker->attach(10.0 / networkTime.rate10, plusOneSecond, &networkTime);
  }
  else
  {
    networkTime.secondTicker->detach();
  }
}

/**
 * Connect to wiThrottle server
 */
void clockConnect(void)
{
  if(clockServer.automatic && automaticServer != nullptr)
  {
    if(client.connect(automaticServerIP, clockServer.port))
    {
      client.setNoDelay(true);
      client.setTimeout(10);
      switchState(STATE_SERVERCONN, 10 * 1000);
    }
    else
    {
      free(automaticServer);
      automaticServer = nullptr;
    }
  }
  else if(!clockServer.automatic)
  {
    if(client.connect(clockServer.name, clockServer.port))
    {
      client.setNoDelay(true);
      client.setTimeout(10);
      switchState(STATE_SERVERCONN, 10 * 1000);
    }
  }

  if(clockServer.automatic && automaticServer == nullptr
     && (wiFredState == STATE_CONNECTED || wiFredState == STATE_CONFIG_AP) )
  {
    uint32_t n = MDNS.queryService("withrottle", "tcp");
    for(uint32_t i = 0; i < n; i++)
    {
      if(MDNS.port(i) == clockServer.port)
      {
        automaticServer = strdup(MDNS.hostname(i).c_str());
        automaticServerIP = MDNS.IP(i);
        break;          
      }
    }
    if(n == 0)
    {
      automaticServerIP = WiFi.localIP();
      automaticServerIP[3] = 1;
      automaticServer = strdup(automaticServerIP.toString().c_str());
    }
  }
}

/**
 * Initialize connection to wiThrottle server with client ID etc. after receiving the greeting message
 */
void clockServerRegister(void)
{
  if(client.connected())
  {
    client.print(String("N") + throttleName + "\n");
  
    if(client.available())
    {
      String line = client.readStringUntil('\n');
      if (line.startsWith("VN2.0"))
      {
        uint8_t mac[6];
        WiFi.macAddress(mac);
        String id = String(mac[0], 16) + String(mac[1], 16) + String(mac[2], 16) + String(mac[3], 16) + String(mac[4], 16) + String(mac[5], 16);
        client.print("HU" + id + "\n");
        switchState(STATE_ONLINE);
      }
    }
  }
  else
  {
    setLED(25, 50);
    switchState(STATE_CONNECTED);
  }
}

void clockHandler(void)
{
  while (client.available())
  {
    String line = client.readStringUntil('\n');
    // is this the data line we are looking for?
    if (line.startsWith("PFT"))
    {
      int32_t timeStamp;
      float rate;

      if(sscanf(line.c_str(), "PFT%d<;>%f", &timeStamp, &rate) == 2)
      {
        // turn LED off on response
        setLED(0, 200);
        clockTime temp;

        // convert timeStamp received to hours (0...12), minutes and seconds, ignore date
        timeStamp %= 12 * 60 * 60;
        temp.hours = timeStamp / (60 * 60);
        timeStamp %= 60 * 60;        
        temp.minutes = timeStamp / 60;
        timeStamp %= 60;
        temp.seconds = timeStamp;

        temp.rate10 = rate * 10;

        // only change networkSecond timer if rate has changed to avoid glitches
        if (temp.rate10 != networkTime.rate10)
        {
          if (temp.rate10 != 0)
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
  if(!client.connected())
  {
    setLED(25, 50);
    switchState(STATE_CONNECTED);
  }
}

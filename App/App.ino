//clean up libraries
#include <Arduino.h> //?
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h> //?
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h> //needed?
#include <ArduinoJson.h>
#include "lights.h"
#include "door.h"
#include "endpoints.h"
#include "datetimeCheck.h"

bool coornickTurnedOn = true;

// default action times [15min periods]
int lightsDayDoorOpenPeriod = 7 * 4 + 2;
int lightsNightPeriod = 17 * 4;
int doorClosedPeriod = 20 * 4;
int lightsOffPeriod = 20 * 4 + 2;

int period = 100;
int lastAction = 4;

void setup(void) {
  setupLights();
  setupDoor();

  setupServer();
  setupEndpoint("/lightsDayDoorOpen", []() {
    lightsDayDoorOpen();
  });
  setupEndpoint("/lightsNight", []() {
    lightsNight();
  });
  setupEndpoint("/doorClosed", []() {
    doorClosed();
  });
  setupEndpoint("/lightsOff", []() {
    lightsOff();
  });
  setupEndpoint("/status", []() {
    getCoornickStatus();
  });
  setupEndpoint("/getActionTimes", []() {
    getActionTimes();
  });
  setupEndpoint("/setActionTimes", []() {
    setActionTimes();
  });
  setupEndpoint("/coornickSwitch", []() {
    coornickSwitch();
  });
  setupEndpoint("/getTime", []() {
    datetimeCheck(httpGet, [](int newPeriod) {
      updatePeriod(newPeriod);
    });
  });
  
  lightsOff();
}

void loop(void) {
  loopServer();
  if (coornickTurnedOn) {
    loopDatetimeCheck(httpGet, [](int newPeriod) {
      updatePeriod(newPeriod);
    });
  }
  checkDoorOpened();
  checkDoorClosed();
}

void lightsDayDoorOpen() {
  lastAction = 1;
  unlockDoor();
  turnDayLightsOn();
  delay(500);
  openDoor();
  server.send(200, "text/plain;charset=UTF-8", "Lights: day, door: open.");
}

void lightsNight() {
  lastAction = 2;
  turnNightLightsOn();
  server.send(200, "text/plain", "Lights: night, door: open.");
}

void doorClosed() {
  lastAction = 3;
  closeDoor();
  server.send(200, "text/plain", "Lights: night, door: closed.");
}

void lightsOff() {
  lastAction = 4;
  turnAllLightsOff();
  stopDoor();
  lockDoor();
  server.send(200, "text/plain", "Lights: off, door: closed.");
}

void getCoornickStatus() {
  char buffer[200];
  sprintf(buffer, "{\"dayLights\": %d, \"nightLights\": %d, \"isDoorLocked\": %d, \"isDoorOpening\": %d, \"isDoorClosing\": %d, \"coornickTurnedOn\": %d}", 
    isDayLightOn, isNightLightOn, isDoorLocked, isDoorOpening, isDoorClosing, coornickTurnedOn);
  server.send(200, "application/json", buffer); 
}

void getActionTimes() {
  //TODO
}

void setActionTimes() {
  lightsDayDoorOpenPeriod = server.arg("lightsDayDoorOpen").toInt();
  lightsNightPeriod = server.arg("lightsNight").toInt();
  doorClosedPeriod = server.arg("doorClosed").toInt();
  lightsOffPeriod = server.arg("lightsOff").toInt();
  datetimeCheck(httpGet, [](int newPeriod) {
    updatePeriod(newPeriod);
  });
  server.send(200, "text/plain", "Period times updated.");
}

void coornickSwitch() {
  int previousState = server.arg("previousState").toInt();
  coornickTurnedOn = !previousState;
  if (coornickTurnedOn) {
    server.send(200, "text/plain", "Coornik activated.");
  } else {
    server.send(200, "text/plain", "Coornik deactivated.");
  }
}

void updatePeriod(int newPeriod)
{
  if (newPeriod == 100) {
    return;
  }
  period = newPeriod;
  if(period >= lightsOffPeriod) {
    if(lastAction == 3) lightsOff();
  } else if(period >= doorClosedPeriod) {
    if(lastAction == 2) doorClosed();
  } else if(period >= lightsNightPeriod) {
    if(lastAction == 1) lightsNight();
  } else if(period >= lightsDayDoorOpenPeriod) {
    if(lastAction == 4) lightsDayDoorOpen();
  }
}

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

// default action times [15min periods]
int lightsDayDoorOpenPeriod = 7 * 4 + 2;
int lightsNightPeriod = 17 * 4;
int doorClosedPeriod = 20 * 4;
int lightsOffPeriod = 20 * 4 + 2;

int period = 100;
int lastAction = 4;

void lightsDayDoorOpen();
void lightsNight();
void doorClosed();
void lightsOff();
void getCoornickStatus();

void setup(void) {
  pinMode(LIGHTS_DAY_RELAY_PIN, OUTPUT);
  pinMode(LIGHTS_NIGHT_RELAY_PIN, OUTPUT);
  pinMode(LOCKER_RELAY_PIN, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(SENSOR_DOOR_OPEN, INPUT);
  pinMode(SENSOR_DOOR_CLOSED, INPUT);

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
  setupEndpoint("/getTime", []() {
    datetimeCheck(httpGet, [](int newPeriod) {
      updatePeriod(newPeriod);
    });
  });
  
  lightsOff();
}

void loop(void) {
  loopServer();
  loopDatetimeCheck(httpGet, [](int newPeriod) {
    updatePeriod(newPeriod);
  });
  checkDoorOpened();
  checkDoorClosed();
}

void lightsDayDoorOpen() {
  lastAction = 1;
  Serial.println("Lights: day, door: open.");
  unlockDoor();
  turnDayLightsOn();
  server.send(200, "text/plain;charset=UTF-8", "Dzień dobry, kurki!");
  delay(500);
  openDoor();
}

void lightsNight() {
  lastAction = 2;
  Serial.println("Lights: night, door: open.");
  turnNightLightsOn();
  server.send(200, "text/plain", "Kurki, kurki do domu!");
}

void doorClosed() {
  lastAction = 3;
  Serial.println("Lights: night, door: closed.");
  closeDoor();
  server.send(200, "text/plain", "Zamykamy drzwiczki!");
}

void lightsOff() {
  lastAction = 4;
  Serial.println("Lights: off, door: closed.");
  turnAllLightsOff();
  stopDoor();
  lockDoor();
  server.send(200, "text/plain", "Dobranoc, kurki!");
}

void getCoornickStatus() {
  char buffer[100];
  sprintf(buffer, "{\"dayLights\": %d, \"nightLights\": %d, \"isDoorLocked\": %d, \"isDoorOpening\": %d, \"isDoorClosing\": %d}", 
    isDayLightOn, isNightLightOn, isDoorLocked, isDoorOpening, isDoorClosing);
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
  updatePeriod(period);
  server.send(200, "text/plain", "Godziny zmienione");
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

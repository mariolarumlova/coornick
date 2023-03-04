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

const int LIGHTS_DAY_DOOR_OPEN_PERIOD = 7 * 4 + 2; // action 1
const int LIGHTS_NIGHT_PERIOD = 17 * 4; // action 2
const int DOOR_CLOSED_PERIOD = 20 * 4; // action 3
const int LIGHTS_OFF_PERIOD = 20 * 4 + 2; // action 4
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

void updatePeriod(int newPeriod)
{
  if (newPeriod == 100) {
    return;
  }
  period = newPeriod;
  if(period >= LIGHTS_OFF_PERIOD) {
    if(lastAction != 4) lightsOff();
  } else if(period >= DOOR_CLOSED_PERIOD) {
    if(lastAction != 3) doorClosed();
  } else if(period >= LIGHTS_NIGHT_PERIOD) {
    if(lastAction != 2) lightsNight();
  } else if(period >= LIGHTS_DAY_DOOR_OPEN_PERIOD) {
    if(lastAction != 1) lightsDayDoorOpen();
  }
}

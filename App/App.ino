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
bool lastActionManual = false;

void setup(void) {
  setupLights();
  setupDoor();

  setupServer();
  setupEndpoint("/lightsDayDoorOpen", []() {
    lastActionManual = true;
    lightsDayDoorOpen();
  });
  setupEndpoint("/lightsNight", []() {
    lastActionManual = true;
    lightsNight();
  });
  setupEndpoint("/doorClosed", []() {
    lastActionManual = true;
    doorClosed();
  });
  setupEndpoint("/lightsOff", []() {
    lastActionManual = true;
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
  Serial.println("Lights turned to day mode, door open");
  server.send(200, "text/plain;charset=UTF-8", "Lights: day, door: open.");
}

void lightsNight() {
  lastAction = 2;
  turnNightLightsOn();
  Serial.println("Lights turned to night mode");
  server.send(200, "text/plain", "Lights: night, door: open.");
}

void doorClosed() {
  lastAction = 3;
  closeDoor();
  Serial.println("Door closed");
  server.send(200, "text/plain", "Lights: night, door: closed.");
}

void lightsOff() {
  lastAction = 4;
  turnAllLightsOff();
  stopDoor();
  lockDoor();
  Serial.println("Lights turned off");
  server.send(200, "text/plain", "Lights: off, door: closed.");
}

void getCoornickStatus() {
  char buffer[200];
  sprintf(buffer, "{\"dayLights\": %d, \"nightLights\": %d, \"isDoorLocked\": %d, \"isDoorOpening\": %d, \"isDoorClosing\": %d, \"coornickTurnedOn\": %d, \"isDoorOpened\": %d, \"isDoorClosed\": %d, \"lastAction\": %d, \"lastActionManual\": %d}", 
    isDayLightOn, isNightLightOn, isDoorLocked, isDoorOpening, isDoorClosing, coornickTurnedOn, isDoorOpened, isDoorClosed, lastAction, lastActionManual);
  server.send(200, "application/json", buffer); 
}

void getActionTimes() {
  char buffer[200];
  sprintf(buffer, "{\"lightsDayDoorOpen\": %d, \"lightsNight\": %d, \"doorClosed\": %d, \"lightsOff\": %d}", 
    lightsDayDoorOpenPeriod, lightsNightPeriod, doorClosedPeriod, lightsOffPeriod);
  server.send(200, "application/json", buffer); 
}

void setActionTimes() {
  lightsDayDoorOpenPeriod = server.arg("lightsDayDoorOpen").toInt();
  lightsNightPeriod = server.arg("lightsNight").toInt();
  doorClosedPeriod = server.arg("doorClosed").toInt();
  lightsOffPeriod = server.arg("lightsOff").toInt();
  if (coornickTurnedOn) {
    datetimeCheck(httpGet, [](int newPeriod) {
      updatePeriod(newPeriod);
    });
  }
  Serial.println("Action times set: ");
  Serial.println(lightsDayDoorOpenPeriod);
  Serial.println(lightsNightPeriod);
  Serial.println(doorClosedPeriod);
  Serial.println(lightsOffPeriod);
  server.send(200, "text/plain", "Period times updated.");
}

void coornickSwitch() {
  int previousState = server.arg("previousState").toInt();
  coornickTurnedOn = !previousState;
  Serial.println("Changed automation state:");
  Serial.println(previousState);
  Serial.println(coornickTurnedOn);
  server.send(200, "text/plain", "Coornick state changed.");
}

void updatePeriod(int newPeriod)
{
  if (newPeriod == INVALID_PERIOD) {
    return;
  }
  period = newPeriod;
  lastActionManual = false;
  if(lastAction == 3) {
    if(period >= lightsOffPeriod) lightsOff();
  } else if(lastAction == 2) {
    if(period >= doorClosedPeriod) doorClosed();
  } else if(lastAction == 1) {
    if(period >= lightsNightPeriod) lightsNight();
  } else if(lastAction == 4) {
    if(period >= lightsDayDoorOpenPeriod) lightsDayDoorOpen();
  }
}

const int ENGINE_IN_1 = D1;
const int ENGINE_IN_2 = D2;
const int SENSOR_DOOR_OPEN = D0;
const int SENSOR_DOOR_CLOSED = D7;
const int LOCKER_RELAY_PIN = D5;

int isDoorClosing = 0;
int isDoorOpening = 0;
int isDoorLocked = 0;
int isDoorOpened = 0;
int isDoorClosed = 0;

void setupDoor() {
  pinMode(LOCKER_RELAY_PIN, OUTPUT);
  pinMode(ENGINE_IN_1, OUTPUT);
  pinMode(ENGINE_IN_2, OUTPUT);
  pinMode(SENSOR_DOOR_OPEN, INPUT);
  pinMode(SENSOR_DOOR_CLOSED, INPUT);
}

void openDoor() {
  digitalWrite(ENGINE_IN_1, HIGH);
  digitalWrite(ENGINE_IN_2, LOW);
  isDoorOpening = 1;
  isDoorClosing = 0;
  isDoorOpened = 0;
  isDoorClosed = 0;
}

void closeDoor() {
  digitalWrite(ENGINE_IN_1, LOW);
  digitalWrite(ENGINE_IN_2, HIGH);
  isDoorOpening = 0;
  isDoorClosing = 1;
  isDoorOpened = 0;
  isDoorClosed = 0;
}

void stopDoor() {
  isDoorOpening = 0;
  isDoorClosing = 0;
  digitalWrite(ENGINE_IN_1, LOW);
  digitalWrite(ENGINE_IN_2, LOW);
}

void lockDoor() {
  digitalWrite(LOCKER_RELAY_PIN, HIGH);
  isDoorLocked = 1;
}

void unlockDoor() {
  digitalWrite(LOCKER_RELAY_PIN, LOW);
  isDoorLocked = 0;
}

void checkDoorOpened() {
  if (digitalRead(SENSOR_DOOR_OPEN) == HIGH && isDoorOpening == 1) {
    stopDoor();
    lockDoor();
    isDoorOpened = 1;
    isDoorClosed = 0;
  }
}

void checkDoorClosed() {
  if (digitalRead(SENSOR_DOOR_CLOSED) == HIGH && isDoorClosing == 1) {
    stopDoor();
    isDoorOpened = 0;
    isDoorClosed = 1;
  }
}
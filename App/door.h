const int in1 = D1;
const int in2 = D2;
const int SENSOR_DOOR_OPEN = D0;
const int SENSOR_DOOR_CLOSED = D7;
const int LOCKER_RELAY_PIN = D5;

int isDoorClosing = 0;
int isDoorOpening = 0;
int isDoorLocked = 0;

void openDoor() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  isDoorOpening = 1;
}

void closeDoor() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  isDoorClosing = 1;
}

void stopDoor() {
  isDoorOpening = 0;
  isDoorClosing = 0;
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
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
  }
}

void checkDoorClosed() {
  if (digitalRead(SENSOR_DOOR_CLOSED) == HIGH && isDoorClosing == 1) {
    stopDoor();
  }
}
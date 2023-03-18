const int LIGHTS_DAY_RELAY_PIN = D4;  // the Arduino pin, which connects to the IN pin of relay
const int LIGHTS_NIGHT_RELAY_PIN = D6;

int isDayLightOn = 0;
int isNightLightOn = 0;

void setupLights() {
  pinMode(LIGHTS_DAY_RELAY_PIN, OUTPUT);
  pinMode(LIGHTS_NIGHT_RELAY_PIN, OUTPUT);
}

void turnDayLightsOn() {
  digitalWrite(LIGHTS_DAY_RELAY_PIN, LOW);
  digitalWrite(LIGHTS_NIGHT_RELAY_PIN, HIGH);
  isDayLightOn = 1;
  isNightLightOn = 0;
}

void turnNightLightsOn() {
  digitalWrite(LIGHTS_DAY_RELAY_PIN, LOW);
  digitalWrite(LIGHTS_NIGHT_RELAY_PIN, LOW);
  isDayLightOn = 1;
  isNightLightOn = 1;
}

void turnAllLightsOff() {
  digitalWrite(LIGHTS_DAY_RELAY_PIN, HIGH);
  digitalWrite(LIGHTS_NIGHT_RELAY_PIN, HIGH);
  isDayLightOn = 0;
  isNightLightOn = 0;
}
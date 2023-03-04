const unsigned long hourCheckInterval = 15 * 60 * 1000; // 15min
unsigned long previousHourCheckTime = 0;
const char* hourCheckUrl = "http://worldtimeapi.org/api/ip"; 
int Year, Month, Day, Hour, Minute;

int parseDateTime(String payload) {
  int period = 100;
  StaticJsonDocument<800> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return period;
  }
  const char* datetime = doc["datetime"];
  Serial.println(datetime);

  sscanf(datetime, "%d-%d-%dT%d:%d:", &Year, &Month, &Day, &Hour, &Minute);
  period = 4 * Hour + (Minute / 15);
  Serial.println(period);
  return period;
}

void datetimeCheck(String (*httpGet)(const char* url), void (*callback)(int newPeriod)) {
  unsigned long currentTime = millis();
  unsigned long previousHourCheckTimeCopy = previousHourCheckTime;
  previousHourCheckTime = currentTime;
  String payload = (*httpGet)(hourCheckUrl);
  if (payload != "") {
    int period = parseDateTime(payload);
    (*callback)(period);
  } else {
    previousHourCheckTime = previousHourCheckTimeCopy;
  }
}

void loopDatetimeCheck(String (*httpGet)(const char* url), void (*callback)(int newPeriod)) {  
  unsigned long currentTime = millis();
  if( currentTime - previousHourCheckTime >= hourCheckInterval ){
    datetimeCheck((*httpGet), (*callback));
  }
}

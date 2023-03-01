#include <Arduino.h> //?
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h> //?
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h> //needed?
#include <ArduinoJson.h>
#include "tmp.h"

#ifndef STASSID
#define STASSID "BOMBA2"
#define STAPSK "Ziombel9/11"
#endif

#define html "<!DOCTYPE html><html lang=\"en\"><head>  <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css\"    integrity=\"sha384-GJzZqFGwb1QTTN6wy59ffF1BuGJpLSa9DkKMp0DgiMDm4iYMj70gZWKYbI706tWS\"    crossorigin=\"anonymous\">  <title>Kurnik Państwa Nowaków</title>  <style>    h1 {      color: grey;      text-align: center;    }    div.one {      margin-top: 40px;      text-align: center;    }    button {      margin-top: 10px;    }  </style></head><body>  <div class=\"container\">    <h1>Ko, ko, ko!</h1>    <div class=\"one\">      <div class=\"row\">        <button id=\"lightsDayDoorOpen\" type=\"button\" class=\"btn btn-light\">Otwórz drzwiczki i zapal światło dzienne</button>      </div>      <div class=\"row\">        <button id=\"lightsNight\" type=\"button\" class=\"btn btn-light\">Zapal światło nocne i zgaś dzienne</button>      </div>      <div class=\"row\">        <button id=\"doorClosed\" type=\"button\" class=\"btn btn-light\">Zamknij drzwiczki</button>      </div>      <div class=\"row\">        <button id=\"lightsOff\" type=\"button\" class=\"btn btn-light\">Zgaś światło nocne</button>      </div>    </div>    <div class=\"row\"> <p id=\"status\"></p> </div>  </div>  <!-- Optional JavaScript -->  <!-- jQuery first, then Popper.js, then Bootstrap JS -->    <script src=\"https://code.jquery.com/jquery-3.3.1.slim.min.js\"    integrity=\"sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo\"    crossorigin=\"anonymous\"></script>  <script src=\"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.6/umd/popper.min.js\"    integrity=\"sha384-wHAiFfRlMFy6i5SRaxvfOCifBUQy1xHdJ/yoi7FRNXMRBu5WHdZYu1hA6ZOblgut\"    crossorigin=\"anonymous\"></script>  <script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/js/bootstrap.min.js\"    integrity=\"sha384-B0UglyR+jN6CkvvICOB2joaf5I4l3gm9GU6Hc1og6Ls7i6U/mkkaduKaBhlAXv9k\"    crossorigin=\"anonymous\"></script>  <script type=\"text/javascript\">    function reqListener() {      console.log(this.responseText);      $('#status').text(\"Data: \" + this.responseText);     }    function get(url, callback){      const req = new XMLHttpRequest();      if (callback) req.addEventListener(\"load\", callback);      req.open(\"GET\", window.location.href + url);      req.send();    }    $(\"#lightsDayDoorOpen\").button().click(function(){        get(\"lightsDayDoorOpen\")    });     $(\"#lightsNight\").button().click(function(){        get(\"lightsNight\")    });     $(\"#doorClosed\").button().click(function(){        get(\"doorClosed\")    });     $(\"#lightsOff\").button().click(function(){        get(\"lightsOff\")    });         setInterval(async () => { get(\"status\", reqListener);}, 5000);  </script></body></html>"
const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);
ESP8266WiFiMulti WiFiMulti;

const int LIGHTS_DAY_RELAY_PIN = D4;  // the Arduino pin, which connects to the IN pin of relay
const int LIGHTS_NIGHT_RELAY_PIN = D6;
const int LOCKER_RELAY_PIN = D5;
// const int enA = D6;
const int in1 = D1;
const int in2 = D2;
const int SENSOR_DOOR_OPEN = D0;
const int SENSOR_DOOR_CLOSED = D7;

int rotDirection = 0;
int isDayLightOn = 0;
int isNightLightOn = 0;

const int LIGHTS_DAY_DOOR_OPEN_PERIOD = 7 * 4 + 2; // action 1
const int LIGHTS_NIGHT_PERIOD = 17 * 4; // action 2
const int DOOR_CLOSED_PERIOD = 20 * 4; // action 3
const int LIGHTS_OFF_PERIOD = 20 * 4 + 2; // action 4
int period = 100;
int lastAction = 4;
int buttonState = 0;
int Year, Month, Day, Hour, Minute;

const unsigned long hourCheckInterval = 15 * 60 * 1000; // 15min
unsigned long previousHourCheckTime = 0;

int isClosing = 0;
int isOpening = 0;

void setup(void) {
  tmp_setup();
  pinMode(LIGHTS_DAY_RELAY_PIN, OUTPUT);
  pinMode(LIGHTS_NIGHT_RELAY_PIN, OUTPUT);
  pinMode(LOCKER_RELAY_PIN, OUTPUT);

  // pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(SENSOR_DOOR_OPEN, INPUT);
  pinMode(SENSOR_DOOR_CLOSED, INPUT);
  // Set initial rotation direction
  // digitalWrite(in1, LOW);
  // digitalWrite(in2, HIGH);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  WiFiMulti.addAP(ssid, password);

  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) { Serial.println("MDNS responder started"); }

  server.on("/", handleRoot);

  server.on("/lightsDayDoorOpen", []() {
    lightsDayDoorOpen();
  });
  server.on("/lightsNight", []() {
    lightsNight();
  });
  server.on("/doorClosed", []() {
    doorClosed();
  });
  server.on("/lightsOff", []() {
    lightsOff();
  });
  // Serial.begin(9600);
  // while (!Serial) continue;
  server.on("/status", []() {
    getCoornickStatus();
  });

  server.on("/getTime", []() {
    httpGet("http://worldtimeapi.org/api/ip");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  lightsOff();
}

void loop(void) {
  server.handleClient();
  MDNS.update();

  unsigned long currentTime = millis();
  if( currentTime - previousHourCheckTime >= hourCheckInterval ){
    previousHourCheckTime = currentTime; // pass currentTime as a parameter and assign only when downloaded
    httpGet("http://worldtimeapi.org/api/ip");
  }

  if (digitalRead(SENSOR_DOOR_CLOSED) == HIGH && isClosing == 1) {
    stopDoor();
  }

  if (digitalRead(SENSOR_DOOR_OPEN) == HIGH && isOpening == 1) {
    stopDoor();
    digitalWrite(LOCKER_RELAY_PIN, HIGH);
  }
}

//from BasicHttpClient
void httpGet(const char* url) {
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, url)) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
          server.send(200, "text/plain", payload);
          StaticJsonDocument<800> doc;
          DeserializationError error = deserializeJson(doc, payload);

          // Test if parsing succeeds.
          if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
          }
          const char* datetime = doc["datetime"];
          Serial.println(datetime);
          createElements(datetime);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }
}

void createElements(const char *str)
{
  sscanf(str, "%d-%d-%dT%d:%d:", &Year, &Month, &Day, &Hour, &Minute);
  period = 4 * Hour + (Minute / 15);
  Serial.println(period);
  if(period >= LIGHTS_OFF_PERIOD) {
    if(lastAction != 4) lightsOff();
  } else if(period >= DOOR_CLOSED_PERIOD) {
    if(lastAction != 3) doorClosed();
  } else if(period >= LIGHTS_NIGHT_PERIOD) {
    if(lastAction != 2) lightsOff();
  } else if(period >= LIGHTS_DAY_DOOR_OPEN_PERIOD) {
    if(lastAction != 1) lightsOff();
  }
}

void lightsDayDoorOpen() {
  lastAction = 1;
  Serial.println("Lights: day, door: open.");
  digitalWrite(LOCKER_RELAY_PIN, LOW);
  digitalWrite(LIGHTS_DAY_RELAY_PIN, LOW);
  digitalWrite(LIGHTS_NIGHT_RELAY_PIN, HIGH);
  isDayLightOn = 1;
  isNightLightOn = 0;
  server.send(200, "text/plain;charset=UTF-8", "Dzień dobry, kurki!");
  delay(500);
  openDoor();
}
void lightsNight() {
  lastAction = 2;
  Serial.println("Lights: night, door: open.");
  digitalWrite(LIGHTS_DAY_RELAY_PIN, HIGH);
  digitalWrite(LIGHTS_NIGHT_RELAY_PIN, LOW);
  isDayLightOn = 0;
  isNightLightOn = 1;
  server.send(200, "text/plain", "Kurki, kurki do domu!");
}
void doorClosed() {
  lastAction = 3;
  Serial.println("Lights: night, door: closed.");
  server.send(200, "text/plain", "Zamykamy drzwiczki!");
  //turn the engine on
  closeDoor();
}
void lightsOff() {
  lastAction = 4;
  Serial.println("Lights: off, door: closed.");
  digitalWrite(LIGHTS_DAY_RELAY_PIN, HIGH);
  digitalWrite(LIGHTS_NIGHT_RELAY_PIN, HIGH);
  digitalWrite(LOCKER_RELAY_PIN, HIGH);
  isDayLightOn = 0;
  isNightLightOn = 0;
  server.send(200, "text/plain", "Dobranoc, kurki!");
  stopDoor();
}

void handleRoot() {
  server.send(200, "text/html;charset=UTF-8", html);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }
  server.send(404, "text/plain", message);
}

void openDoor() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  rotDirection = 1;
  isOpening = 1;
}

void closeDoor() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  rotDirection = 0;
  isClosing = 1;
}

void stopDoor() {
  isOpening = 0;
  isClosing = 0;
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void getCoornickStatus() {
  char buffer[100];
  sprintf(buffer, "{\"dayLights\": %d, \"nightLights\": %d}", isDayLightOn, isNightLightOn);
  server.send(200, "application/json", buffer); 
}

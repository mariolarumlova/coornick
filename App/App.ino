#include <Arduino.h> //?
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h> //?
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h> //needed?
#include <ArduinoJson.h>

#ifndef STASSID
#define STASSID "Karuzelakaruzela"
#define STAPSK "naolbinieconiedziela"
#endif

#define html "<!DOCTYPE html><html lang=\"en\"><head>  <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css\"    integrity=\"sha384-GJzZqFGwb1QTTN6wy59ffF1BuGJpLSa9DkKMp0DgiMDm4iYMj70gZWKYbI706tWS\"    crossorigin=\"anonymous\">  <title>Kurnik Państwa Nowaków</title>  <style>    h1 {      color: grey;      text-align: center;    }    div.one {      margin-top: 40px;      text-align: center;    }    button {      margin-top: 10px;    }  </style></head><body>  <div class=\"container\">    <h1>Ko, ko, ko!</h1>    <div class=\"one\">      <div class=\"row\">        <button id=\"btnSubmit\" type=\"button\" class=\"btn btn-light\">Otwórz drzwiczki i zapal światło dzienne</button>      </div>      <div class=\"row\">        <button type=\"button\" class=\"btn btn-light\">Zapal światło nocne i zgaś dzienne</button>      </div>      <div class=\"row\">        <button type=\"button\" class=\"btn btn-light\">Zamknij drzwiczki</button>      </div>      <div class=\"row\">        <button type=\"button\" class=\"btn btn-light\">Zgaś światło nocne</button>      </div>    </div>  </div>  <!-- Optional JavaScript -->  <!-- jQuery first, then Popper.js, then Bootstrap JS -->  <script type=\"text/javascript\">    function btnClick()    {        $(\"#btnSubmit\").button().click(function(){            alert(\"button\");        });        }  </script>  <script src=\"https://code.jquery.com/jquery-3.3.1.slim.min.js\"    integrity=\"sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo\"    crossorigin=\"anonymous\"></script>  <script src=\"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.6/umd/popper.min.js\"    integrity=\"sha384-wHAiFfRlMFy6i5SRaxvfOCifBUQy1xHdJ/yoi7FRNXMRBu5WHdZYu1hA6ZOblgut\"    crossorigin=\"anonymous\"></script>  <script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/js/bootstrap.min.js\"    integrity=\"sha384-B0UglyR+jN6CkvvICOB2joaf5I4l3gm9GU6Hc1og6Ls7i6U/mkkaduKaBhlAXv9k\"    crossorigin=\"anonymous\"></script></body></html>"

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);
ESP8266WiFiMulti WiFiMulti;

const int RELAY_PIN = D5;  // the Arduino pin, which connects to the IN pin of relay
const int BUTTON_PIN = D6;
int buttonState = 0;

void handleRoot() {
  digitalWrite(RELAY_PIN, LOW);
  server.send(200, "text/html charset=unicode", html);
  digitalWrite(RELAY_PIN, HIGH);
}

void handleNotFound() {
  digitalWrite(RELAY_PIN, LOW);
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
  digitalWrite(RELAY_PIN, HIGH);
}

void setup(void) {
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH);

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

  server.on("/on", []() {
    digitalWrite(RELAY_PIN, LOW);
    server.send(200, "text/plain", "light is on");
  });

  server.on("/off", []() {
    digitalWrite(RELAY_PIN, HIGH);
    server.send(200, "text/plain", "light is off");
  });

  server.on("/getTime", []() {
    httpGet("http://worldtimeapi.org/api/ip");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();

  buttonState = digitalRead(BUTTON_PIN);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(RELAY_PIN, LOW);
  } else {
    // turn LED off:
    digitalWrite(RELAY_PIN, HIGH);
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

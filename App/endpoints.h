#ifndef STASSID
#define STASSID "BOMBA2"
#define STAPSK "Ziombel9/11"
#endif

#define html "<!DOCTYPE html><html lang=\"en\"><head>  <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css\"    integrity=\"sha384-GJzZqFGwb1QTTN6wy59ffF1BuGJpLSa9DkKMp0DgiMDm4iYMj70gZWKYbI706tWS\"    crossorigin=\"anonymous\">  <title>Kurnik Państwa Nowaków</title>  <style>    h1 {      color: grey;      text-align: center;    }    div.one {      margin-top: 40px;      text-align: center;    }    button {      margin-top: 10px;    }  </style></head><body>  <div class=\"container\">    <h1>Ko, ko, ko!</h1>    <div class=\"one\">      <div class=\"row\">        <button id=\"lightsDayDoorOpen\" type=\"button\" class=\"btn btn-light\">Otwórz drzwiczki i zapal światło dzienne</button>      </div>      <div class=\"row\">        <button id=\"lightsNight\" type=\"button\" class=\"btn btn-light\">Zapal światło nocne i zgaś dzienne</button>      </div>      <div class=\"row\">        <button id=\"doorClosed\" type=\"button\" class=\"btn btn-light\">Zamknij drzwiczki</button>      </div>      <div class=\"row\">        <button id=\"lightsOff\" type=\"button\" class=\"btn btn-light\">Zgaś światło nocne</button>      </div>    </div>    <div class=\"row\"> <p id=\"status\"></p> </div>  </div>  <!-- Optional JavaScript -->  <!-- jQuery first, then Popper.js, then Bootstrap JS -->    <script src=\"https://code.jquery.com/jquery-3.3.1.slim.min.js\"    integrity=\"sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo\"    crossorigin=\"anonymous\"></script>  <script src=\"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.6/umd/popper.min.js\"    integrity=\"sha384-wHAiFfRlMFy6i5SRaxvfOCifBUQy1xHdJ/yoi7FRNXMRBu5WHdZYu1hA6ZOblgut\"    crossorigin=\"anonymous\"></script>  <script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/js/bootstrap.min.js\"    integrity=\"sha384-B0UglyR+jN6CkvvICOB2joaf5I4l3gm9GU6Hc1og6Ls7i6U/mkkaduKaBhlAXv9k\"    crossorigin=\"anonymous\"></script>  <script type=\"text/javascript\">    function reqListener() {      console.log(this.responseText);      $('#status').text(\"Data: \" + this.responseText);     }    function get(url, callback){      const req = new XMLHttpRequest();      if (callback) req.addEventListener(\"load\", callback);      req.open(\"GET\", window.location.href + url);      req.send();    }    $(\"#lightsDayDoorOpen\").button().click(function(){        get(\"lightsDayDoorOpen\")    });     $(\"#lightsNight\").button().click(function(){        get(\"lightsNight\")    });     $(\"#doorClosed\").button().click(function(){        get(\"doorClosed\")    });     $(\"#lightsOff\").button().click(function(){        get(\"lightsOff\")    });         setInterval(async () => { get(\"status\", reqListener);}, 5000);  </script></body></html>"
const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);
ESP8266WiFiMulti WiFiMulti;

void setupEndpoint(const char* endpoint, void (*)());
void handleRoot();
void handleNotFound();

void setupServer() {
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

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loopServer() {
  server.handleClient();
  MDNS.update();
}

void setupEndpoint(const char* endpoint, void (*callback)()) {
  server.on(endpoint, (*callback));
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

//from BasicHttpClient
String httpGet(const char* endpoint) {//, void (*onSuccess)(String param1), void (*onError)()) {
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, endpoint)) {  // HTTP


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
          return payload;
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }
  return "";
}
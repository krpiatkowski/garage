#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include "config.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* isOpenWarningURL = IS_OPEN_WARNING_URL;

#define RELAY_PIN 13
#define SERVER_PORT 80

ESP8266WebServer server(SERVER_PORT);
HTTPClient http;

bool hasSendClosed = false;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);

  Serial.begin(9600);
  delay(4000);
  setupWifi();
  setupServer();
}

void loop() {
  server.handleClient();
  //handleIsOpen();
  delay(500);
}

void setupWifi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    WiFi.begin(ssid, password);
  }
  Serial.println("");
  Serial.printf("Connected to %s\n", ssid);
  Serial.println("");
}

void setupServer() {
  server.on("/toggle", handleToggle);
  server.on("/isClosed", handleIsClosed);

  //Starting the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: http://");
  Serial.print(WiFi.localIP());
}


void handleIsClosed() {
  String response = "{\"isClosed\":";
  response += isClosed() ? "true" : "false";
  response += "}";
  server.send(200, "json/application", response);
}

void handleToggle() {
  server.send(200, "json/application", "{\"response\": true}");
  digitalWrite(RELAY_PIN, HIGH);
  delay(RELAY_TOGGLE_TIME);
  digitalWrite(RELAY_PIN, LOW);
}

bool didSendIsOpen = false;
void handleIsOpen(){
  if(!isClosed() && !didSendIsOpen){
    http.begin(isOpenWarningURL);
    int httpCode = http.GET();
    if(httpCode > 0) {
        if(httpCode == HTTP_CODE_OK) {
          Serial.println("Did send isOpen");
          didSendIsOpen = true;
        }
    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    didSendIsOpen = false;
  }
}

bool isClosed() {
  return analogRead(A0) >= 1000;
}

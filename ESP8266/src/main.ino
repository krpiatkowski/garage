#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include "config.h"

const char* wifi_ssid = WIFI_SSID;
const char* wifi_password = WIFI_PASSWORD;
const char* user = USER;
const char* password = PASSWORD;
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
  delay(500);
}

void setupWifi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    WiFi.begin(wifi_ssid, wifi_password);
  }
  Serial.println("");
  Serial.printf("Connected to %s\n", wifi_ssid);
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
  if (!server.authenticate(user, password)) {
      return server.requestAuthentication();
  }
  String response = "{\"isClosed\":";
  response += isClosed() ? "true" : "false";
  response += "}";
  Serial.printf("%i", analogRead(A0));
  server.send(200, "json/application", response);
}

void handleToggle() {
  if (!server.authenticate(user, password)) {
      return server.requestAuthentication();
  }
  server.send(200, "json/application", "{\"response\": true}");
  digitalWrite(RELAY_PIN, HIGH);
  delay(RELAY_TOGGLE_TIME);
  digitalWrite(RELAY_PIN, LOW);
}

bool isClosed() {
  return analogRead(A0) >= 1000;
}

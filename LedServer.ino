#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "RGBEffects.h"

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "you-password"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

// RGB Led stuff
int redPin = D7;
int greenPin = D8;
int bluePin = D6;
int updateDelay = 200;
RGBEffects rgbEffects( redPin, greenPin, bluePin );

void handleRoot() {
  String msg = "<html><head><title>RGB Led Server</title><style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style></head>";
  msg += "<body><h2>RGB Led Server</h2>Para trocar a cor/efeito envie:<br/>";
  msg += "<b>/mode</b>/off<br/>";
  msg += "<b>/mode</b>/red<br/>";
  msg += "<b>/mode</b>/green<br/>";
  msg += "<b>/mode</b>/blue<br/>";
  msg += "<b>/mode</b>/yellow<br/>";
  msg += "<b>/mode</b>/purple<br/>";
  msg += "<b>/mode</b>/violet<br/>";
  msg += "<b>/mode</b>/white<br/>";
  msg += "<b>/mode</b>/rainbow<br/>";
  msg += "<b>/mode</b>/fade<br/>";
  msg += "<b>/mode</b>/cube<br/>";
  msg += "<b>/mode</b>/blink<br/>";
  msg += "<body></html>";
  
  server.send(200, "text/html", msg);
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
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(9600);

  rgbEffects.setEffect(EFFECT_BLINK);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    rgbEffects.update();
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("ledserver")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/mode/off", []() {
    rgbEffects.setEffect(EFFECT_OFF);
    server.send(200, "text/plain", "OK");
  });
  
  server.on("/mode/red", []() {
    rgbEffects.setEffect(EFFECT_SOLID_RED);
    server.send(200, "text/plain", "OK");
  });

  server.on("/mode/green", []() {
    rgbEffects.setEffect(EFFECT_SOLID_GREEN);
    server.send(200, "text/plain", "OK");
  });

  server.on("/mode/blue", []() {
    rgbEffects.setEffect(EFFECT_SOLID_BLUE);
    server.send(200, "text/plain", "OK");
  });

  server.on("/mode/yellow", []() {
    rgbEffects.setEffect(EFFECT_SOLID_YELLOW);
    server.send(200, "text/plain", "OK");
  });

  server.on("/mode/purple", []() {
    rgbEffects.setEffect(EFFECT_SOLID_PURPLE);
    server.send(200, "text/plain", "OK");
  });

  server.on("/mode/violet", []() {
    rgbEffects.setEffect(EFFECT_SOLID_VIOLET);
    server.send(200, "text/plain", "OK");
  });

  server.on("/mode/white", []() {
    rgbEffects.setEffect(EFFECT_SOLID_WHITE);
    server.send(200, "text/plain", "OK");
  });

  server.on("/mode/rainbow", []() {
    rgbEffects.setEffect(EFFECT_RAINBOW);
    server.send(200, "text/plain", "OK");
  });

  server.on("/mode/fade", []() {
    rgbEffects.setEffect(EFFECT_FADE);
    server.send(200, "text/plain", "OK");
  });

  server.on("/mode/cube", []() {
    rgbEffects.setEffect(EFFECT_CUBE);
    server.send(200, "text/plain", "OK");
  });

  server.on("/mode/blink", []() {
    rgbEffects.setEffect(EFFECT_BLINK);
    server.send(200, "text/plain", "OK");
  });
  
  server.onNotFound(handleNotFound);  

  server.begin();
  Serial.println("HTTP server started");

  rgbEffects.setEffect(EFFECT_CUBE);
}

void loop(void) {
  server.handleClient();
  MDNS.update();
  rgbEffects.update();
  delay(updateDelay);
  
  // Still connected?
  if (WiFi.status() != WL_CONNECTED) 
  {
    rgbEffects.setEffect(EFFECT_BLINK); // Sign as disconnected
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      rgbEffects.update();
      WiFi.reconnect();
    }
    // Back to the normal effect
    rgbEffects.setEffect(EFFECT_CUBE);
  }
}

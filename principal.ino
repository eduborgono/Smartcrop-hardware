/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid     = "Fayola";
const char* password = "frodo123";

const char* host = "smartcrop.lightup.cl";

StaticJsonBuffer<300> JSONBuffer;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient cliente;
  const int httpPort = 80;
  if (!cliente.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  /*Serial.print("URI: ");
  Serial.println("GET /api/pots/-Kopfh8WtjjTtCsirW8S HTTP/1.1\r\nHost: smartcrop.lightup.cl\r\nCache-Control: no-cache\r\n");*/
  //Serial.println();
  cliente.println("GET /api/pots/-Kopfh8WtjjTtCsirW8S HTTP/1.1\r\nHost: smartcrop.lightup.cl\r\nCache-Control: no-cache\r\n");
  unsigned long timeout = millis();
  while (cliente.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      cliente.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(cliente.available()){
    String line = cliente.readStringUntil('\r');
    JsonObject&  parsed= JSONBuffer.parseObject(line);
    if (parsed.success()) {
      Serial.print(line);
      Serial.print(parsed["humidity"])
    }
  }
  
  Serial.println();
  Serial.println("closing connection");
}

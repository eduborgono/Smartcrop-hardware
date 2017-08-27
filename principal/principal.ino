#include "SmartCropWifi.h"

SmartCropWifi cliente;
unsigned long timer;

void setup() {
  cliente.conectarRed("Fayola","frodo123");
  Serial.begin(9600);
  timer = millis();
}

void loop() {
    if(cliente.estadoConexion()) {
      cliente.conectarServidor("smartcrop.lightup.cl", 80);
      cliente.estadoMaceta("-Kopfh8WtjjTtCsirW8S", &Serial);
      cliente.leerRespuesta(&Serial);
    }
    else {
      Serial.println(" ");
    }
}

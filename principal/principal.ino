#include "SmartCropWifi.h"

SmartCropWifi cliente;
unsigned long timer;

void setup() {
  cliente.conectarRed("Fayola","frodo123");
  Serial.begin(9600);
  timer = millis();
}

void loop() {
  if(millis()-timer>2000UL) {
    if(cliente.estadoConexion()) {
      Serial.println("Conectado");
      cliente.direccionIP(&Serial);
    }
    else {
      Serial.println("Desconectado");
    }
    timer = millis();
  }
}

#include "SmartCropWifi.h"

SmartCropWifi cliente;
unsigned long timer;
byte estado_actual;

void setup() {
  cliente.conectarRed("Fayola","frodo123");
  Serial.begin(9600);
  timer = millis();
}

void loop() {
    if(cliente.estadoConexion()) {
      cliente.conectarServidor("smartcrop.lightup.cl", 80);
      estado_actual = cliente.leerRespuesta(&Serial);
      switch(estado_actual) {
        case NADA:
          cliente.estadoMaceta("-Kopfh8WtjjTtCsirW8S", &Serial);
          break;
      }
      
    }
    else {
      Serial.println(" ");
    }
}

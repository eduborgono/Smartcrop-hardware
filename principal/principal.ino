#include "SmartCropSensor.h"
#include "SmartCropWifi.h"

SmartCropSensor sensores(D4, D5); // dht, termo

SmartCropWifi internet;
bool reconectar;
unsigned long timer;
byte estado_actual;


void setup() {
  internet.conectarRed("Fayola", "frodo123");
  reconectar = false;
  Serial.begin(115200);
  Serial.println("");
  timer = millis();
}

void loop() {
  if(internet.estadoConexion()) {
    if(!reconectar) { // Conectado por primera vez, se setea para una posible reconexion y setear valores
      reconectar = true;
      Serial.println("");
      internet.direccionIP(&Serial);
      internet.conectarServidor("smartcrop.lightup.cl", 3000);
    }
    if(millis()-timer>8000UL) {

      if(internet.estadoServidor() && sensores.cambioTempAmbiental()) {
        internet.actHumeAmbiental(sensores.getTempAmbiental());
      }
      
      timer = millis();
    }
    else {
      sensores.leerSensores();
      if(internet.estadoServidor()) {
        internet.recepcionServidor(&Serial);
      }
    }
  }
  else {
    if(reconectar) Serial.println("Se debe reconfigurar la clave");
  }

}

#include "SmartCropSensor.h"
#include "SmartCropWifi.h"

/*#define pin_s0 D1
#define pin_s1 D2
#define pin_s2 D3

#define pin_dht D4
#define pin_termocupula D5
#define pin_hidro A0*/

SmartCropSensor sensores(D4, D5);

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
      /*Serial.print("Temperatura ambiental ");
      Serial.print(sensores.getTempAmbiental());
      Serial.print(" Humedad ambiental ");
      Serial.print(sensores.getHumeAmbiental());
      Serial.print(" Temp ");
      Serial.print(sensores.getTempTierra());
      Serial.print(" Hume ");
      Serial.print(sensores.getHumeTierra());
      Serial.println("");*/

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
  /*if(cliente.estadoConexion()) {
    cliente.conectarServidor("smartcrop.lightup.cl", 80);
    estado_actual = cliente.leerRespuesta(&Serial);
    switch(estado_actual) {
      case NADA:
        cliente.estadoMaceta("-Kopfh8WtjjTtCsirW8S", &Serial);
        break;
    }
  }
    
    Serial.println(" ");
  else {
  }*/
}

#include "SmartCropSensor.h"
#include "SmartCropWifi.h"

/*#define pin_s0 D1
#define pin_s1 D2
#define pin_s2 D3

#define pin_dht D4
#define pin_termocupula D5
#define pin_hidro A0*/

SmartCropSensor sensores(D4, D5, D1, D2, D3);

SmartCropWifi cliente;
unsigned long timer;
byte estado_actual;


void setup() {
  Serial.begin(115200);
  Serial.println("");
}

void loop() {

  //sensores.leerSensores(&Serial);

  /*if(cliente.estadoConexion()) {
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
  }*/

}

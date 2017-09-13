#include "SmartCropSensor.h"
#include "SmartCropWifi.h"
#include <SoftwareSerial.h>

SmartCropSensor sensores(D4, D5); // dht, termo

SmartCropWifi internet;
bool reconectar;
unsigned long timer;
byte estado_actual;

SoftwareSerial usb_celu(D7, D6, false, 256); //rx - tx
bool configInter;
byte indice;
byte max_chars;
char ssid[33];
char pssw[64];
char *ptr_slot;
char caracterActual;

void setup() {
  reconectar = false;
  configInter = true;
  indice = 0;
  max_chars = 33;
  ptr_slot = &ssid[0];
  Serial.begin(115200);
  usb_celu.begin(57600);
  Serial.println("");
  timer = millis();
}

void loop() {
  if(configInter) {
    if(usb_celu.available()) {
      caracterActual = usb_celu.read();
      if(caracterActual != '\r') {
        if(indice < max_chars) {
          *(ptr_slot+indice) = caracterActual;
          indice++;
        }
      }
      else {
        *(ptr_slot+indice) = '\0';
        if(max_chars == 33) {
          indice = 0;
          max_chars = 64;
          ptr_slot = &pssw[0];
        }
        else {
          indice = 0;
          max_chars = 33;
          ptr_slot = &ssid[0];
          Serial.println();
          Serial.print("'");
          Serial.print(ssid);
          Serial.print("'");
          Serial.println();
          Serial.print("'");
          Serial.print(pssw);
          Serial.print("'");
          Serial.println();

          else {
            configInter = false;
            internet.conectarRed(ssid, pssw);
            timer = millis();
          }
        }
      }
    }
  }
  else {
    if(internet.estadoConexion()) { //conectado
      if(!reconectar) { // Conectado por primera vez, se setea para una posible reconexion y setear valores
        reconectar = true;
        Serial.println("");
        internet.direccionIP(&Serial);
        internet.conectarServidor("smartcrop.lightup.cl", 80);
        
        timer = millis();
      }
      if(millis()-timer>8000UL) {
  
        if(internet.estadoServidor()) {
          if(sensores.cambioTempAmbiental()) {
            internet.actTempAmbiental(sensores.getTempAmbiental());
          }
          if(sensores.cambioHumeAmbiental()) {
            internet.actHumeAmbiental(sensores.getHumeAmbiental());
          }
          if(sensores.cambioTempTierra()) {
            internet.actTempTierra(sensores.getTempTierra());
          }
          if(sensores.cambioHumeTierra()) {
            internet.actHumeTierra(sensores.getHumeTierra());
          }
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
    else { //desconectado
      if(reconectar) { //se conecto antes, ya no
        Serial.println("Se debe reconfigurar la clave");
      }
      else { //conectando
        
        
        if(millis()-timer>10000UL) {
          
          configInter = true;
        }
      }
    }
  }
}

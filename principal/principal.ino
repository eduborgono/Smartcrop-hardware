#include "SmartCropSensor.h"
#include "SmartCropWifi.h"
#include <SoftwareSerial.h>

//59e9b52d03f6e54106d27508

SmartCropSensor sensores(D4, D5, D0); // dht, termo

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
  /**
   * Configuracion Wifi
   */
  if(configInter) {
    internet.conectarRed("Fayola", "frodo123");
    configInter = false;
    usb_celu.print('1'); //1 conectando ..
    timer = millis();
    /*if(usb_celu.available()) {
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
          if(strlen(ssid) == 0 || strlen(pssw) == 0) {
            usb_celu.print('0'); //0 no conectado malas credenciales
          }
          else {
            internet.conectarRed(ssid, pssw);
            configInter = false;
            usb_celu.print('1'); //1 conectando ..
            timer = millis();
          }
        }
      }
    }*/
  }
  /**
   * Todo lo relacionado a los sensores y envio de datos
   */
  else {
    if(internet.estadoConexion() == 1) { //conectado
      if(!reconectar) { // Conectado por primera vez, se setea para una posible reconexion y setear valores
        reconectar = true;
        Serial.println("");
        internet.direccionIP(&Serial);
        internet.conectarServidor("smartcrop.lightup.cl", 80);
        usb_celu.print('2'); //2 conectado !
        timer = millis();
      }
      if(millis()-timer>8000UL) {
        internet.actualizarBaseDatos("smartcrop.lightup.cl", 80 , "59e9b52d03f6e54106d27508", sensores.getHumeAmbiental(), sensores.getTempAmbiental(), sensores.getTempTierra(), sensores.getHumeTierra());
        //internet.actualizarBaseDatos("smartcrop.lightup.cl", 80 , "59e9b52d03f6e54106d27508", random(100), random(100), random(100), random(100));
        timer = millis();
      }
      else {
        sensores.leerSensores();
        internet.recepcionServidor();
      }
    }
    else if (internet.estadoConexion() == 0) { //desconectado
      if(reconectar) { //se conecto antes, ya no
        Serial.println("Se debe reconfigurar la clave");
      }
    }
    else if(internet.estadoConexion() == 2) { //malas credenciales
      usb_celu.print('0'); //0 no conectado malas credenciales
      configInter = true;
    }
  }
}

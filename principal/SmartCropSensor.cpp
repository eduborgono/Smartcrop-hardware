#include "SmartCropSensor.h"

const byte sensor_dht = 0;
const byte sensor_termoculpula = 1;
const byte sensor_hidrometro = 2;
const byte cantidad_sensores = 3;

SmartCropSensor::SmartCropSensor(byte pin_dht, byte pin_termo): 
                    dht(pin_dht, DHT22), cableTermo(pin_termo), termocupula(&cableTermo)  {
  hume_dht = 0.0;
  temp_dht = 0.0;
  temp_termo = 0.0;
  hume_tierra = 0;
  cambiar_sensor = false;
  sensor_actual = sensor_dht;
  numero_lecturas = 0;
  
  timer = millis();
  
  dht.begin();
  termocupula.begin();
}

SmartCropSensor::~SmartCropSensor() {  }

void SmartCropSensor::leerSensores(HardwareSerial* salida) {
  if(millis() - timer > 2000UL) {
    switch(sensor_actual) {
      case sensor_dht:
        hume_dht = dht.readHumidity();
        temp_dht = dht.readTemperature();
        break;
      case sensor_termoculpula:
        termocupula.requestTemperatures();
        temp_termo = termocupula.getTempCByIndex(0);
        break;
      case sensor_hidrometro:
        hume_tierra = analogRead(A0);
        //hume_tierra = map(hume_tierra,850,300,0,100);
        break;
    }
    
    numero_lecturas++;
    sensor_actual = numero_lecturas%cantidad_sensores;

    salida->print("Temperatura ambiental ");
    salida->print(temp_dht);
    salida->print(" Humedad ambiental ");
    salida->print(hume_dht);
    salida->print(" Temp ");
    salida->print(temp_termo);
    salida->print(" Hume ");
    salida->print(hume_tierra);
    salida->println("");
    
    timer = millis();
  }
}

float SmartCropSensor::getTempAmbiental() {
  return temp_dht;
}

float SmartCropSensor::getHumeAmbiental() {
  return hume_dht;
}

float SmartCropSensor::getTempTierra() {
  return temp_termo;
}

int SmartCropSensor::getHumeTierra() {
  return hume_tierra;
}



#include "SmartCropSensor.h"

enum {temporal, revisada};

const byte sensor_dht = 0;
const byte sensor_termoculpula = 1;
const byte sensor_hidrometro = 2;
const byte cantidad_sensores = 3;

SmartCropSensor::SmartCropSensor(byte pin_dht, byte pin_termo): 
                    dht(pin_dht, DHT22), cableTermo(pin_termo), termocupula(&cableTermo)  {
  hume_dht[revisada] = 0.0;
  temp_dht[revisada] = 0.0;
  temp_termo[revisada] = 0.0;
  hume_tierra[revisada] = 0;
  cambiar_sensor = false;
  sensor_actual = sensor_dht;
  numero_lecturas = 0;
  
  timer = millis();
  
  dht.begin();
  termocupula.begin();
}

SmartCropSensor::~SmartCropSensor() {  }

void SmartCropSensor::leerSensores() {
  if(millis() - timer > 2000UL) {
    switch(sensor_actual) {
      case sensor_dht:
        hume_dht[temporal] = dht.readHumidity();
        temp_dht[temporal] = dht.readTemperature();
        break;
      case sensor_termoculpula:
        termocupula.requestTemperatures();
        temp_termo[temporal] = termocupula.getTempCByIndex(0);
        break;
      case sensor_hidrometro:
        hume_tierra[revisada] = analogRead(A0);
        //hume_tierra = map(hume_tierra,850,300,0,100);
        break;
    }
    
    numero_lecturas++;
    sensor_actual = numero_lecturas%cantidad_sensores;
    timer = millis();
  }
}

float SmartCropSensor::getTempAmbiental() {
  if(!isnan(temp_dht[temporal])) temp_dht[revisada] = temp_dht[temporal];
  return temp_dht[revisada];
}

float SmartCropSensor::getHumeAmbiental() {
  if(!isnan(hume_dht[temporal])) hume_dht[revisada] = hume_dht[temporal];
  return hume_dht[revisada];
}

float SmartCropSensor::getTempTierra() {
  if(-127.0 != temp_termo[temporal]) temp_termo[revisada] = temp_termo[temporal];
  return temp_termo[revisada];
}

int SmartCropSensor::getHumeTierra() {
  //if(hume_tierra[temporal]<=100 && hume_tierra[temporal]>=0) hume_tierra[revisada] = hume_tierra[temporal];
  return hume_tierra[revisada];
}

bool SmartCropSensor::cambioTempAmbiental() {
  return (temp_dht[revisada] != temp_dht[temporal]);
}
bool SmartCropSensor::cambioHumeAmbiental() {
  return (hume_dht[revisada] != hume_dht[temporal]);
}
bool SmartCropSensor::cambioTempTierra() {
  return (temp_termo[revisada] != temp_termo[temporal]);
}
bool SmartCropSensor::cambioHumeTierra() {
  return (hume_tierra[revisada] != hume_tierra[temporal]);
}


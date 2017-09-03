#include "SmartCropSensor.h"

const byte sensor_dht = 0;
const byte sensor_termoculpula = 1;
const byte sensor_hidrometro = 2;

SmartCropSensor::SmartCropSensor(byte pin_dht, byte pin_termo, byte s0, byte s1, byte s2): 
                    dht(pin_dht, DHT22), cableTermo(pin_termo), termocupula(&cableTermo)  {
  hume_dht = 0.0;
  temp_dht = 0.0;
  temp_termo = 0.0;
  hume_tierra = 0;
  cambiar_sensor = false;
  sensor_actual = sensor_dht;
  numero_lecturas = 0;
  pin_s0 = s0;
  pin_s1 = s1;
  pin_s2 = s2;

  pinMode(pin_s0, OUTPUT); 
  pinMode(pin_s1, OUTPUT);
  pinMode(pin_s2, OUTPUT);

  digitalWrite(pin_s0, LOW);
  digitalWrite(pin_s1, LOW);
  digitalWrite(pin_s2, LOW);
  
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
    if(numero_lecturas == 5) {
      sensor_actual ++;
      if(sensor_actual >= 3) sensor_actual = 0;
      digitalWrite(pin_s0, bitRead(sensor_actual, 0));
      digitalWrite(pin_s1, bitRead(sensor_actual, 1));
      digitalWrite(pin_s2, bitRead(sensor_actual, 2));
      numero_lecturas = 0;

      salida->print("Temperatura ambiental ");
      salida->print(temp_dht);
      salida->print(" Humedad ambiental ");
      salida->print(hume_dht);
      salida->print(" Temp ");
      salida->print(temp_termo);
      salida->print(" Hume ");
      salida->print(hume_tierra);
      salida->println("");
    }
    timer = millis();
  }
}


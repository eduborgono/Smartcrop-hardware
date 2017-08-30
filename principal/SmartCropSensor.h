#ifndef SMART_SENSOR_H
#define SMART_SENSOR_H

#include <Arduino.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <DHT.h>

class SmartCropSensor {
  private:
    DHT dht;
    OneWire cableTermo;
    DallasTemperature termocupula;
    float hume_dht;
    float temp_dht;
    float temp_termo;
    int hume_tierra;
    bool cambiar_sensor;
    byte sensor_actual;
    unsigned long timer;
    byte numero_lecturas;
    byte pin_s0;
    byte pin_s1;
    byte pin_s2;

  public:
    SmartCropSensor(byte, byte, byte, byte, byte);
    ~SmartCropSensor();
    void leerSensores(HardwareSerial*);
  
};

#endif

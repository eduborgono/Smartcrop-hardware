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
    float hume_dht[2];
    float temp_dht[2];
    float temp_termo[2];
    int hume_tierra[2];
    bool cambiar_sensor;
    byte sensor_actual;
    unsigned long timer;
    byte numero_lecturas;

  public:
    SmartCropSensor(byte, byte);
    ~SmartCropSensor();
    void leerSensores();
    float getTempAmbiental();
    float getHumeAmbiental();
    float getTempTierra();
    int getHumeTierra();
    bool cambioTempAmbiental();
    bool cambioHumeAmbiental();
    bool cambioTempTierra();
    bool cambioHumeTierra();
};

#endif

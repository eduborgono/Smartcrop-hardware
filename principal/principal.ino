#include <OneWire.h> 
#include <DallasTemperature.h>
#include <DHT.h>

#define pin_s0 D1
#define pin_s1 D2
#define pin_s2 D4
#define pin_data D7

#define sensor_dht 0
#define sensor_termoculpula 1

OneWire oneWire(pin_data); 
DallasTemperature termocupula(&oneWire);
DHT dht(pin_data, DHT22);

float hume_dht;
float temp_dht;
float temp_termo;
bool cambiar_sensor;
byte sensor_actual;
unsigned long timer;
unsigned long timer_log;

void setup() {
  pinMode(pin_s0, OUTPUT); 
  pinMode(pin_s1, OUTPUT);
  pinMode(pin_s2, OUTPUT);

  digitalWrite(pin_s0, LOW);
  digitalWrite(pin_s1, LOW);
  digitalWrite(pin_s2, LOW);

  hume_dht = 0.0;
  temp_dht = 0.0;
  temp_termo = 0.0;
  cambiar_sensor = false;
  sensor_actual = sensor_dht;
  Serial.begin(115200);
  dht.begin();
  termocupula.begin();

  timer = millis();
  timer_log = timer;
}

void loop() {
  if(millis() - timer > 2000UL) {
    switch(sensor_actual) {
      case sensor_dht:
        if(!cambiar_sensor) {
          hume_dht = dht.readHumidity();
          temp_dht = dht.readTemperature();
        }
        else {
          digitalWrite(pin_s0, HIGH);
          sensor_actual = sensor_termoculpula;
        }
        break;
      case sensor_termoculpula:
        if(!cambiar_sensor) {
          termocupula.requestTemperatures();
          temp_termo = termocupula.getTempCByIndex(0);
        }
        else {
          digitalWrite(pin_s0, LOW);
          sensor_actual = sensor_dht;
        }
        break;
    }
    cambiar_sensor = !cambiar_sensor;
    timer = millis();
  }
  if(millis()-timer_log > 4000UL) {
    Serial.print("Temperatura ambiental ");
    Serial.print(temp_dht);
    Serial.print(" Humedad ambiental ");
    Serial.print(hume_dht);
    Serial.print(" Temp ");
    Serial.print(temp_termo);
    Serial.println("");
    timer_log = millis();
  }
}

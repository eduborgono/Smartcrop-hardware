#include <OneWire.h> 
#include <DallasTemperature.h>
#include <DHT.h>

#define pin_s0 D1
#define pin_s1 D2
#define pin_s2 D3

#define pin_dht D4
#define pin_termocupula D5
#define pin_hidro A0


#define sensor_dht 0
#define sensor_termoculpula 1
#define sensor_hidrometro 2

DHT dht(pin_dht, DHT22);
OneWire cableTermo(pin_termocupula); 
DallasTemperature termocupula(&cableTermo);


float hume_dht;
float temp_dht;
float temp_termo;
int hume_tierra;
bool cambiar_sensor;
byte sensor_actual;
unsigned long timer;
byte numero_lecturas;
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
  hume_tierra = 0;
  cambiar_sensor = false;
  sensor_actual = sensor_dht;
  Serial.begin(115200);
  dht.begin();
  termocupula.begin();
  Serial.println("");
  timer = millis();
  numero_lecturas = 0;
  timer_log = timer;
}

void loop() {
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
        hume_tierra = analogRead(pin_hidro);
        hume_tierra = map(hume_tierra,850,300,0,100);
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
      Serial.print("Temperatura ambiental ");
      Serial.print(temp_dht);
      Serial.print(" Humedad ambiental ");
      Serial.print(hume_dht);
      Serial.print(" Temp ");
      Serial.print(temp_termo);
      Serial.print(" Hume ");
      Serial.print(hume_tierra);
      Serial.println("");
    }
    timer = millis();
  }
  /*if(millis()-timer_log > 60000UL) {
    Serial.print("Temperatura ambiental ");
    Serial.print(temp_dht);
    Serial.print(" Humedad ambiental ");
    Serial.print(hume_dht);
    Serial.print(" Temp ");
    Serial.print(temp_termo);
    Serial.print(" Hume ");
    Serial.print(hume_tierra);
    Serial.println("");
    timer_log = millis();
  }*/
}

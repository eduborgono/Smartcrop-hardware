#ifndef SMART_WIFI_H
#define SMART_WIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

class SmartCropWifi {
	private:
		bool m_conectado;
		bool m_conectando;
	
	public:
		SmartCropWifi();
		~SmartCropWifi();
		void conectarRed(char*, char*);
		bool estadoConexion();
		void direccionIP(HardwareSerial*);
};

#endif
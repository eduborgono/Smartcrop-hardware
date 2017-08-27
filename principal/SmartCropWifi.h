#ifndef SMART_WIFI_H
#define SMART_WIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

class SmartCropWifi {
	private:
		bool m_conectado;
		bool m_conectando;
		
		bool m_server_conex;
		WiFiClient m_conexion;
		char m_respuesta[35];
		byte m_indice_respuesta;
		unsigned long m_timer;
		byte m_consulta;
		bool m_leer_json;
    const char* m_humedad;
    const char* m_temp;
	
	public:
		SmartCropWifi();
		~SmartCropWifi();
		void conectarRed(char*, char*);
		bool estadoConexion();
		void direccionIP(HardwareSerial*);
		void conectarServidor(char*, unsigned short);
		void estadoMaceta(char*, HardwareSerial*);
		void leerRespuesta(HardwareSerial*);
};

#endif

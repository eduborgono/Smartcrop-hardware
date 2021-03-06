#ifndef SMART_WIFI_H
#define SMART_WIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <SocketIOClient.h>

const byte NADA = 0; 
const byte CONSULTA = 1; 
const byte ACTUALIZACION = 2; 

class SmartCropWifi {
	private:
		bool m_conectado;
		bool m_conectando;
    
    SocketIOClient socket_cliente;
    bool conectando_socket;
    
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
    void desconectarRed();
		int estadoConexion();
		void direccionIP(HardwareSerial*);
		bool conectarServidor(char*, unsigned short);
    void recepcionServidor(int);
    bool estadoServidor();
		void estadoMaceta(char*, HardwareSerial*);
		byte leerRespuesta(HardwareSerial*);

    void actualizarBaseDatos(char*,unsigned short,char*,float,float,float,int);
    void actHumeAmbiental(float);
    void actTempAmbiental(float);
    void actTempTierra(float);
    void actHumeTierra(int);
};

#endif

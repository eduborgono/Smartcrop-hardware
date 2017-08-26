#include "SmartCropWifi.h"

SmartCropWifi::SmartCropWifi() {
	m_conectado = false;
	m_conectando = false;
}

SmartCropWifi::~SmartCropWifi() {  }

void SmartCropWifi::conectarRed(char* ssid, char* password) {
	WiFi.begin(ssid, password);
	m_conectando = true;
}

bool SmartCropWifi::estadoConexion() {
	if(m_conectando) {
		if(WiFi.status() == WL_CONNECTED) { 
			m_conectando = false;
			m_conectado = true;
		}
	}
	else {
		if(m_conectado && WiFi.status() != WL_CONNECTED) { 
			m_conectado = false;
			m_conectando = false;
		}
	}
	return m_conectado;
}

void SmartCropWifi::direccionIP(HardwareSerial* puntero) {
	if(m_conectado) {
		puntero->println(WiFi.localIP());
	}
}
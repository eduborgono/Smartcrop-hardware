#include "SmartCropWifi.h"

SmartCropWifi::SmartCropWifi() {
	m_conectado = false;
	m_conectando = false;
	m_server_conex = false;
	m_consulta = NADA;
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

void SmartCropWifi::conectarServidor(char* host, unsigned short puerto) {
	if(m_conectado && !m_server_conex) {
		if(!m_conexion.connect(host, puerto)) {
			m_server_conex = false;
		}
		else {
			m_server_conex = true;
		}
	}
}

void SmartCropWifi::estadoMaceta(char* id_maceta, HardwareSerial* puntero) {
	if(m_server_conex && m_consulta == NADA) {
		m_conexion.print("GET /api/pots/");
		m_conexion.print(id_maceta);
		m_conexion.println(" HTTP/1.1\r\nHost: smartcrop.lightup.cl\r\nCache-Control: no-cache\r\n");
		m_timer = millis();
		m_consulta = CONSULTA;
		m_leer_json = false;
		m_indice_respuesta = 0;
	}
}

byte SmartCropWifi::leerRespuesta(HardwareSerial* puntero) {
	if(m_server_conex) {
		switch(m_consulta) {
			case CONSULTA:
				if(m_conexion.available() == 0) {
					if(millis()-m_timer > 5000UL) {
						m_consulta = NADA;
						m_conexion.stop();
						m_server_conex = false;
						return m_consulta;
					}
				}
				else {
					char c = m_conexion.read();
					m_timer = millis();
					if(c=='{') m_leer_json = true;
					if(m_leer_json) {
						m_respuesta[m_indice_respuesta] = c;
						m_indice_respuesta++;
            m_respuesta[m_indice_respuesta] = '\0';
						if(c=='}') {
              StaticJsonBuffer<300> m_JSONBuffer;
              JsonObject& m_linea_parseada = m_JSONBuffer.parseObject(m_respuesta);
							if (m_linea_parseada.success()) {
                m_humedad = m_linea_parseada["humidity"];
                m_temp = m_linea_parseada["temperature"];
							  puntero->print(m_humedad);
                puntero->print(" ");
                puntero->println(m_temp);
							}
							m_consulta = NADA;
							m_server_conex = false;
							m_leer_json = false;
              m_conexion.stop();
						}
					}
				}
				break;
		}
	}
  return m_consulta;
}

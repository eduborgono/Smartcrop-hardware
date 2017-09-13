#include "SmartCropWifi.h"

extern String RID;
extern String Rname;
extern String Rcontent;

SmartCropWifi::SmartCropWifi() {
	m_conectado = false;
	m_conectando = false;
  conectando_socket = false;
	m_consulta = NADA;
}

SmartCropWifi::~SmartCropWifi() {  }

/**
 * Inicia un intento de conexión a la red domestica
 */
void SmartCropWifi::conectarRed(char* ssid, char* password) {
	WiFi.begin(ssid, password);
	m_conectando = true;
}

/**
 * Verifica el estado de un intento de conexion. De perderse
 * una conexion se debe realizar un nuevo intento de conexion.
 * 
 * @return Si está establecida una conexión a internet.
 */
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

/**
 * Imprime la dirección IP en un puerto serial
 * 
 * @param puntero Un puntero al puerto serial en el que se imprimira.
 */
void SmartCropWifi::direccionIP(HardwareSerial* puntero) {
	if(m_conectado) {
		puntero->println(WiFi.localIP());
	}
}

/**
 * Conecta a un servidor dado mediante un host y un puerto.
 * 
 * @param host Cadena de caracteres de la direccion del servidor
 * @param puerto Numero del puerto al cual conectarse.
 */
bool SmartCropWifi::conectarServidor(char* host, unsigned short puerto) {
	if(m_conectado) {
		if(socket_cliente.connect(host, puerto)) {
      conectando_socket = true;
			return true;
		}
	}
  return false;
}

bool SmartCropWifi::estadoServidor() {
  if(socket_cliente.connected()) {
    if(conectando_socket) {
      socket_cliente.send("connection", "message", "Connected !!!!");
      conectando_socket = false;
    }
    return true;
  }
  return false;
}

void SmartCropWifi::recepcionServidor(HardwareSerial* puntero) {
  if(socket_cliente.monitor()) {
    if (RID == "humidity"){
      puntero->println(Rcontent); 
    }
  }
}

void SmartCropWifi::actHumeAmbiental(float hume) {
  socket_cliente.send("change humidity", "message", String(hume));
}

void SmartCropWifi::actTempAmbiental(float temp) {
  socket_cliente.send("change room temperature", "message", String(temp));
}

void SmartCropWifi::actTempTierra(float temp) {
  socket_cliente.send("change temperature", "message", String(temp));
}

void SmartCropWifi::actHumeTierra(int hume) {
  socket_cliente.send("change moisture", "message", String(hume));
}

/**
 * Obsoleto
 */
void SmartCropWifi::estadoMaceta(char* id_maceta, HardwareSerial* puntero) {
	/*if(m_server_conex && m_consulta == NADA) {
		m_conexion.print("GET /api/pots/");
		m_conexion.print(id_maceta);
		m_conexion.println(" HTTP/1.1\r\nHost: smartcrop.lightup.cl\r\nCache-Control: no-cache\r\n");
		m_timer = millis();
		m_consulta = CONSULTA;
		m_leer_json = false;
		m_indice_respuesta = 0;
	}*/
}

/**
 * Obsoleto
 */
byte SmartCropWifi::leerRespuesta(HardwareSerial* puntero) {
	/*if(m_server_conex) {
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
  return m_consulta;*/
  return 0;
}



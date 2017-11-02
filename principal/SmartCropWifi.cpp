#include "SmartCropWifi.h"

SmartCropWifi::SmartCropWifi() {
	m_conectado = false;
	m_conectando = false;
	m_consulta = NADA;
}

SmartCropWifi::~SmartCropWifi() {  }

/**
 * Inicia un intento de conexión a la red domestica
 */
void SmartCropWifi::conectarRed(char* ssid, char* password) {
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  m_conectando = true;
}
void SmartCropWifi::desconectarRed() {
  WiFi.disconnect();
  m_conectando = false;
  m_conectado = false;
}

/**
 * Verifica el estado de un intento de conexion. De perderse
 * una conexion se debe realizar un nuevo intento de conexion.
 * 
 * @return Si está establecida una conexión a internet.
 */
int SmartCropWifi::estadoConexion() {
  int estado = WiFi.status();
	if(m_conectando) {
		if(estado == WL_CONNECTED) { 
			m_conectando = false;
			m_conectado = true;
      return 1;
		}
    else if(estado == WL_NO_SSID_AVAIL || estado == WL_CONNECT_FAILED) {
      return 2;
   }
	}
	else {
		if(m_conectado) { 
      if(estado != WL_CONNECTED) {
			  m_conectado = false;
			  m_conectando = false;
        return 0;
      }
      return 1;
		}
	}
	return 0;
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
		socket_cliente.begin(host, puerto);
    socket_cliente.on("bomb", event_bomb);
    return true;
	}
  return false;
}

void event_bomb(const char * payload, size_t length) {
  char* tofree = strdup(payload);
  char* line = strtok(tofree, ",");
  if (line != 0) {
    if(strcmp("Activar bomba",line) == 0) {
      line = strtok(0, ",");
      if(strcmp("59e9b52d03f6e54106d27508", line) == 0) {
        digitalWrite(D0,HIGH);
      }
    }
    else if(strcmp("Desactivar bomba",line) == 0) {
      line = strtok(0, ",");
      if(strcmp("59e9b52d03f6e54106d27508", line) == 0) {
        digitalWrite(D0,LOW);
      }
    }
  }
  free(tofree);
}

void SmartCropWifi::recepcionServidor() {
  socket_cliente.loop();
}

void SmartCropWifi::actualizarBaseDatos(char* host, unsigned short puerto , char* id, float hume_amb, float temp_amb, float temp_tie, int hume_tie) {
  WiFiClient cliente;
  if (!cliente.connect(host, puerto)) {
    Serial.println("connection failed");
    return;
  }
  String datos = String("{\"temperature\":"+String(temp_tie)+",\"roomTemperature\":"+String(temp_amb)+",\"moisture\":"+String(hume_tie)+",\"humidity\":"+String(hume_amb)+"}");
  cliente.print("PUT /api/pots/");
  cliente.print(id);
  cliente.print(" HTTP/1.1\r\nHost: smartcrop.lightup.cl\r\nConnection: close\r\nContent-Type: application/json\r\nCache-Control: no-cache\r\nContent-Length: ");
  cliente.print(datos.length());
  cliente.print("\r\n\r\n");
  cliente.println(datos);
  cliente.stop();
}

void SmartCropWifi::actHumeAmbiental(float hume) {
  //socket_cliente.send("change humidity", "message", String(hume));
}

void SmartCropWifi::actTempAmbiental(float temp) {
  //socket_cliente.send("change room temperature", "message", String(temp));
}

void SmartCropWifi::actTempTierra(float temp) {
  //socket_cliente.send("change temperature", "message", String(temp));
}

void SmartCropWifi::actHumeTierra(int hume) {
  //socket_cliente.send("change moisture", "message", String(hume));
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



/*
 * WifiModule.h
 *
 *  Created on: 4 de jun de 2017
 *      Author: thiagordp
 */

#ifndef WIFI_H_
#define WIFI_H_

#include "ESP8266.h"

class Wifi
{

public:

	Wifi();

	virtual ~Wifi();

	/**
	 * Requisição GET ao servidor.
	 */
	void httpGetRequest(char *host, uint16_t port, char *service, char *json, char*response);

	/**
	 *  Requisição POST ao servidor.
	 */
	int8_t httpPostRequest(char *host, uint16_t port, char *service, char *json);

	/**
	 * Conectar à uma rede Wifi.
	 */
	int8_t connectWifi(char *networkName, char *password);

private:
	ESP8266 esp8266;
};

#endif /* WIFI_H_ */

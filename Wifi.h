/*
 * WifiModule.h
 *
 *  Created on: 4 de jun de 2017
 *      Author: thiagordp
 */

#ifndef WIFI_H_
#define WIFI_H_

#include "ESP8266.h"
#include "util_gateway.h"
#include <string.h>
#include <stdio.h>
#include <alarm.h> // Include EPOS para delay

using namespace EPOS;

class Wifi
{
public:

	Wifi()
	{

	}

	virtual ~Wifi()
	{

	}

	/**
	 * Requisição GET ao servidor.
	 */
	void httpGetRequest(char *host, uint16_t port, char *service, char *json, char*response)
	{
		char buf[400];
		//char str[100];
		clearBuffer(buf, 400);

		strcpy(buf, "GET ");
		strcat(buf, service);
		strcat(buf, " HTTP/1.1");
		strcat(buf, "\r\n");
		strcat(buf, "Host: ");
		strcat(buf, host);
		strcat(buf, ":");

		char tmp[6];

		sprintf(tmp, "%d", port);
		strcat(buf, tmp);
		strcat(buf, "\r\n");
		strcat(buf, "Content-Type: application/json");
		strcat(buf, "\r\n\r\n");

		esp8266.openConnection("TCP", host, port);
		Alarm::delay(1000000);
		esp8266.sendData(buf, strlen(buf), buf);

		// Extrair o conteúdo
		char *p = strstr(buf, "+IPD,");
		char *q = strstr(p + 1, "+IPD,") + 5;

		strtok(q, ":");
		p = strtok(NULL, "");
		sprintf(response, p);

		Alarm::delay(500000);

		esp8266.closeConnection();
	}

	/**
	 *  Requisição POST ao servidor.
	 */
	int8_t httpPostRequest(char *host, uint16_t port, char *service, char *json, char* response)
	{
		char buf[400];
		//char str[100];
		clearBuffer(buf, 400);

		strcpy(buf, "POST ");
		strcat(buf, service);
		strcat(buf, " HTTP/1.1");
		strcat(buf, "\r\n");
		strcat(buf, "Host: ");
		strcat(buf, host);
		strcat(buf, ":");

		char tmp[6];

		sprintf(tmp, "%d", port);
		strcat(buf, tmp);
		strcat(buf, "\r\n");
		strcat(buf, "Content-Type: application/json\r\n");
		strcat(buf, "Content-Length: ");
		sprintf(tmp, "%d", strlen(json));
		strcat(buf, tmp);
		strcat(buf, "\r\n\r\n");
		strcat(buf, json);
		strcat(buf, "\r\n");

		esp8266.openConnection(0, "TCP", host, port);
		esp8266.sendData(buf, strlen(buf), buf);

		if (findStrInBuffer(buf, "SEND OK") == 0)
			return 0;
		// Extrair o conteúdo
		char *p = strstr(buf, "+IPD,");
		char *q = strstr(p + 1, "+IPD,") + 5;

		strtok(q, ":");
		p = strtok(NULL, "");

		sprintf(response, p);

		esp8266.closeConnection();

		return 1;
	}

	/**
	 * Conectar à uma rede Wifi.
	 */
	int8_t connectWifi(char *networkName, char *password)
	{
		return esp8266.connectNetwork(networkName, password);
	}

	int8_t httpPostRequest2(char *host, uint16_t port, char *service, char *json, char* response)
	{
		char buf[400];
		//char str[100];
		clearBuffer(buf, 400);

		strcpy(buf, "POST ");
		strcat(buf, service);
		strcat(buf, " HTTP/1.1");
		strcat(buf, "\r\n");
		strcat(buf, "Host: ");
		strcat(buf, host);
		strcat(buf, ":");

		char tmp[6];

		sprintf(tmp, "%d", port);

		strcat(buf, tmp);
		strcat(buf, "\r\n");
		strcat(buf, "Content-Type: multipart/form-data; boundary=----borda\r\n");
		strcat(buf, "Content-Length: ");
		sprintf(tmp, "%d", strlen(json));
		strcat(buf, tmp);
		strcat(buf, "\r\n\r\n");
		strcat(buf, json);

		//Serial.println(buf);
		/*strcat(buf, "");
		 strcat(buf, "");
		 strcat(buf, "");
		 strcat(buf, "");
		 strcat(buf, "");
		 strcat(buf, "camera123\r\n");
		 strcat(buf, "----WebKitFormBoundary7MA\r\n");*/
		//strcat(buf, "Content-Length: ");
		//	sprintf(tmp, "%d", strlen(json));
		//strcat(buf, tmp);
		//strcat(buf, "\r\n\r\n");
		//strcat(buf, json);
		//strcat(buf, "\r\n");
		//return 1;
		if (esp8266.openConnection(0, "TCP", host, port))
		{
			esp8266.sendData(buf, strlen(buf), buf);

			if (findStrInBuffer(buf, "SEND OK") == 0)
				return RESPONSE_ERROR;
			// Extrair o conteúdo
			char *p = strstr(buf, "+IPD,");
			char *q = strstr(p + 1, "+IPD,") + 5;

			strtok(q, ":");
			p = strtok(NULL, "");

			sprintf(response, p);

			esp8266.closeConnection();

			return RESPONSE_OK;
		}
		return RESPONSE_ERROR;
	}

	int8_t receiveRequest()
	{
		esp8266.receiveRequest();
		return RESPONSE_OK;
	}

private:
	ESP8266 esp8266;
};

#endif /* WIFI_H_ */

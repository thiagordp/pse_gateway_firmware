/*
 * WifiModule.cpp
 *
 *  Created on: 4 de jun de 2017
 *      Author: thiagordp
 */

#include "Wifi.h"
#include "util_gateway.h"
#include <Arduino.h>
#include <string.h>

Wifi::Wifi()
{

}

Wifi::~Wifi()
{

}

int8_t Wifi::connectWifi(char *networkName, char *password)
{
	return esp8266.connectNetwork(networkName, password);
}

void Wifi::httpGetRequest(char *host, uint16_t port, char *service, char *json, char*response)
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
	delay(1000);
	esp8266.sendData(buf, strlen(buf), buf);

	// Extrair o conteúdo
	char *p = strstr(buf, "+IPD,");
	char *q = strstr(p + 1, "+IPD,") + 5;

	strtok(q, ":");
	p = strtok(NULL, "");
	sprintf(response, p);

	delay(500);

	esp8266.closeConnection();
}

int8_t Wifi::httpPostRequest(char *host, uint16_t port, char *service, char *json, char* response)
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


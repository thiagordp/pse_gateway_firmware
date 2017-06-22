/*
 * ESP8266.cpp
 *
 *  Created on: 14 de mai de 2017
 *      Author: thiagordp
 */

#include "esp_at_list.h"
#include <stdlib.h>

#include <string.h>
#include "ESP8266.h"

#include "util_gateway.h"

#if ARDUINO
#include<Arduino.h>
#else

#endif

//SerialRS232 serial;

ESP8266::ESP8266()
{
	serial.configure(9600, 8, 0, 2);
}

ESP8266::~ESP8266()
{

}

int8_t ESP8266::getStatus()
{
	int num = 10;
	char *cmd = (char*) calloc(num, sizeof(char));

	strcpy(cmd, AT_STATUS);
	strcat(cmd, AT_CMD_FINISH_STRING);

	serial.send(cmd, strlen(cmd));

	clearBuffer(cmd, num);

	serial.receive(cmd, num);
	num = getStatusFromResponse(cmd);
	free(cmd);

	return num;
}

// TODO: testar
char* ESP8266::getIp()
{
	char *buf = (char*) calloc(150, sizeof(char));
	char *ip = (char*) calloc(15, sizeof(char));

	strcpy(buf, AT_GET_IP);
	strcat(buf, AT_CMD_FINISH_STRING);

	serial.flush();
	serial.send(buf, strlen(buf));
	clearBuffer(buf, 150);
	serial.flush();
	serial.receive(buf, 150);

	char *pattern = "+CIFSR:STAIP,\"";

	char *tmp = strstr(buf, pattern) + strlen(pattern);
	int i;
	for (i = 0; *tmp != '\"'; i++, tmp++)
	{
		ip[i] = *tmp;
	}

	ip[i] = '\0';

	free(buf);
	return ip;
}

int8_t ESP8266::connectNetwork(char *ssid, char *password)
{
	// Formato: AT+CWJAP="ssid","password"\r\n
	char cmd[300];
	clearBuffer(cmd, 300);
	strcpy(cmd, AT_CONNECT_NETWORK);
	strcat(cmd, "=\"");
	strcat(cmd, ssid);
	strcat(cmd, "\",\"");
	strcat(cmd, password);
	strcat(cmd, "\"");
	strcat(cmd, AT_CMD_FINISH_STRING);

	serial.flush();
	serial.send(cmd, strlen(cmd));

	delay(5000);

	clearBuffer(cmd, 300);
	serial.receive(cmd, 300);

	if (strstr(cmd, "OK") != NULL || strstr(cmd, "CONNECTED") != NULL)
	{
		return 1;
	}
	else if (strstr(cmd, "ERROR"))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

char* ESP8266::getNetworks()
{
	char buf[500];

	strcpy(buf, AT_LIST_NETWORKS);
	strcat(buf, AT_CMD_FINISH_STRING);

	serial.flush();
	serial.send(buf, strlen(buf));
	strcpy(buf, "");
	serial.receive(buf, 500);
	serial.flush();

	return buf;
}

int8_t ESP8266::openConnection(char *type, char *ip, uint16_t port)
{
	return openConnection(0, type, ip, port);
}

int8_t ESP8266::openConnection(uint8_t id, char *type, char *ip, uint16_t port)
{
	char buf[100];
	clearBuffer(buf, 100);
	strcpy(buf, AT_OPEN_CONNECTION);
	strcat(buf, "=");

	if (this->cipmux == AT_MULTI_CONN_ON)
	{
		char tmp[5];
		sprintf(tmp, "%u", id);
		strcat(buf, tmp);
		strcat(buf, ",");
	}
	strcat(buf, "\"");
	if (strcmp(type, AT_CONNECT_TCP) == 0)
	{
		strcat(buf, AT_CONNECT_TCP);
	}
	else if (strcmp(type, AT_CONNECT_UDP) == 0)
	{
		strcat(buf, AT_CONNECT_UDP);
	}
	else
	{
#if ARDUINO
		Serial.println("Tipo incorreto de conexão");
#endif
	}

	strcat(buf, "\",\"");
	strcat(buf, ip);
	strcat(buf, "\",");
	char buftmp[6];
	sprintf(buftmp, "%d", port);
	strcat(buf, buftmp);

	strcat(buf, AT_CMD_FINISH_STRING);

	serial.flush();

	serial.send(buf, strlen(buf));
	clearBuffer(buf, 100);
	serial.receive(buf, 100);

	if (findStrInBuffer(buf, "OK"))
	{
		return 1;
	}
	else if (findStrInBuffer(buf, "ERROR"))
	{
		return 0;
	}

	return -1;
}

int8_t ESP8266::closeConnection(uint8_t id)
{
	char buf[200];
	clearBuffer(buf, 200);

	strcpy(buf, AT_CLOSE);

	if (this->cipmux == AT_MULTI_CONN_ON)
	{
		char tmp[5];
		sprintf(tmp, "=%u", id);
		strcat(buf, tmp);
	}

	strcat(buf, AT_CMD_FINISH_STRING);
	serial.flush();
	serial.send(buf, strlen(buf));
	strcpy(buf, "");
	serial.receive(buf, 200);

	if (findStrInBuffer(buf, "CLOSED") == 0)
		return 1;

	return 0;
}

int8_t ESP8266::closeConnection(void)
{
	char buf[100];
	clearBuffer(buf, 100);

	strcpy(buf, AT_CLOSE);
	strcat(buf, AT_CMD_FINISH_STRING);

	serial.flush();
	serial.send(buf, strlen(buf));
	strcpy(buf, "");
	serial.receive(buf, 1000);

	if (findStrInBuffer(buf, "CLOSED") == 0)
		return 1;

	return 0;
}

void ESP8266::receiveRequest()
{

}

void ESP8266::reset()
{
	char *buf = (char*) calloc(10, sizeof(char));
	clearBuffer(buf, 10);

	strcpy(buf, AT_RESET);
	strcat(buf, AT_CMD_FINISH_STRING);

	serial.flush();
	serial.send(buf, strlen(buf));
	clearBuffer(buf, 10);
	serial.flush();

	free(buf);
}

void ESP8266::sendData(char *buf, uint64_t length, char* response)
{
	char cmd[350];

	strcpy(cmd, AT_SEND_DATA); // copia o comando
	strcat(cmd, "=");			// concatena '='
	char tmp[5];

	sprintf(tmp, "%lu", length);
	strcat(cmd, tmp);			// concatena o comprimento dos dados
	strcat(cmd, AT_CMD_FINISH_STRING);
	serial.flush();
	serial.send(cmd, strlen(cmd));
	clearBuffer(cmd, 350);
	serial.receive(cmd, 350);

	if (findStrInBuffer(cmd, ">") == 1)
	{
		clearBuffer(cmd, 350);

		serial.flush();
		serial.send(buf, length);
		clearBuffer(cmd, 350);
		serial.receive(cmd, 350);

		strcpy(response, cmd);
	}
}

void ESP8266::setupMode(uint8_t mode)
{
	char *cmd = (char*) calloc(10, sizeof(char));

	strcpy(cmd, AT_MODE); // copia o comando
	strcat(cmd, "=");			// concatena '='
	char tmp[5];

	sprintf(tmp, "%u", mode);

	strcat(cmd, tmp);			// concatena o comprimento dos dados

	serial.flush();
	serial.send(cmd, strlen(cmd));
	free(cmd);

	cmd = (char*) calloc(10, sizeof(char));

	cwmode = mode;
	delay(500);
	serial.receive(cmd, 10);
	free(cmd);
}

void ESP8266::enableServer(uint16_t port)
{

}

void ESP8266::disableServer()
{

}

void ESP8266::setupMuxConnection(uint8_t cipmux)
{
	char *cmd = (char*) calloc(10, sizeof(char));

	strcpy(cmd, AT_MULTI_CONN); // copia o comando
	strcat(cmd, "=");			// concatena '='
	char tmp[5];

	sprintf(tmp, "%u", cipmux);

	strcat(cmd, tmp);			// concatena o comprimento dos dados
	strcat(cmd, AT_CMD_FINISH_STRING);

	serial.send(cmd, strlen(cmd));
	free(cmd);

	this->cipmux = cipmux;

	cmd = (char*) calloc(30, sizeof(char));

	serial.receive(cmd, 30);

	free(cmd);
}

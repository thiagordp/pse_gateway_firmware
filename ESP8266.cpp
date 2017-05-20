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

SerialRS232 serial;

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
	char *buf = (char*) calloc(300, sizeof(char));
	char *ip = (char*) calloc(15, sizeof(char));

	strcpy(buf, AT_GET_IP);
	strcat(buf, AT_CMD_FINISH_STRING);

	serial.send(buf, strlen(buf));
	clearBuffer(buf, 300);
	serial.receive(buf, 300);

	char *pattern = "+CIFSR:STAIP,\"";

	char *tmp = strstr(buf, pattern) + strlen(pattern);

	for (int i = 0; *tmp != '\"'; i++, tmp++)
	{
		ip[i] = *tmp;
	}

	free(buf);
	return ip;
}

// TODO: testar
int8_t ESP8266::connectNetwork(char *ssid, char *password)
{
	// Formato: AT+CWJAP="ssid","password"\r\n
	char *cmd = (char*) calloc(300, sizeof(char));

	strcpy(cmd, AT_CONNECT_NETWORK);
	strcat(cmd, "=\"");
	strcat(cmd, ssid);
	strcat(cmd, "\",\"");
	strcat(cmd, password);
	strcat(cmd, "\"");
	strcat(cmd, AT_CMD_FINISH_STRING);
	Serial.println(cmd);
	serial.flush();
	serial.send(cmd, strlen(cmd));

	delay_ms(10000);

	Serial.println(serial.receive(cmd, 300));
	Serial.println(cmd);

	free(cmd);

	if (strstr(cmd, "OK"))
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

// TODO: testar
char* ESP8266::getNetworks()
{
	char *buf = (char*) calloc(1000, sizeof(char));

	strcpy(buf, AT_LIST_NETWORKS);
	strcat(buf, AT_CMD_FINISH_STRING);

	serial.flush();
	serial.send(buf, strlen(buf));
	strcpy(buf, "");
	serial.receive(buf, 1000);
	serial.flush();

	return buf;
}

// TODO: testar
void ESP8266::openConnection(uint8_t id, char *type, char *ip, char *port)
{
	char *buf = (char*) calloc(1000, sizeof(char));

	strcpy(buf, AT_OPEN_CONNECTION);
	strcat(buf, "=");

	if (this->cipmux == AT_MULTI_CONN_ON)
	{
		char tmp[5];
		sprintf(tmp, "%u", id);
		strcat(buf, tmp);
		strcat(buf, ",");
	}

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

	strcat(buf, "\"");
	strcat(buf, ip);
	strcat(buf, "\",\"");
	strcat(buf, port);
	strcat(buf, "\"");

	strcat(buf, AT_CMD_FINISH_STRING);

	serial.flush();
	serial.send(buf, strlen(buf));
	strcpy(buf, "");
	serial.receive(buf, 1000);

	/***
	 * TODO: FAZER VERIFICAÇÕES DA RESPOSTA: ok, ERROR, ALREADY CONNECT
	 */
}

// TODO: testar
int8_t ESP8266::closeConnection(uint8_t id)
{
	char *buf = (char*) calloc(1000, sizeof(char));

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
	serial.receive(buf, 1000);

	return 1;
}

int8_t ESP8266::closeConnection(void)
{
	char *buf = (char*) calloc(1000, sizeof(char));

	strcpy(buf, AT_CLOSE);
	strcat(buf, AT_CMD_FINISH_STRING);

	serial.flush();
	serial.send(buf, strlen(buf));
	strcpy(buf, "");
	serial.receive(buf, 1000);

	return 1;
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

void ESP8266::sendData(char *buf, uint64_t length)
{
	char *cmd = (char*) calloc(10, sizeof(char));

	strcpy(cmd, AT_SEND_DATA); // copia o comando
	strcpy(cmd, "=");			// concatena '='
	char tmp[5];

	sprintf(tmp, "%llu", length);

	strcpy(cmd, tmp);			// concatena o comprimento dos dados

	serial.send(cmd, strlen(cmd));
	free(cmd);

	cmd = (char*) calloc(10, sizeof(char));

	serial.receive(cmd, 10);

	if (strstr(cmd, ">") != NULL)
	{
		serial.send(buf, length);
	}

	//TODO: Rever os próximos passos a serem realizados casos seja enviado com sucesso ou não
}

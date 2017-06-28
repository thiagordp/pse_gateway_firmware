/*
 * ESP8266.h
 *
 *  Created on: 14 de mai de 2017
 *      Author: thiagordp
 */

#ifndef ESP8266_H_
#define ESP8266_H_

//#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <alarm.h> 

#include "util_gateway.h"
#include "SerialRS232.h"
#include "esp_at_list.h"

#if ARDUINO
#include<Arduino.h>
#else
//TODO: Inclusão da biblioteca de delay do EPOS
#endif

using namespace EPOS;

class ESP8266
{
public:

	ESP8266()
	{
		cipmux = 0;
		cwmode = 0;
		serial.configure(9600, 8, 0, 2);
	}

	virtual ~ESP8266()
	{

	}

	char* getIp()
	{
		char *buf = (char*) calloc(150, sizeof(char));
		char *ip = (char*) calloc(15, sizeof(char));
		//char *buf = new char[150];
		//char *ip = new char[15];

		strcpy(buf, AT_GET_IP);
		strcat(buf, AT_CMD_FINISH_STRING);

		serial.flush();
		serial.send(buf);
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

		delete(buf);
		return ip;
	}

	int8_t getStatus()
	{
		int num = 10;
		char *cmd = (char*) calloc(num, sizeof(char));

		strcpy(cmd, AT_STATUS);
		strcat(cmd, AT_CMD_FINISH_STRING);

		serial.send(cmd);

		clearBuffer(cmd, num);

		serial.receive(cmd, num);
		num = getStatusFromResponse(cmd);
		free(cmd);

		return num;
	}

	int8_t connectNetwork(char *ssid, char *password)
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
		serial.send(cmd);

		Alarm::delay(7000000);
		clearBuffer(cmd, 300);
		serial.receive(cmd, 300);

		if (strstr(cmd, "OK") != NULL || strstr(cmd, "CONNECTE") != NULL)
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

	char* getNetworks()
	{
		char *buf = (char*) calloc(500, sizeof(char));

		strcpy(buf, AT_LIST_NETWORKS);
		strcat(buf, AT_CMD_FINISH_STRING);

		serial.flush();
		serial.send(buf);
		strcpy(buf, "");
		serial.receive(buf, 500);
		serial.flush();

		return buf;
	}

	int8_t openConnection(char *type, char *ip, uint16_t port)
	{
		return openConnection(0, type, ip, port);
	}

	int8_t openConnection(uint8_t id, char *type, char *ip, uint16_t port)
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

		serial.send(buf);
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

	int8_t closeConnection(void)
	{
		char buf[100];
		clearBuffer(buf, 100);

		strcpy(buf, AT_CLOSE);
		strcat(buf, AT_CMD_FINISH_STRING);

		serial.flush();
		serial.send(buf);
		strcpy(buf, "");
		serial.receive(buf, 1000);

		if (findStrInBuffer(buf, "CLOSED") == 0)
			return 1;

		return 0;
	}

	int8_t closeConnection(uint8_t id)
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
		serial.send(buf);
		strcpy(buf, "");
		serial.receive(buf, 200);

		if (findStrInBuffer(buf, "CLOSED") == 0)
			return 1;

		return 0;
	}

	void receiveRequest()
	{

	}

	void reset(void)
	{
		char *buf = (char*) calloc(10, sizeof(char));
		clearBuffer(buf, 10);

		strcpy(buf, AT_RESET);
		strcat(buf, AT_CMD_FINISH_STRING);

		serial.flush();
		serial.send(buf);
		clearBuffer(buf, 10);
		serial.flush();

		free(buf);
	}

	void sendData(char *buf, uint64_t length, char* response)
	{
		char cmd[350];

		strcpy(cmd, AT_SEND_DATA); // copia o comando
		strcat(cmd, "="); // concatena '='
		char tmp[5];

		sprintf(tmp, "%lu", length);
		strcat(cmd, tmp); // concatena o comprimento dos dados
		strcat(cmd, AT_CMD_FINISH_STRING);
		serial.flush();

		serial.send(cmd);
		clearBuffer(cmd, 350);
		serial.receive(cmd, 350);

		if (findStrInBuffer(cmd, ">") == 1)
		{
			clearBuffer(cmd, 350);

			serial.flush();
			serial.send(buf);
			clearBuffer(cmd, 350);
			serial.receive(cmd, 350);

			strcpy(response, cmd);
		}
	}

	void setupMode(uint8_t mode)
	{
		char *cmd = (char*) calloc(10, sizeof(char));

		strcpy(cmd, AT_MODE); // copia o comando
		strcat(cmd, "="); // concatena '='
		char tmp[5];

		sprintf(tmp, "%u", mode);

		strcat(cmd, tmp); // concatena o comprimento dos dados

		serial.flush();
		serial.send(cmd);
		free(cmd);

		cmd = (char*) calloc(10, sizeof(char));

		cwmode = mode;
		Alarm::delay(500000);
		serial.receive(cmd, 10);
		free(cmd);
	}

	void enableServer(uint16_t port)
	{

	}

	void disableServer()
	{

	}

	void setupMuxConnection(uint8_t cipmux)
	{
		char *cmd = (char*) calloc(10, sizeof(char));

		strcpy(cmd, AT_MULTI_CONN); // copia o comando
		strcat(cmd, "="); // concatena '='
		char tmp[5];

		sprintf(tmp, "%u", cipmux);

		strcat(cmd, tmp); // concatena o comprimento dos dados
		strcat(cmd, AT_CMD_FINISH_STRING);

		serial.send(cmd);
		free(cmd);

		this->cipmux = cipmux;

		cmd = (char*) calloc(30, sizeof(char));

		serial.receive(cmd, 30);

		free(cmd);
	}

private:
	SerialRS232 serial;
	uint8_t cipmux;
	uint8_t cwmode;

};

#endif /* ESP8266_H_ */


/*
 * ESP8266.h
 *
 *  Created on: 14 de mai de 2017
 *      Author: thiagordp
 */

#ifndef ESP8266_H_
#define ESP8266_H_

#include "SerialRS232.h"

class ESP8266
{
public:
	ESP8266();

	virtual ~ESP8266();

	char* getIp();

	int8_t getStatus();

	int8_t connectNetwork(char *ssid, char *password);

	char* getNetworks();

	int8_t openConnection(char *type, char *ip, uint16_t port);

	int8_t openConnection(uint8_t id, char *type, char *ip, uint16_t port);

	int8_t closeConnection(void);

	int8_t closeConnection(uint8_t id);

	void receiveRequest();

	void reset(void);

	void sendData(char *buf, uint64_t length, char* response);

	void setupMode(uint8_t mode);

	void enableServer(uint16_t port);

	void disableServer();

	void setupMuxConnection(uint8_t cipmux);

private:
	SerialRS232 serial;
	uint8_t cipmux = 0;
	uint8_t cwmode = 0;

};

#endif /* ESP8266_H_ */


#ifndef GATEWAY_H_PSE
#define GATEWAY_H_PSE

#include "zigbee.h"
#include "defines.h"
#include "../include/machine/cortex/uart.h"
#include "Wifi.h"
#include <string.h>
#include <stdio.h>

using namespace EPOS;

class Gateway
{
public:
	Gateway()
	{
		this->host = "192.168.1.47";
		this->port = 8000;		
		this->bytesPhoto = 50;
		for (char i = 0; i < 4; i++)
			this->dispositives[i] = (Dispositive*) 0;
	}
	void TreatZigBee()
	{
		Frame* f = zigbee.ReadPacket();
		if (f != (Frame*) 0)
		{
			auto d = f->data<char>();
			char id = d[0];

			switch (id & 0x0F)
			{
			case ID_TYPE_ULTRASSOM:

				if (id == ID_TYPE_ULTRASSOM)
				{
					//Atualizando status no servidor
					UpdateStatus(ID_TYPE_ULTRASSOM, dispositives[ID_TYPE_ULTRASSOM]->id, d[2]);
					//Habilita o atuador
					EnableDispositive(ID_TYPE_ATUADOR, dispositives[ID_TYPE_ATUADOR]->id);
					//Requisita a foto
					RequestPhoto(dispositives[ID_TYPE_CAMERA]->id);
				}
				else
				{
					//Salvando o dispositivo, d[1] = ID
					dispositives[0] = new Dispositive(ID_TYPE_ULTRASSOM, d[1], f->src());
					AddDevice(ID_TYPE_ULTRASSOM, d[1]);
				}

				break;

			case ID_TYPE_ATUADOR:

				if (id == ID_TYPE_ATUADOR)
				{
					//Atualizando status no servidor
					UpdateStatus(ID_TYPE_ATUADOR, dispositives[ID_TYPE_ATUADOR]->id, d[2]);
				}
				else
				{
					//Salvando o dispositivo, d[1] = ID
					dispositives[1] = new Dispositive(ID_TYPE_ATUADOR, d[1], f->src());
					AddDevice(ID_TYPE_ATUADOR, d[1]);
				}

				break;

			case ID_TYPE_FUMACA:

				if (id == ID_TYPE_FUMACA)
				{
					//No servidor reconhece que é fumaça habilita também o atuador
					UpdateStatus(ID_TYPE_FUMACA, dispositives[ID_TYPE_FUMACA]->id, TRUE);
					//Habilita o atuador
					EnableDispositive(ID_TYPE_ATUADOR, dispositives[ID_TYPE_ATUADOR]->id);
				}
				else
				{
					//Salvando o dispositivo, d[1] = ID
					dispositives[2] = new Dispositive(ID_TYPE_FUMACA, d[1], f->src());
					AddDevice(ID_TYPE_FUMACA, d[1]);
				}

				break;

			case ID_TYPE_CAMERA:
				//d[0] | d[1] |...| d[5]
				//ID_TYPE_CAMERA | ID | SEQ | TOTAL |NUM_BYTES | DATA
				if (id == ID_TYPE_CAMERA)
				{
					//Preenchendo vetor de bytes da foto
					for (unsigned int i = 0; i < (unsigned int) d[4]; i++)
						photo[d[2] * bytesPhoto + i] = d[5 + i];

					//Verificando se é o último pacote
					if (d[2] == d[3])
						SendPicture(d[1], "foto", "png");

				}
				else
				{
					//Salvando o dispositivo, d[1] = ID
					dispositives[3] = new Dispositive(ID_TYPE_CAMERA, d[1], f->src());
					AddDevice(ID_TYPE_CAMERA, (char) d[1]);
				}

				break;
			}
		}
	}

	void RequestAddress()
	{
		char payload[] =
		{ ID_TYPE_BROADCAST, CMD_GET_INFO };

		zigbee.SendPacketBroadcast(payload, 2);
	}

	void RequestPhoto(char pId)
	{
		char payload[] =
		{ ID_TYPE_GATEWAY, CMD_GET_PHOTO };

		ShortAddress address = dispositives[ID_TYPE_CAMERA]->address;

		zigbee.SendPacket(payload, 2, address);
	}

	void EnableDispositive(char pType, char pId)
	{
		char payload[] =
		{ ID_TYPE_GATEWAY, CMD_ENABLE };

		ShortAddress address = dispositives[pType]->address;

		zigbee.SendPacket(payload, 2, address);
	}

	void DisableDispositive(char pType, char pId)
	{
		char payload[] =
		{ ID_TYPE_GATEWAY, CMD_DISABLE };

		ShortAddress address = dispositives[pType]->address;

		zigbee.SendPacket(payload, 2, address);
	}

	void TreatWiFi()
	{
		wifi.receiveRequest();
	}

	void SendPicture(char pCameraId, char* pName, char *pExtension)
	{
		char response[100];
		char content_request[500];

		strcpy(content_request, "");

		/**
		 * Montagem da URL
		 */
		/**
		 ----borda\r\nContent-Disposition: form-data; name=\"camera_id\"\r\n\r\n123\r\n----borda\r\nContent-Disposition: form-data; name=\"name\"\r\n\r\ncamera123\r\n----borda\r\n");
		 */
		strcat(content_request, "----borda\r\nContent-Disposition: form-data; name=\"camera_id\"\r\n\r\n");
		char tmp[4];
		sprintf(tmp, "%c", pCameraId);
		strcat(content_request, tmp);

		strcat(content_request, "\r\n----borda\r\nContent-Disposition: form-data; name=\"name\"\r\n\r\n");
		strcat(content_request, pName);
		strcat(content_request, "\r\n----borda\r\n");

		wifi.httpPostRequest2(host, port, "/send-image", content_request, response);
	}

	void UpdateStatus(char pType, char pId, char pStatus)
	{
		char response[100];
		char content_request[500];

		strcpy(content_request, "");

		/**
		 * Montagem da URL
		 */
		strcat(content_request, "----borda\r\nContent-Disposition: form-data; name=\"device_id\"\r\n\r\n");
		char tmp[4];
		sprintf(tmp, "%c", pId);
		strcat(content_request, tmp);

		strcat(content_request, "\r\n----borda\r\nContent-Disposition: form-data; name=\"type\"\r\n\r\n");
		sprintf(tmp, "%c", pType);
		strcat(content_request, tmp);

		strcat(content_request, "\r\n----borda\r\nContent-Disposition: form-data; name=\"status\"\r\n\r\n");
		sprintf(tmp, "%c", pStatus);
		strcat(content_request, tmp);
		strcat(content_request, "\r\n----borda\r\n");

		wifi.httpPostRequest2(host, port, "/send-capture", content_request, response);
	}

	void AddDevice(char pType, char pId)
	{
		char response[100];
		char content_request[500];

		strcpy(content_request, "");

		/**
		 * Montagem da URL
		 */
		strcat(content_request, "----borda\r\nContent-Disposition: form-data; name=\"device_id\"\r\n\r\n");
		char tmp[4];
		sprintf(tmp, "%c", pId);
		strcat(content_request, tmp);

		strcat(content_request, "\r\n----borda\r\nContent-Disposition: form-data; name=\"type\"\r\n\r\n");
		sprintf(tmp, "%c", pType);
		strcat(content_request, tmp);

		strcat(content_request, "\r\n----borda\r\n");

		wifi.httpPostRequest2(host, port, "/send-capture", content_request, response);
	}

private:
	char *host;
	uint32_t port;
	ZigBee zigbee;
	Wifi wifi;
	Dispositive* dispositives[4];
	char photo[14400];
	unsigned char bytesPhoto;
};
#endif
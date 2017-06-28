#include "zigbee.h"
#include "defines.h"
#include "../include/machine/cortex/uart.h"

using namespace EPOS;

class Gateway
{
public:
	Gateway()
	{
		bytesPhoto = 50;
		for(char i = 0; i < 4; i++)
			dispositives[i] = (Dispositive*)0;
	}
	void TreatZigBee()
	{	
		Frame* f = zigbee.ReadPacket();
		if (f != (Frame*)0)
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
                        dispositives[0] = new Dispositive(ID_TYPE_ULTRASSOM, d[1], f->src());
                        AddDevice(ID_TYPE_ULTRASSOM, d[1]);
                    }

                break;

                case ID_TYPE_ATUADOR:

                    if (id == ID_TYPE_ATUADOR)
                    {
                        UpdateStatus(ID_TYPE_ATUADOR, dispositives[ID_TYPE_ATUADOR]->id, d[2]);
                    }
                    else
                    {
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
                        dispositives[2] = new Dispositive(ID_TYPE_FUMACA, d[1], f->src());
                        AddDevice(ID_TYPE_FUMACA, d[1]);
                    }

                break;

                case ID_TYPE_CAMERA:
                    //ID_TYPE_CAMERA | ID | SEQ | TOTAL |NUM_BYTES | DATA
                    if (id == ID_TYPE_CAMERA)
                    {
                        for(unsigned int i = 0; i < (unsigned int)d[4]; i++)
                        	photo[d[2] * bytesPhoto + i] = d[5 + i];

                        //Verificando se é o último pacote	
                        if (d[2] == d[3])
                        	SendPicture();

                    }
                    else
                    {
                        dispositives[3] = new Dispositive(ID_TYPE_CAMERA, d[1], f->src());
                        AddDevice(ID_TYPE_CAMERA, d[1]);
                    }
                                   
                break;                                                
            }
		}
	}

	void RequestAddres()
	{
		char payload[] = {ID_TYPE_BROADCAST, CMD_GET_INFO};

		zigbee.SendPacketBroadcast(payload, 2);
	}

	void RequestPhoto(char pId)
	{
		char payload[] = {ID_TYPE_GATEWAY, CMD_GET_PHOTO};

		ShortAddress address = dispositives[ID_TYPE_CAMERA]->address;

		zigbee.SendPacket(payload, 2, address);		
	}

	void EnableDispositive(char pType, char pId)
	{
		char payload[] = {ID_TYPE_GATEWAY, CMD_ENABLE};

		ShortAddress address = dispositives[pType]->address;

		zigbee.SendPacket(payload, 2, address);
	}

	void DisableDispositive(char pType, char pId)
	{
		char payload[] = {ID_TYPE_GATEWAY, CMD_DISABLE};

		ShortAddress address = dispositives[pType]->address;

		zigbee.SendPacket(payload, 2, address);
	}

	void TreatWiFi()
	{

	}

	void SendPicture()
	{

	}

	void UpdateStatus(char pType, char pId, char pStatus)
	{

	}

	void AddDevice(char pType, char pId)
	{

	}

private:
	ZigBee zigbee;
	Dispositive* dispositives[4];
	char photo[14400];
	unsigned char bytesPhoto;
};
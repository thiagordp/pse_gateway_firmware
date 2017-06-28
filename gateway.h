#include "zigbee.h"
#include "defines.h"

class Gateway
{
public:
	Gateway()
	{
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
                        //cout << "ULTRASOM (" << d[1] << ") STATUS " << d[2] << endl;

                    	//Atualizando status no servidor
                    	UpdateStatus(ID_TYPE_ULTRASSOM, dispositives[ID_TYPE_ULTRASSOM]->id, d[2]);
                        //Habilita o atuador
                        EnableDispositive(ID_TYPE_ATUADOR, dispositives[ID_TYPE_ATUADOR]->id); 
                        //Requisita a foto
                        RequestPhoto(dispositives[ID_TYPE_CAMERA]->id);                       
                    }
                    else
                    {
                        //cout << "ULTRASOM SAVE " << f->src() << endl;
                        dispositives[0] = new Dispositive(ID_TYPE_ULTRASSOM, d[1], f->src());
                        AddDevice(ID_TYPE_ULTRASSOM, d[1]);
                    }

                break;

                case ID_TYPE_ATUADOR:

                    if (id == ID_TYPE_ATUADOR)
                    {
                        //cout << "ATUADOR (" << d[1] << ") STATUS " << d[2] << endl;
                        UpdateStatus(ID_TYPE_ATUADOR, dispositives[ID_TYPE_ATUADOR]->id, d[2]);
                    }
                    else
                    {
                        //cout << "ATUADOR SAVE " << f->src() << endl;
                        dispositives[1] = new Dispositive(ID_TYPE_ATUADOR, d[1], f->src());
                        AddDevice(ID_TYPE_ATUADOR, d[1]);
                    }

                break;

                case ID_TYPE_FUMACA:

                    if (id == ID_TYPE_FUMACA)
                    {
                        //cout << "FUMACA (" << d[1] << ") STATUS " << d[2] << endl;

						//No servidor reconhece que é fumaça habilita também o atuador
                        UpdateStatus(ID_TYPE_FUMACA, dispositives[ID_TYPE_FUMACA]->id, TRUE); 
                        //Habilita o atuador
                        EnableDispositive(ID_TYPE_ATUADOR, dispositives[ID_TYPE_ATUADOR]->id);
                    }
                    else
                    {
                        //cout << "FUMACA SAVE " << f->src() << endl;
                        dispositives[2] = new Dispositive(ID_TYPE_FUMACA, d[1], f->src());
                        AddDevice(ID_TYPE_FUMACA, d[1]);
                    }

                break;

                case ID_TYPE_CAMERA:
                    //ID_TYPE_CAMERA | ID | SEQ | TOTAL |NUM_BYTES | DATA
                    if (id == ID_TYPE_CAMERA)
                    {
                        //cout << "CAMERA (" << d[1] << ") |" << d[2] << "|" << d[3] << "|" << d[4] << endl;
                        for(unsigned int i = 0; i < (unsigned int)d[4]; i++)
                        	photo[d[2] * bytesPhoto + i] = d[5 + i];

                        //Verificando se é o último pacote	
                        if (d[2] == d[3])
                        	SendPicture();

                    }
                    else
                    {
                        //cout << "CAMERA SAVE " << f->src() << endl;
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

		NIC * nic = new NIC();
		ShortAddress address = nic->broadcast();

		zigbee.SendPacket(payload, 2, address);
	}

	void RequestPhoto(char pId)
	{
		char payload[] = {ID_TYPE_GATEWAY, CMD_GET_PHOTO};

		ShortAddress address = *dispositives[ID_TYPE_CAMERA].address;

		zigbee.SendPacket(payload, 2, address);		
	}

	void EnableDispositive(char pType, char pId)
	{
		char payload[] = {ID_TYPE_GATEWAY, CMD_ENABLE};

		ShortAddress address = *dispositives[pType].address;

		zigbee.SendPacket(payload, 2, address);
	}

	void DisableDispositive(char pType, char pId)
	{
		char payload[] = {ID_TYPE_GATEWAY, CMD_DISABLE};

		ShortAddress address = *dispositives[pType].address;

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
	const unsigned char bytesPhoto = 50;
};
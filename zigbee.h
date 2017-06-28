#include "../include/machine/cortex/nic.h"
#include "../include/machine/cortex/uart.h"
#include <alarm.h>
#include "utility/ostream.h"

using namespace EPOS;
typedef IEEE802_15_4::Frame Frame;
typedef IEEE802_15_4::Short_Address ShortAddress;

UART uart1(0, 9600, 8, 0, 1);

class Dispositive
{
    typedef IEEE802_15_4::Short_Address ShortAddress;
public:
    Dispositive(char pType, char pId, ShortAddress pAddress) : type(pType), id(pId), address(pAddress) {}
    char type;
    char id;
    ShortAddress address;
};

class Receiver : public IEEE802_15_4::Observer
{
    typedef IEEE802_15_4::Protocol Protocol;
    typedef IEEE802_15_4::Buffer Buffer;
    typedef IEEE802_15_4::Frame Frame;
    typedef IEEE802_15_4::Observed Observed;
public:    
    Receiver(const Protocol & pProtocol, NIC * pNic) : protocol(pProtocol), nic(pNic)
    {
        this->begin = this->end = 0;

        for (char i = 0; i < 16; i++)
            this->frame[i] = (Frame*)0; 

        this->nic->attach(this, protocol);         
    }

    void update(Observed * o, Protocol p, Buffer * b)
    {
        if (p == this->protocol)
        {
            {
                Frame* f = b->frame();
                auto d = f->data<char>();
                char a = 'a' + d[1];
                uart1.put(a);
                uart1.put('\n'); 
            }

            Frame* f = new Frame( *(b->frame()) );
            *f = *(b->frame());

            if (this->frame[end] != (Frame*)0)
                delete(this->frame[end]);

            this->frame[end] = f;

            if (this->end == 15 && this->begin == 0)
                this->begin = 1;
            else if (this->begin - this->end == 1)
                this->begin = (this->begin + 1) % 16;

            this->end = (this->end + 1) % 16;

            nic->free(b);
            //auto d = f->data<data_type>();
        }   
    }

    Frame* GetPacket()
    {
        if (this->begin != this->end)
        {
            Frame* f = this->frame[begin];
            this->frame[begin] = (Frame*)0;
            this->begin = (this->begin + 1) % 16;
            return f;            
        }
        return (Frame*)0; 
    }
private:
    Protocol protocol;
    NIC * nic;
    Frame* frame[16];
    unsigned long begin;
    unsigned long end;    
};

class ZigBee
{
    typedef IEEE802_15_4::Frame Frame;
    typedef IEEE802_15_4::Short_Address ShortAddress;
public:

    ZigBee()
    {
        this->nic = new NIC();
        this->receiver = new Receiver(NIC::ELP, nic); 
    }

    Frame* ReadPacket()
    {
        this->receiver->GetPacket();
    }

    void SendPacket(char* pPayload, unsigned char pSize, ShortAddress pAddress)
    {
        this->nic->send(pAddress, NIC::ELP, pPayload, pSize);
    }

    void SendPacketBroadcast(char* pPayload, unsigned char pSize)
    {
        this->nic->send(nic->broadcast(), NIC::ELP, pPayload, pSize);
    }    

private:
    Receiver* receiver;
    NIC * nic;
};
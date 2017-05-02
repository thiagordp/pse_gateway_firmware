#include "SerialPse.h"

#if EPOS
#include "../include/machine/cortex/uart.h"

UART uart0(0, 9600, 8, 0, 1);

int8_t SerialPse::configure(uint32_t baude_rate, uint32_t data_bits, uint32_t parity, uint32_t stop_bits)
{
	//config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits)
	uart0.config(baude_rate, data_bits, parity, stop_bits);
}

int8_t SerialPse::send(char *buf, uint16_t len)
{
	for (uint16_t i = 0; (buf[i] != '\0') && (i < len); i++)
	{
		if(uart0.ready_to_put())
			uart0.put(buf[i]);
		else
			return i;		
	}
}

int8_t SerialPse::receive(char *buf, uint16_t len)
{
	for (uint16_t i = 0; i < len; i++)
	{
		if(uart0.ready_to_get())
			buf[i] = uart0.get();
		else
			break;
	}

	return i;
}

int8_t SerialPse::available(void)
{
	if(uart0.ready_to_get())
		return 1;
	else
		return -1;
}
#else

#endif
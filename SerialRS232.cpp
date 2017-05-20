#include "SerialRS232.h"

#if EPOS
#include "../include/machine/cortex/uart.h"

UART uart0(0, 9600, 8, 0, 1);

int8_t SerialRS232::configure(uint32_t baude_rate, uint32_t data_bits, uint32_t parity, uint32_t stop_bits)
{
	//config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits)
	uart0.config(baude_rate, data_bits, parity, stop_bits);
}

int8_t SerialRS232::send(char *buf, uint16_t len)
{
	for (uint16_t i = 0; (buf[i] != '\0') && (i < len); i++)
	{
		if (uart0.ready_to_put())
		uart0.put(buf[i]);
		else
		return i;
	}
}

int8_t SerialRS232::receive(char *buf, uint16_t len)
{
	for (uint16_t i = 0; i < len; i++)
	{
		if (uart0.ready_to_get())
		buf[i] = uart0.get();
		else
		break;
	}

	return i;
}

int8_t SerialRS232::available(void)
{
	if (uart0.ready_to_get())
	return 1;
	else
	return -1;
}
#else

#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial uart(2, 3);

int8_t SerialRS232::configure(uint32_t baude_rate, uint32_t data_bits,
		uint32_t parity, uint32_t stop_bits)
{
	/*  uint8_t cfg_byte = 0b00000000;

	 cfg_byte |= (data_bits - 5) << 1; // bits 1, 2, cfg de data_bits
	 cfg_byte |= (stop_bits - 1) << 3; // bit 3, cfg de stop bits
	 cfg_byte |= (parity == 2 ? PARITY_EVEN : parity == 1 ? PARITY_ODD : PARITY_NONE) << 4; // bits 4, cfg de paridade

	 uart.begin(baude_rate, cfg_byte);
	 */

	uart.begin(baude_rate);

	return 1;
}

int8_t SerialRS232::send(char *buf, int16_t len)
{
	return uart.print(buf);
}

int8_t timedRead()
{
	int c;
	uint64_t _timeout = SERIAL_TIMEOUT;
	uint64_t _startMillis = millis();
	do
	{
		c = uart.read();
		if (c >= 0)
			return c;
	} while (millis() - _startMillis < _timeout);

	return -1;     // -1 indicates timeout
}

int8_t SerialRS232::receive(char *buf, uint16_t len)
{
	size_t count = 0;
	while (count < len)
	{
		int c = timedRead();
		if (c < 0)
			break;
		*buf++ = (char) c;
		count++;
	}
	return count;
}

int8_t SerialRS232::available(void)
{
	Serial.print("ava... ");
	Serial.println(uart.available());

	if (uart.available() > 0)
		return 1;
	else
		return -1;
}

int8_t SerialRS232::flush(void)
{
	while (uart.available())
	{
		timedRead();
		delay(1);
	}

	return 1;
}

#endif

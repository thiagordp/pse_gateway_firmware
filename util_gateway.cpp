#include "util_gateway.h"
#include <stdlib.h>
#include <string.h>
#include "response_defines.h"
#include "esp_at_list.h"

int8_t findStrInBuffer(char *buf, char *str)
{
	if (strstr(buf, str) != NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int8_t getStatusFromResponse(char *buf)
{
	if (findStrInBuffer(buf, AT_OK) == 1)
	{
		return RESPONSE_OK;
	}
	else if (findStrInBuffer(buf, AT_BUSY) == 1)
	{
		return RESPONSE_BUSY;
	}
	else if (findStrInBuffer(buf, AT_ERROR) == 1)
	{
		return RESPONSE_ERROR;
	}
	else
	{
		return RESPONSE_UNKNOWN;
	}
}

void clearBuffer(char *buf, int16_t len)
{
	for (int i = 0; i < len; i++)
		buf[i] = 0;
}

void delay_us(int a_us)
{
	int i;
	int a, b;

	for (i = 0; i < a_us; ++i)
	{
		for (a = 0; a < 6; a++)
		{
			b++;
		}
	}
}

void delay_ms(int a_ms)
{
	for (int i = 0; i < a_ms; i++)
	{
		delay_us(1000);
	}
}

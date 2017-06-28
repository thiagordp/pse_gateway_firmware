#ifndef UTIL_H_PSE
#define UTIL_H_PSE

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "response_defines.h"
#include "esp_at_list.h"

/**
 * Encontra uma string dentro de outra
 * Caso encontrar retorna o ponteiro pra posição, caso contrário retorna NULL.
 */
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

/**
 * Preenche o vetor com 0's.
 */
void clearBuffer(char *buf, int16_t len)
{
	for (int i = 0; i < len; i++)
		buf[i] = 0;
}

/**
 * Procura qual o status da resposta a um comando AT do ESP, verificando a string que é passada como parÂmetro
 *
 */
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

#endif /*UTIL_H_PSE*/


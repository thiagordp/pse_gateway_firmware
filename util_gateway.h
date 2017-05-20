#ifndef UTIL_H_PSE
#define UTIL_H_PSE

#include <stdint.h>

int8_t findStrInBuffer(char *buf, char *str);

void clearBuffer(char *buf, int16_t len);

int8_t getStatusFromResponse(char *buf);

void delay_us(int a_us);

void delay_ms(int a_ms);

#endif /*UTIL_H_PSE*/


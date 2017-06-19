#ifndef SERIAL_PSE_H
#define SERIAL_PSE_H

#include <stdint.h>

//Alterar de acordo com o uso
#define EPOS    (0)
#define ARDUINO (1)

#if ARDUINO
#define PARITY_EVEN     (0b10)
#define PARITY_ODD      (0b11)
#define PARITY_NONE     (0b00)
#define SERIAL_TIMEOUT	(5000)

#endif

class SerialRS232
{
private:

public:

	/**
	 *
	 */
	int8_t configure(uint32_t baude_rate, uint32_t data_bits, uint32_t parity, uint32_t stop_bits);

	/**
	 * Envia o comando especificado de acordo com o padrão RS232 do EPOS
	 * @param buf:  Comando enviado
	 * @param len:  Comprimento do cmd
	 * @return      Resultado da ação
	 */
	int8_t send(char *buf, int16_t len);

	/**
	 *
	 * @param buf
	 * @param len
	 * @return
	 */
	int8_t receive(char *buf, uint16_t len);

	/**
	 *
	 * @return
	 */
	int8_t available(void);

	/**
	 *
	 * @return
	 */
	int8_t flush(void);

};

#endif


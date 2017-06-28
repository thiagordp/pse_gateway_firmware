#ifndef SERIAL_PSE_H
#define SERIAL_PSE_H

#include <stdint.h>

//Alterar de acordo com o uso
#define EPOS_MOTE    (1)
#define ARDUINO (0)

#if ARDUINO
#define PARITY_EVEN     (0b10)
#define PARITY_ODD      (0b11)
#define PARITY_NONE     (0b00)
#define SERIAL_TIMEOUT (100)

#endif

#if EPOS_MOTE
#include "../include/machine/cortex/uart.h"
using namespace EPOS;
UART uart0(0, 9600, 8, 0, 1);
#endif

class SerialRS232 {
private:

public:

    /**
     * 
     */
    int8_t configure(uint32_t baude_rate, uint32_t data_bits, uint32_t parity, uint32_t stop_bits) {
        //config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits)
        uart0.config(baude_rate, data_bits, parity, stop_bits);
    }

    /**
     * Envia o comando especificado de acordo com o padrão RS232 do EPOS
     * @param buf:  Comando enviado
     * @param len:  Comprimento do cmd
     * @return      Resultado da ação
     */
    int8_t send(char *buf) {
        for (uint16_t i = 0; (buf[i] != '\0') /*&& (i < len)*/; i++) {
            if (uart0.ready_to_put())
                uart0.put(buf[i]);
            else
                return i;
        }
    }

    /**
     *
     * @param buf
     * @param len
     * @return
     */
    int8_t receive(char *buf, uint16_t len) {
        uint16_t i;
        for (i = 0; i < len; i++) {
            if (uart0.ready_to_get())
                buf[i] = uart0.get();
            else
                break;
        }

        return i;
    }

    /**
     *
     * @return
     */
    int8_t available(void) {
        if (uart0.ready_to_get())
            return 1;
        else
            return -1;
    }

    /* int8_t timedRead() {
         int c;
         uint64_t _timeout = SERIAL_TIMEOUT;
         uint64_t _startMillis = millis();
         do {
             c = uart0.get();
             if (c >= 0)
                 return c;
         } while (millis() - _startMillis < _timeout);

         return -1; // -1 indicates timeout
     }*/

    /**
     *
     * @return
     */
    int8_t flush(void) {
        while (uart0.ready_to_get()) {
            //timedRead();
            uart0.get();
            // delay(1);
        }

        return 1;
    }

};

#endif


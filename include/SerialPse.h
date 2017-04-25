/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   arduino_at.h
 * Author: thiagordp
 *
 * Created on 18 de Abril de 2017, 16:39
 */

#ifndef SERIAL_PSE_H
#define SERIAL_PSE_H


#include <stdint.h>

#define EPOS    (0)
#define ARDUINO (1)
#define SELECAO EPOS


class SerialPse {
    
private:
  
public:

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
    int8_t receive(char *buf, int16_t len);
    
    /**
     * 
     * @return 
     */
    int8_t available(void);
    
};

#endif


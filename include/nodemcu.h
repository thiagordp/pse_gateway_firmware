/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   nodemcu.h
 * Author: thiagordp
 *
 * Created on 18 de Abril de 2017, 15:24
 */
#ifndef NODEMCU_H
#define NODEMCU_H

#include <stdint.h>

#define AT_MODE // comentar depois

#ifdef AT_MODE
#include "esp_at_list.h"
#endif

/**
 * Node MCU class
 */
class NodeMcu {
private:
    uint8_t mode;

public:
    int8_t config_nodemcu();

    int8_t set_mode(uint8_t);

    uint8_t get_mode(void);
};

#endif /* NODEMCU_H */


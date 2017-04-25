/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cc
 * Author: thiagordp
 *
 * Created on 18 de Abril de 2017, 15:23
 */

#include <gpio.h>
#include <utility/ostream.h>

using namespace EPOS;

const unsigned int DEFAULT_TEST_N_TIMES = 200;
const NIC::Protocol NIC_PROTOCOL = 42;

GPIO * led;
OStream cout;

int main()
{
    led = new GPIO('C', 3, GPIO::OUT);
    blink_led(30, 0xFFFF, led);


    return 0;
}


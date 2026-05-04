#ifndef __DHT11_H__
#define __DHT11_H__

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "macro.h"

class DHT11 {
    private:
        uint8_t temperature;
        uint8_t humidity;
        uint8_t read_byte();
        uint8_t wait_high(uint16_t timeout);
        uint8_t wait_low(uint16_t timeout);
        uint8_t set_temperature(uint8_t t);
        uint8_t set_humidity(uint8_t h);
    public:
        DHT11();
        ~DHT11();
        uint8_t get_temperature();
        uint8_t get_humidity();
        uint8_t read();
};

#endif
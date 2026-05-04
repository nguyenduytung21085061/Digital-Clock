#include "dht11.h"

DHT11::DHT11(){
    DDRD &= ~(1 << P_DHT11);
    PORTD |= (1 << P_DHT11);
}

DHT11::~DHT11(){
}

uint8_t DHT11::wait_high(uint16_t timeout){
    while (!(PIND & (1 << P_DHT11))) {
        if (--timeout == 0) 
            return 0;
        _delay_us(1);
    }
    return 1;
}

uint8_t DHT11::wait_low(uint16_t timeout){
    while (PIND & (1 << P_DHT11)) {
        if (--timeout == 0) 
            return 0;
        _delay_us(1);
    }
    return 1;
}

uint8_t DHT11::set_temperature(uint8_t t){
    if(this->temperature == t)
        return;
    this->temperature = t;    
}

uint8_t DHT11::set_humidity(uint8_t h){
    if(this->humidity == h)
        return;
    this->humidity = h;
}

uint8_t DHT11::get_temperature(){
    return this->temperature;
}

uint8_t DHT11::get_humidity(){
    return this->humidity;
}

uint8_t DHT11::read_byte(){
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (!wait_low(200)) 
            return 0;
        if (!wait_high(200)) 
            return 0;
        _delay_us(40);
        if (PIND & (1 << P_DHT11))
            byte |= (1 << (7 - i));
        if (!wait_low(200)) 
            return 0;
    }
    return byte;
}

uint8_t DHT11::read(){
    uint8_t data[5] = {0};
    DDRD |= (1 << P_DHT11);
    PORTD &= ~(1 << P_DHT11);
    _delay_ms(20);
    PORTD |= (1 << P_DHT11);
    _delay_us(30);
    DDRD &= ~(1 << P_DHT11); 
    if (!wait_low(500)) 
        return 0;
    if (!wait_high(500)) 
        return 0;
    if (!wait_low(500)) 
        return 0;
    for (uint8_t i = 0; i < 5; i++)
        data[i] = this->read_byte();
    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) != data[4])
        return 0;
    this->set_humidity(data[0]);
    this->set_temperature(data[2]);
    return 1;
}
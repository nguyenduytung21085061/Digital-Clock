#include "ds1302.h"

DS1302::DS1302(/* args */){
    DDRD |= (1 << RESET_DS1302) | (1 << SCLK_DS1302);
    DDRD &= ~(1 << IO_DS1302);
    PORTD &= ~((1 << RESET_DS1302) | (1 << SCLK_DS1302));
}

DS1302::~DS1302(){
    
}

void DS1302::write_byte(uint8_t data){
    DDRD |= (1 << IO_DS1302);
    for(uint8_t i = 0; i < 8; i++){
		(data & 0x01) ? (PORTD |= (1 << IO_DS1302)) : (PORTD &= ~(1 << IO_DS1302));
		PORTD |= (1 << SCLK_DS1302);
		_delay_us(2);
		PORTD &= ~(1 << SCLK_DS1302);
		_delay_us(2);
		data >>= 1;
	}
}

uint8_t DS1302::read_byte(){
    uint8_t data = 0;
    DDRD &= ~(1 << IO_DS1302);
    for(uint8_t i = 0; i < 8; i++){
		data >>= 1;
        if(PIND & (1 << IO_DS1302))
			data |= 0x80;
        PORTD |= (1 << SCLK_DS1302);
        _delay_us(2);
        PORTD &= ~(1 << SCLK_DS1302);
        _delay_us(2);
    }
    return data;
}

void DS1302::write_reg(uint8_t address, uint8_t data){
    PORTD |= (1 << RESET_DS1302);
	this->write_byte((address << 1) | 0x80);
	this->write_byte(data);
	PORTD &= ~(1 << RESET_DS1302);   
}

uint8_t DS1302::read_reg(uint8_t address){
    uint8_t value;
	PORTD |= (1 << RESET_DS1302);
	this->write_byte((address << 1) | 0x81);
	value = this->read_byte();
	PORTD &= ~(1 << RESET_DS1302);
	return value;
}

uint8_t DS1302::bcd_convert_dec(uint8_t bcd){
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

uint8_t DS1302::dec_covert_bcd(uint8_t dec){
    return ((dec / 10) << 4) | (dec % 10);
}


void DS1302::reset(){
    this->write_reg(CONTROL, WP_DISABLE);
    this->write_reg(SEC, WP_DISABLE);
    this->write_reg(CONTROL, WP_ENABLE);
}

void DS1302::set_hours(uint8_t h){
    this->write_reg(CONTROL, WP_DISABLE);
    this->write_reg(HOUR, dec_covert_bcd(h));
    this->write_reg(CONTROL, WP_ENABLE);   
}

void DS1302::set_minutes(uint8_t m){
    this->write_reg(CONTROL, WP_DISABLE);
    this->write_reg(MIN, dec_covert_bcd(m));
    this->write_reg(CONTROL, WP_ENABLE); 
}

uint8_t DS1302::get_hours(){
    this->hours = this->bcd_convert_dec(this->read_reg(HOUR) & 0x3F);
    return this->hours;
}

uint8_t DS1302::get_minutes(){
    this->minutes = this->bcd_convert_dec(this->read_reg(MIN));
    return this->minutes;
}
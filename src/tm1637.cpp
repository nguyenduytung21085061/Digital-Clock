#include "tm1637.h"

TM1637::TM1637(/* args */){
    DDRD |= (1 << CLK_TM1637); 
    DDRB |= (1 << DIO_TM1637);
    PORTD |= (1 << CLK_TM1637);
    PORTB |= (1 << DIO_TM1637);
}

TM1637::~TM1637()
{
    
}

void TM1637::start(){
    PORTB |= (1 << DIO_TM1637);
    PORTD |= (1 << CLK_TM1637);
    _delay_us(2);
    PORTB &= ~(1 << DIO_TM1637);
}

void TM1637::stop(){
    PORTD &= ~(1 << CLK_TM1637);
    _delay_us(2);
    PORTB &= ~(1 << DIO_TM1637);
    _delay_us(2);
    PORTD |= (1 << CLK_TM1637);
    _delay_us(2);
    PORTB |= (1 << DIO_TM1637);
}

void TM1637::ack()
{
    PORTD &= ~(1 << CLK_TM1637);
    _delay_us(5);
    DDRB &= ~(1 << DIO_TM1637); 
    PORTB |= (1 << DIO_TM1637);
    while (PINB & (1 << DIO_TM1637));
    DDRB |= (1 << DIO_TM1637);
    PORTD |= (1 << CLK_TM1637);
    _delay_us(2);
    PORTD &= ~(1 << CLK_TM1637);
}

void TM1637::write_byte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++){
        PORTD &= ~(1 << CLK_TM1637);
        if (data & 0x01)
            PORTB |= (1 << DIO_TM1637);
        else
            PORTB &= ~(1 << DIO_TM1637);
        _delay_us(3);
        data >>= 1;
        PORTD |= (1 << CLK_TM1637);
        _delay_us(3);
    }
}

uint8_t TM1637::segment(int index){
    if (index < 0)
        return 0x00;
    return this->seg[index];
}

void TM1637::display(int digit_left, int digit_right, bool dots){
    this->start();
    this->write_byte(0x40);
    this->ack();
    this->stop();

    this->start();
    this->write_byte(0xC0);
    this->ack();
    if(digit_left < 0){
        this->write_byte(0x00);
        this->ack();
        this->write_byte(0x00);
        this->ack();
    }else{
        this->write_byte(this->segment(digit_left / 10));
        this->ack();
        this->write_byte(this->segment(digit_left % 10) | (dots ? 0x80 : 0x00));
        this->ack();
    }
    if(digit_right < 0){
        this->write_byte(0x00);
        this->ack();
        this->write_byte(0x00);
        this->ack();
    }else{
        this->write_byte(this->segment(digit_right / 10));
        this->ack();
        this->write_byte(this->segment(digit_right % 10));
        this->ack();
    }
    this->stop();

    this->start();
    this->write_byte(0x8F);
    this->ack();
    this->stop();
}
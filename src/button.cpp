#include "button.h"

BUTTON *BUTTON::instance = nullptr;

BUTTON::BUTTON() {
    DDRB &= ~((1 << BUTTON_1) | (1 << BUTTON_2) | (1 << BUTTON_3) | (1 << BUTTON_4));
    PORTB |= (1 << BUTTON_1) | (1 << BUTTON_2) | (1 << BUTTON_3) | (1 << BUTTON_4);
    GIMSK |= (1 << PCIE);
    PCMSK |= (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3) | (1 << PCINT4);
    this->instance = this;
}

BUTTON::~BUTTON(){}

void BUTTON::set_raw_state(uint8_t raw){
    if(this->raw_state == raw)
        return;
    this->raw_state = raw;
}

void BUTTON::set_event_state(uint8_t event){
    if(this->event_state == event)
        return;
    this->event_state = event;
}

void BUTTON::set_previous_state(uint8_t previous){
    if(this->prev_state == previous)
        return;
    this->prev_state = previous;
}

uint8_t BUTTON::get_raw_state(){
    return this->raw_state;
}

uint8_t BUTTON::get_event_state(){
    return this->event_state;
}

uint8_t BUTTON::get_previous_state(){
    return this->prev_state;
}

uint8_t BUTTON::get_state() {
    uint8_t data = this->get_event_state();
    this->set_event_state(0); 
    return data;
}

uint8_t BUTTON::get_level_button_1() { 
    return (this->get_state() & (1 << 0)) ? 1 : 0; 
}

uint8_t BUTTON::get_level_button_2() { 
    return (this->get_state() & (1 << 1)) ? 1 : 0; 
}

uint8_t BUTTON::get_level_button_3() { 
    return (this->get_state() & (1 << 2)) ? 1 : 0; 
}

uint8_t BUTTON::get_level_button_4() { 
    return (this->get_state() & (1 << 3)) ? 1 : 0; 
}

void BUTTON::handle_irq(){
    uint8_t pin = PINB;
    this->set_raw_state(0);
    if(!(pin & (1 << BUTTON_1))){
        uint8_t state = this->get_raw_state();
        this->set_raw_state(state |= (1 << 0));
    }
    if(!(pin & (1 << BUTTON_2))){
        uint8_t state = this->get_raw_state();
        this->set_raw_state(state |= (1 << 1));
    }
    if(!(pin & (1 << BUTTON_3))){
        uint8_t state = this->get_raw_state();
        this->set_raw_state(state |= (1 << 2));
    }
    if(!(pin & (1 << BUTTON_4))){
        uint8_t state = this->get_raw_state();
        this->set_raw_state(state |= (1 << 3));
    }
    uint8_t changed = this->get_previous_state() ^ this->get_raw_state();
    if((changed & (1 << 0)) && !(this->get_raw_state() & (1 << 0))){
        uint8_t state = this->get_event_state();
        this->set_event_state(state | (1 << 0));
    }
    if((changed & (1 << 1)) && !(this->get_raw_state() & (1 << 1))){
        uint8_t state = this->get_event_state();
        this->set_event_state(state | (1 << 1));
    }
    if((changed & (1 << 2)) && !(this->get_raw_state() & (1 << 2))){
        uint8_t state = this->get_event_state();
        this->set_event_state(state | (1 << 2));
    }
    if((changed & (1 << 3)) && !(this->get_raw_state() & (1 << 3))){
        uint8_t state = this->get_event_state();
        this->set_event_state(state | (1 << 3));
    }
    this->set_previous_state(this->get_raw_state());
}

ISR(PCINT_vect){
    if(BUTTON::instance)
        BUTTON::instance->handle_irq();
}
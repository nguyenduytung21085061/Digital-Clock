#include "buzzer.h"

BUZZER::BUZZER(/* args */){
    DDRD |= (1 << P_BUZZER);
    PORTD &= ~(1 << P_BUZZER);
}

BUZZER::~BUZZER() {
    
}

void BUZZER::bip() {
    PORTD |= (1 << P_BUZZER);
}

void BUZZER::stop(){
    PORTD &= ~(1 << P_BUZZER);
}

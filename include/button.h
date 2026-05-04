#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include "macro.h"

class BUTTON{
    private:
        uint8_t raw_state = 0;
        uint8_t event_state = 0;
        uint8_t prev_state = 0;
    public:
        static BUTTON* instance;
        BUTTON();
        ~BUTTON();
        void set_raw_state(uint8_t raw);
        void set_event_state(uint8_t event);
        void set_previous_state(uint8_t previous);
        uint8_t get_raw_state();
        uint8_t get_event_state();
        uint8_t get_previous_state();
        uint8_t get_state();
        uint8_t get_level_button_1();
        uint8_t get_level_button_2();
        uint8_t get_level_button_3();
        uint8_t get_level_button_4();
        void handle_irq();
};

#endif
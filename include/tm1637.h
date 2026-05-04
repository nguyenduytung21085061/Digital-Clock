#ifndef __TM1637_H__
#define __TM1637_H__

#include <avr/io.h>
#include <util/delay.h>
#include "macro.h"

class TM1637{
    private:
        /* data */
        const uint8_t seg[10] = {
            0x3F, // 0
            0x06, // 1
            0x5B, // 2
            0x4F, // 3
            0x66, // 4
            0x6D, // 5
            0x7D, // 6
            0x07, // 7
            0x7F, // 8
            0x6F  // 9
        };
        void ack();
        void write_byte(uint8_t data);
        void start();
        void stop();
    public:
        TM1637(/* args */);
        ~TM1637();
        uint8_t segment(int index);
        void display(int digit_left, int digit_right, bool dots = true);
};

#endif // __TM1637_H__
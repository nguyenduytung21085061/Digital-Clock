#ifndef __DS1302_H__
#define __DS1302_H__

#include <avr/io.h>
#include <util/delay.h>
#include "macro.h"

#define WP_DISABLE  0x00
#define WP_ENABLE   0x80
#define SEC 	    0x00
#define MIN 	    0x01
#define HOUR 	    0x02
#define CONTROL	    0x07

class DS1302{
    private:
        /* data */
        uint8_t hours;
        uint8_t minutes;
        void write_byte(uint8_t data);
        uint8_t read_byte();
        void write_reg(uint8_t address, uint8_t data);
        uint8_t read_reg(uint8_t address);
        uint8_t bcd_convert_dec(uint8_t bcd);
        uint8_t dec_covert_bcd(uint8_t dec);
    public:
        DS1302(/* args */);
        ~DS1302();
        void reset();
        void set_hours(uint8_t h);
        void set_minutes(uint8_t m);
        uint8_t get_hours();
        uint8_t get_minutes();
};

#endif // __DS1302_H__
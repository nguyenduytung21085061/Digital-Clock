#include <stdint.h>
#include <avr/io.h>
#define F_CPU 11059200UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "buzzer.h"
#include "button.h"
#include "ds1302.h"
#include "tm1637.h"
#include "dht11.h"

BUZZER buzzer;
BUTTON button;
DS1302 ds1302;
TM1637 tm1637;
DHT11 dht11;

int main(){
    sei();
    // ds1302.set_hours(21);
    // ds1302.set_minutes(40); 
    _delay_ms(100);
    (void)button.get_state();
    tm1637.display(ds1302.get_hours(), ds1302.get_minutes());
    uint8_t mode = 0;
    uint8_t context = 0;
    uint8_t alarm_hour = 0;
    uint8_t alarm_min = 0;
    uint8_t alarm_enable = 0;
    uint8_t alarm_ringing = 0;
    uint16_t alarm_timer = 0;
    uint16_t blink_timer = 0;
    uint8_t blink_state = 0;
    _delay_ms(200);
    while (1) {
        uint8_t event = button.get_state();
        if (event & (1 << 0)) {
            if (mode == 2){
                context++;
                if (context >= 24) 
                    context = 0;
            } else if (mode == 3){
                context += 5;
                if (context >= 60) 
                    context = 0;
            }
        }
        if (event & (1 << 1)) {
            if (mode == 2){
                if (context == 0) 
                    context = 23;
                else    
                    context--;
            } else if (mode == 3){
                if(context < 5) 
                    context = 55;
                else 
                    context -= 5;
            }
        }
        if (event & (1 << 2)) {
            mode++;
            if (mode >= 4) 
                mode = 0;
            if (mode == 2) 
                context = alarm_hour;
            if (mode == 3) 
                context = alarm_min;
        }
        if (event & (1 << 3)) {
            if (alarm_ringing){
                alarm_ringing = 0;
                alarm_enable = 0;
                buzzer.stop();
            } else{
                if (mode == 2){
                    alarm_hour = context;
                } else if (mode == 3){
                    alarm_min = context;
                    alarm_enable = 1;
                }
            }
        }
        blink_timer += 100;
        if (blink_timer >= 500){
            blink_timer = 0;
            blink_state = !blink_state;
        }
        switch (mode){
            case 0:
                tm1637.display(ds1302.get_hours(), ds1302.get_minutes());
                break;
            case 1:
                (void)dht11.read();
                tm1637.display(dht11.get_temperature(), dht11.get_humidity()); 
                break;
            case 2:
                if (blink_state)
                    tm1637.display(-1, alarm_min);
                else
                    tm1637.display(context, alarm_min);
                break;
            case 3:
                if (blink_state)
                    tm1637.display(alarm_hour, -1);
                else
                    tm1637.display(alarm_hour, context);
                break;
        }
        uint8_t h = ds1302.get_hours();
        uint8_t m = ds1302.get_minutes();
        if (alarm_enable && !alarm_ringing){
            if (h == alarm_hour && m == alarm_min){
                alarm_ringing = 1;
                alarm_timer = 0;
            }
        }
        if (alarm_ringing){
            if ((alarm_timer / 250) % 2 == 0)
                buzzer.bip();
            else
                buzzer.stop();
            alarm_timer += 100;
            if(alarm_timer >= 5000){
                alarm_ringing = 0;
                alarm_enable = 0;
                buzzer.stop();
            }
        }
        _delay_ms(200);
    }
}
#ifndef __BUZZER_H__
#define __BUZZER_H__

#include <avr/io.h>
#include "macro.h"

class BUZZER{
    private:
        /* data */
    public:
        BUZZER(/* args */);
        ~BUZZER();
        void bip();
        void stop();
};

#endif // __BUZZER_H__
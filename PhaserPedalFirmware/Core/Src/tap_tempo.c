#include "tap_tempo.h"

bool button_debounce(button* btn, bool pinState){
    btn->state = (btn->state << 1) | (uint16_t)pinState;
    return (btn->state == btn->mask);
}

static button tapBtn;


void tap_tempo_init(){
    tapBtn.mask = 0xFFF0;
    tapBtn.state = 0xFFFF;
}


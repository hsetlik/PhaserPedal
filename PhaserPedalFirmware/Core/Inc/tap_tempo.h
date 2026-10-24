#ifndef TAP_TEMPO_H
#define TAP_TEMPO_H
#include "main.h"
typedef struct {
    uint16_t mask;
    uint16_t state;
} button;
bool button_debounce(button* btn, bool pinState);
// call on startup
void tap_tempo_init();
// pass the GPIO state into this on the callback
void tap_tempo_update(bool pinState);
// 
bool tap_tempo_was_tapped_within(float length);

float tap_tempo_current_period();

#endif
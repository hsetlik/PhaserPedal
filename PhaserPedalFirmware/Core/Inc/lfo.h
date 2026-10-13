#ifndef LFO_H
#define LFO_H
#include "main.h"

typedef struct {
    float currentPhase;
    float periodSeconds;
    float peakPhase;
    float phaseDelta;
    float risingSlope;
    float fallingSlope;
} lfo_state;
// set up the LFO with default values
void lfo_init_state(lfo_state* lfo);

uint16_t lfo_next_value(lfo_state* lfo);


#endif
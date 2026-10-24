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
// these update functions handle all the math involved in these values changing
void lfo_update_period(lfo_state* lfo, float newPeriod);
void lfo_update_peak_phase(lfo_state* lfo, float newPeakPhase);


#endif
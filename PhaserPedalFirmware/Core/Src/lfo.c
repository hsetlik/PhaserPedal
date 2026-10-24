#include "lfo.h"
#include "main.h"
#include <stdint.h>

// remember, slopes are y value/PHASE, not seconds
static void set_slopes(lfo_state* lfo){
    lfo->phaseDelta = 1.0f / (lfo->periodSeconds * (float)SAMPLE_RATE);
    lfo->risingSlope = 1.0f / lfo->peakPhase;
    lfo->fallingSlope = 1.0f / (1.0f - lfo->peakPhase);
}

void lfo_init_state(lfo_state* lfo){
    // one second period
    lfo->periodSeconds = 1.0f;
    // even triangle
    lfo->peakPhase = 0.5f;
    // start at 0
    lfo->currentPhase = 0.0f;
    set_slopes(lfo);
}


uint16_t lfo_next_value(lfo_state* lfo){  
    float fValue = 0.0f;
    if(lfo->currentPhase > lfo->peakPhase){
        fValue = 1.0f - (lfo->fallingSlope * (lfo->currentPhase - lfo->peakPhase));
    } else {
        fValue = lfo->currentPhase * lfo->risingSlope;
    }
    lfo->currentPhase += lfo->phaseDelta;
    if(lfo->currentPhase >= 1.0f){
        lfo->currentPhase -= 1.0f;
    }
    return (uint16_t)(fValue * 4095.0f);
}

void lfo_update_period(lfo_state* lfo, float newPeriod){
    lfo->periodSeconds = newPeriod;
    set_slopes(lfo);
}
void lfo_update_peak_phase(lfo_state* lfo, float newPeakPhase){
    if(lfo->currentPhase < lfo->peakPhase && lfo->currentPhase >= newPeakPhase){
        float prePeakLevel = (lfo->peakPhase - lfo->currentPhase) / lfo->peakPhase;
        lfo->peakPhase = newPeakPhase;
        set_slopes(lfo);
        // (newPhase - peakPhase) / (1 - peakPhase) = 1 - prePeakLevel
        // newPhase - peakPhase = (1 - peakPhase)(1 - prePeakLevel)
        lfo->currentPhase = ((1.0f - lfo->peakPhase) * (1.0f - prePeakLevel)) + lfo->peakPhase;
    } else if(lfo->currentPhase >= lfo->peakPhase && lfo->currentPhase < newPeakPhase){
        float postPeakLevel = 1.0f - ((lfo->currentPhase - lfo->peakPhase)/(1.0f - lfo->peakPhase));
        lfo->peakPhase = newPeakPhase;
        set_slopes(lfo);
        // postPeakLevel = newPhase / peakPhase
        lfo->currentPhase = postPeakLevel * lfo->peakPhase;
    } else {
        lfo->peakPhase = newPeakPhase;
        set_slopes(lfo);
    }
}
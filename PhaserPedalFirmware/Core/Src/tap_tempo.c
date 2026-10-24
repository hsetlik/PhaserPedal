#include "tap_tempo.h"
#include "main.h"
#include <stdint.h>
#define TAP_BUF_SIZE 64
#define LOOKBACK_SECONDS 3.0f
bool button_debounce(button* btn, bool pinState){
    btn->state = (btn->state << 1) | (uint16_t)pinState;
    return (btn->state == btn->mask);
}

static button tapBtn;
static float tapBuffer[TAP_BUF_SIZE];
static uint16_t tapBufHead = 0;
static inline float tap_buf_read(uint16_t idx){
    return tapBuffer[(idx + tapBufHead) % TAP_BUF_SIZE];
}
// k = 0 is the newest tap, k = 1 the one before it, etc.
static inline float tap_buf_read_recent(uint16_t k){
    return tapBuffer[(tapBufHead + TAP_BUF_SIZE - 1 - k) % TAP_BUF_SIZE];
}
static inline void tap_buf_push(float value){
    tapBuffer[tapBufHead] = value;
    tapBufHead = (tapBufHead + 1) % TAP_BUF_SIZE;
}
static inline uint16_t num_taps_within(float lookbackSeconds, uint16_t maxToCheck){
    const float minTime = get_global_time() - lookbackSeconds;
    uint16_t t = 0;
    while(tap_buf_read_recent(t) >= minTime && t < maxToCheck){
        ++t;
    }
    return t;
}

// average interval (seconds) across the `numTaps` most recent taps
static float tap_avg_interval(uint16_t numTaps){
    if(numTaps < 2 || numTaps > TAP_BUF_SIZE){
        return 0.0f;
    }
    const float newest = tap_buf_read_recent(0);
    const float oldest = tap_buf_read_recent(numTaps - 1);
    return (newest - oldest) / (float)(numTaps - 1);
}

float tap_tempo_current_period(){
    tap_avg_interval(num_taps_within(3.0f, 4));
}

bool tap_tempo_was_tapped_within(float length){
    const float minTime = get_global_time() - length;
    return tap_buf_read_recent(0) >= minTime;
}
void tap_tempo_init(){
    tapBtn.mask = 0xFFF0;
    tapBtn.state = 0xFFFF;
    for(uint16_t i = 0; i < TAP_BUF_SIZE; ++i){
        tapBuffer[i] = 0.0f;
    }
}


void tap_tempo_update(bool pinState){
    if(button_debounce(&tapBtn, pinState)){
        tap_buf_push(get_global_time());
    }
}


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
static inline void tap_buf_push(float value){
    tapBuffer[tapBufHead] = value;
    tapBufHead = (tapBufHead + 1) % TAP_BUF_SIZE;
}
static uint16_t oldest_recent_tap(float minTime){
    if(minTime < 0.0f){
        minTime = 0.0f;
        return 0;
    }
    float oldest = get_global_time();
    uint16_t oldestIdx = 0;
    for(uint16_t i = 0; i < TAP_BUF_SIZE; ++i){
        if(tapBuffer[i] >= minTime){
            if(tapBuffer[i] < oldest) {
                oldest = tapBuffer[i];
                oldestIdx = i;
            }
        }
    }
    return oldestIdx;
}

float tap_tempo_current_hz_estimate(){
    float minTime =  get_global_time() - LOOKBACK_SECONDS;
    const uint16_t oldestIdx = oldest_recent_tap(minTime);
    uint16_t i = 0;
    uint16_t lowerIdx = (oldestIdx + i) % TAP_BUF_SIZE;
    uint16_t upperIdx = (lowerIdx + 1) % TAP_BUF_SIZE;
    float diffSum = 0.0f;
    while(tapBuffer[upperIdx] >= minTime && tapBuffer[upperIdx] > tapBuffer[lowerIdx] && i < TAP_BUF_SIZE){
        diffSum += tapBuffer[upperIdx] - tapBuffer[lowerIdx];
        ++i;
        lowerIdx = (oldestIdx + i) % TAP_BUF_SIZE;
        upperIdx = (lowerIdx + 1) % TAP_BUF_SIZE;
    }
    const float avgPeriod = diffSum / (float)i;
    return 1.0f / avgPeriod;
}

bool tap_tempo_was_tapped_within(float length){
    const float minTime = get_global_time() - length;
    for(uint16_t i = 0; i < TAP_BUF_SIZE; ++i){
        if(tap_buf_read(i) >= minTime){
            return true;
        }
    }
    return false;
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


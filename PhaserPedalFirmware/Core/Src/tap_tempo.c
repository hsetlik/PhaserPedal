#include "tap_tempo.h"
#include "main.h"
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
static uint16_t oldest_recent_tap(){
    float minTime =  get_global_time() - LOOKBACK_SECONDS;
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


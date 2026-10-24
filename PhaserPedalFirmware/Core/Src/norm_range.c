#include "norm_range.h"
#include <math.h>


void norm_range_init(norm_range_t* nr, float min, float max, float center){
    nr->skew = logf(0.5f) / logf((center - min) / (max - min));
    nr->maxVal = max;
    nr->minVal = min;
    nr->centerVal = center;
}

float norm_range_normalize(norm_range_t* nr, float value){
    value = fmaxf(fminf(nr->maxVal, value), nr->minVal);
    const float proportion = (value - nr->minVal) / (nr->maxVal - nr->minVal);
    return powf(proportion, nr->skew);
}

float norm_range_denormalize(norm_range_t* nr, float value){
    const float proportion = expf(logf(value) / nr->skew);
    return nr->minVal + ((nr->maxVal - nr->minVal) * proportion);
}
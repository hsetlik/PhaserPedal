#ifndef NORM_RANGE_H
#define NORM_RANGE_H
#include "main.h"

typedef struct {
    float maxVal;
    float minVal;
    float centerVal;
    float skew;
} norm_range_t;

void norm_range_init(norm_range_t* nr, float min, float max, float center);
float norm_range_normalize(norm_range_t* nr, float value);
float norm_range_denormalize(norm_range_t* nr, float value);
#endif
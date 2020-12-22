#include "Arduino.h"
#include "tools/filters.h"



// 10hz Single Pole Lowpass IIR Filter
uint16_t envelopeFilter(uint16_t sample, uint8_t filter_factor) { //10hz low pass
    static uint16_t xv[2] = {0,0}, yv[2] = {0,0};
    xv[0] = xv[1]; 
    xv[1] = sample;
    yv[0] = yv[1]; 
    yv[1] = ((xv[0] + xv[1]) / 2 + (filter_factor * yv[0])) / (1 + filter_factor);
    return yv[1];
}
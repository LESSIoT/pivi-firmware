

#ifndef _FIRMWARE_H
#define _FIRMWARE_H

#include <stdint.h>
#include <adc.h>

typedef struct {
    uint8_t circuit_id;
    ADC_t *V_adc;
    enum adcch_positive_input V_pin;
    ADC_t *I_adc;
    enum adcch_positive_input I_pin;
    uint16_t V_dc_offset;
    uint16_t I_dc_offset;
    float V_gain; /* 1/Att of voltage circuit */
    float I_gain;
    uint16_t delay; /* delay of the voltage filter */
} circuit_t;

#endif /* _FIRMWARE_H */

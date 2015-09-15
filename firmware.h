/*
 * This file is part of the Pivi firmware package.
 * Released under the Creative Commons ATT-NC-ShareAlire license
 * http://creativecommons.org/licenses/by-nc-sa/4.0/
 * Copyright (C) 2014, 2015 LESS Industries S.A.
 * Author: Diego Mascialino <dmascialino@gmail.com>
 */



#ifndef _FIRMWARE_H
#define _FIRMWARE_H

#include <stdint.h>
#include <asf.h>


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
    port_pin_t ct_detector_pin;
    enum adc_reference I_ref;
    enum adc_reference V_ref;
} circuit_t;

#endif /* _FIRMWARE_H */

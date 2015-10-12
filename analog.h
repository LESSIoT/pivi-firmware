/*
 * This file is part of the Pivi firmware package.
 * Released under the Creative Commons ATT-NC-ShareAlire license
 * http://creativecommons.org/licenses/by-nc-sa/4.0/
 * Copyright (C) 2014, 2015 LESS Industries S.A.
 * Author: Diego Mascialino <dmascialino@gmail.com>
 */

#ifndef _ANALOG_H
#define _ANALOG_H

#include "firmware.h"

#define ADC_FREQ   2000000UL

#define V_ADC_CH    ADC_CH0
#define I_ADC_CH    ADC_CH1

void analog_config(circuit_t *circuit);
volatile float analog_get_V_sample(void);
volatile float analog_get_I_sample(void);


volatile uint16_t analog_get_I_sample_calibration(void);
volatile uint16_t analog_get_V_sample_calibration(void);

#endif /* _ANALOG_H */

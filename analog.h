
#ifndef _ANALOG_H
#define _ANALOG_H

#include "asf.h"
#include "firmware.h"

#define ADC_FREQ  2000000UL

#define V_ADC_CH    ADC_CH0
#define I_ADC_CH    ADC_CH1


void analog_config(circuit_t *circuit);
float analog_get_v_sample(circuit_t *circuit);
float analog_get_i_sample(circuit_t *circuit);

#endif /* _ANALOG_H */

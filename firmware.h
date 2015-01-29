

#ifndef _FIRMWARE_H
#define _FIRMEARE_H

#include "asf.h"


typedef struct {
	uint8_t circuit_id;
	ADC_t *v_adc;
	enum adcch_positive_input v_pin;
	ADC_t *i_adc;
	enum adcch_positive_input i_pin;
	uint16_t dc_offset;
	float v_gain; /* 1/Att of voltage circuit */
	float i_gain;
	uint16_t delay; /* delay of the voltage filter */
} circuit_t;

#endif /* _FIRMWARE_H */

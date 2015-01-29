
#include "analog.h"


/* internal functions prototypes */
void _channel_config(ADC_t *adc, enum adcch_positive_input pin, uint8_t ch_mask);
/* ----------------------------- */

void analog_config(circuit_t *circuit)
{
	_channel_config(circuit->v_adc, circuit->v_pin, V_ADC_CH);
	_channel_config(circuit->i_adc, circuit->i_pin, I_ADC_CH);
}

float analog_get_v_sample(circuit_t *circuit)
{
	uint16_t adc_measure;
	int16_t signed_measure;
	adc_enable(circuit->v_adc);

	adc_start_conversion(circuit->v_adc, V_ADC_CH);
	adc_wait_for_interrupt_flag(circuit->v_adc, V_ADC_CH);
	adc_measure = adc_get_result(circuit->v_adc, V_ADC_CH);

	signed_measure = adc_measure - circuit->dc_offset;
	return circuit->v_gain * (signed_measure / (float) (1<<12));
}

float analog_get_i_sample(circuit_t *circuit)
{
	uint16_t adc_measure;
	adc_enable(circuit->v_adc);
	adc_start_conversion(circuit->v_adc, V_ADC_CH);
	adc_wait_for_interrupt_flag(circuit->v_adc, V_ADC_CH);

	adc_measure = adc_get_result(circuit->v_adc, V_ADC_CH);
	return adc_measure * circuit->i_gain;
}

void _channel_config(ADC_t *adc, enum adcch_positive_input pin, uint8_t ch_mask)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;
	adc_read_configuration(adc, &adc_conf);
	adcch_read_configuration(adc, ch_mask, &adcch_conf);
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12,
	ADC_REF_BANDGAP);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adc_conf, ADC_FREQ);
	adcch_set_input(&adcch_conf, pin, ADCCH_NEG_NONE, 1);
	adc_write_configuration(adc, &adc_conf);
	adcch_write_configuration(adc, pin, &adcch_conf);

}

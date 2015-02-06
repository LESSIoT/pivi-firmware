
#include "analog.h"
#include "mock_signals.h"
#include "adc.h"

circuit_t * _circuit;

uint8_t v_idx = 0, i_idx = 0;

/* internal functions prototypes */
void _channel_config(ADC_t *adc, enum adcch_positive_input pin, uint8_t ch_mask);
/* ----------------------------- */

void analog_config(circuit_t *circuit)
{
    _circuit = circuit;
    _channel_config(circuit->V_adc, circuit->V_pin, V_ADC_CH);
    _channel_config(circuit->I_adc, circuit->I_pin, I_ADC_CH);
}

volatile float analog_get_V_sample(void)
{
    return V[v_idx++%MOCK_N];
    uint16_t adc_measure;
    int16_t signed_measure;
    adc_enable(_circuit->V_adc);

    adc_start_conversion(_circuit->V_adc, V_ADC_CH);
    adc_wait_for_interrupt_flag(_circuit->V_adc, V_ADC_CH);
    adc_measure = adc_get_result(_circuit->V_adc, V_ADC_CH);

    signed_measure = adc_measure - _circuit->V_dc_offset;
    return _circuit->V_gain * (signed_measure / (float) (1<<12));
}

volatile float analog_get_I_sample(void)
{
    return I[i_idx++%MOCK_N];
    uint16_t adc_measure;
    adc_enable(_circuit->I_adc);
    adc_start_conversion(_circuit->I_adc, I_ADC_CH);
    adc_wait_for_interrupt_flag(_circuit->I_adc, I_ADC_CH);

    adc_measure = adc_get_result(_circuit->V_adc, I_ADC_CH);
    return _circuit->I_gain * (adc_measure / (float) (1<<12));
}

void _channel_config(ADC_t *adc, enum adcch_positive_input pin, uint8_t ch_mask)
{
    struct adc_config adc_conf;
    struct adc_channel_config adcch_conf;

    adc_read_configuration(adc, &adc_conf);
    adcch_read_configuration(adc, ch_mask, &adcch_conf);
    adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12, ADC_REF_BANDGAP);
    adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
    adc_set_clock_rate(&adc_conf, ADC_FREQ);
    adcch_set_input(&adcch_conf, pin, ADCCH_NEG_NONE, 1);
    adc_write_configuration(adc, &adc_conf);
    adcch_write_configuration(adc, pin, &adcch_conf);
}

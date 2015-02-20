
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
    uint16_t adc_measure;
    int16_t signed_measure;

    adc_start_conversion(_circuit->V_adc, V_ADC_CH);
    adc_wait_for_interrupt_flag(_circuit->V_adc, V_ADC_CH);
    adc_measure = adc_get_result(_circuit->V_adc, V_ADC_CH);

    signed_measure = adc_measure - _circuit->V_dc_offset;
    return _circuit->V_gain * (signed_measure / (float) (1<<12));
}

volatile float analog_get_I_sample(void)
{
    uint16_t adc_measure;
    int16_t signed_measure;

    adc_start_conversion(_circuit->I_adc, I_ADC_CH);
    adc_wait_for_interrupt_flag(_circuit->I_adc, I_ADC_CH);
    adc_measure = adc_get_result(_circuit->I_adc, I_ADC_CH);

    signed_measure = adc_measure - _circuit->I_dc_offset;
    return _circuit->I_gain * (signed_measure / (float) (1<<12));
}

#define OVERSAMPLING_FACTOR 8
volatile uint16_t analog_get_V_sample_calibration(void)
{
    uint32_t sum = 0;
    uint16_t i;

    // Sum the configured number of samples.
    for (i = 0; i < (1 << OVERSAMPLING_FACTOR); i++) {
        adc_start_conversion(_circuit->V_adc, V_ADC_CH);
        adc_wait_for_interrupt_flag(_circuit->V_adc, V_ADC_CH);
        sum += adc_get_result(_circuit->V_adc, V_ADC_CH);
    }

    // Compute sample mean by scaling down according to oversampling factor.
    sum >>= OVERSAMPLING_FACTOR;

    return (uint16_t) sum;
}
volatile uint16_t analog_get_I_sample_calibration(void)
{
    uint32_t sum = 0;
    uint16_t i;

    // Sum the configured number of samples.
    for (i = 0; i < (1 << OVERSAMPLING_FACTOR); i++) {
        adc_start_conversion(_circuit->I_adc, I_ADC_CH);
        adc_wait_for_interrupt_flag(_circuit->I_adc, I_ADC_CH);
        sum += adc_get_result(_circuit->I_adc, I_ADC_CH);
    }

    // Compute sample mean by scaling down according to oversampling factor.
    sum >>= OVERSAMPLING_FACTOR;

    return (uint16_t) sum;
}

#define N_MEASURES_1_SEC 166666
volatile uint16_t analog_get_I_rms_sample_calibration(uint16_t i_mean)
{
    uint32_t sum = 0, i;
    uint16_t measure;

    // 1 segundo
    for (i = 0; i < N_MEASURES_1_SEC; i++) {
        adc_start_conversion(_circuit->I_adc, I_ADC_CH);
        adc_wait_for_interrupt_flag(_circuit->I_adc, I_ADC_CH);
        measure = adc_get_result(_circuit->I_adc, I_ADC_CH);
        sum += (measure-i_mean) * (measure-i_mean);
    }

    // Compute sample mean by scaling down according to oversampling factor.
    return (uint16_t) (sum / N_MEASURES_1_SEC);
}
volatile uint16_t analog_get_V_rms_sample_calibration(uint16_t v_mean)
{
    uint32_t sum = 0, i;
    uint16_t measure;
    float v_no_mean;

    // 1 segundo
    for (i = 0; i < N_MEASURES_1_SEC; i++) {
        adc_start_conversion(_circuit->V_adc, V_ADC_CH);
        adc_wait_for_interrupt_flag(_circuit->V_adc, V_ADC_CH);
        measure = adc_get_result(_circuit->V_adc, V_ADC_CH);
        v_no_mean = measure-v_mean;
        sum += (v_no_mean) * (v_no_mean);
    }

    // Compute sample mean by scaling down according to oversampling factor.
    return (uint16_t) (sum / N_MEASURES_1_SEC);
}
void _channel_config(ADC_t *adc, enum adcch_positive_input pin, uint8_t ch_mask)
{
    struct adc_config adc_conf;
    struct adc_channel_config adcch_conf;

    adc_read_configuration(adc, &adc_conf);
    adcch_read_configuration(adc, ch_mask, &adcch_conf);

    /* Configure the ADC module:
     * - unsigned, 12-bit results
     * - bandgap (1 V) voltage reference
     * - 200 kHz maximum clock rate
     * - manual conversion triggering
     */
    adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12, ADC_REF_BANDGAP);
    adc_set_clock_rate(&adc_conf, ADC_FREQ);
    adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 2, 0);
    adc_write_configuration(adc, &adc_conf);

    /* Configure ADC channel 0:
     * - single-ended measurement from configured input pin
     * - interrupt flag set on completed conversion
     */
    adcch_set_input(&adcch_conf, pin, ADCCH_NEG_NONE, 1);
    adcch_set_interrupt_mode(&adcch_conf, ADCCH_MODE_COMPLETE);
    adcch_disable_interrupt(&adcch_conf);
    adcch_write_configuration(adc, ch_mask, &adcch_conf);

    adc_enable(adc);
    //Warm Up
    adc_start_conversion(adc, ch_mask);
    adc_wait_for_interrupt_flag(adc, ch_mask);
}

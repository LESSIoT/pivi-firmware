
#include "measurements.h"
#include "communication.h"
#include "firmware.h"
#include "analog.h"
#include "time.h"
#include "asf.h"

#include "gpio.h"

volatile float I_samples_buffer[I_SAMPLES_BUFF_SIZE];
volatile uint16_t I_samples_count, V_samples_count;

volatile float I_rms_acc, V_rms_acc, power_acc;

volatile uint8_t measuring;

#define PA1 IOPORT_CREATE_PIN(PORTA, 1)
#define PA0 IOPORT_CREATE_PIN(PORTA, 0)

/* internal functions prototypes */
void measure_V_sample(void);
void measure_I_sample(void);
/***/

void measure_V_sample(void)
{
    float V_sample = analog_get_V_sample();

    uint16_t idx = V_samples_count % I_SAMPLES_BUFF_SIZE;

    V_rms_acc += V_sample * V_sample;
    power_acc += V_sample * I_samples_buffer[idx];

    if (!V_samples_count)
    {
        time_set_V_period();
    }

    if (++V_samples_count == N_SAMPLES)
    {
        time_stop_V_timer();
        measuring = false;
    }
}

void measure_I_sample(void)
{
    uint16_t idx = I_samples_count % I_SAMPLES_BUFF_SIZE;

    I_samples_buffer[idx] = analog_get_I_sample();
    I_rms_acc += I_samples_buffer[idx] * I_samples_buffer[idx];

    if (++I_samples_count == N_SAMPLES)
    {
        time_stop_I_timer();
    }
}

void measure(circuit_t *circuit)
{
    measurement_packet_t packet;

    I_samples_count = 0; V_samples_count = 0;
    I_rms_acc = 0; V_rms_acc = 0; power_acc = 0;

    analog_config(circuit);

    measure_I_sample();
    time_start_timers(circuit->delay);
    time_set_V_callback(measure_V_sample);
    time_set_I_callback(measure_I_sample);

    measuring = true;

    /* wait until all the samples are taken */
    while (measuring);
    packet.circuit_id = circuit->circuit_id;

    packet.real_power = (power_acc / N_SAMPLES);
    packet.irms = (I_rms_acc / N_SAMPLES);
    packet.vrms = (V_rms_acc / N_SAMPLES);

    send_to_pi(&packet);
}

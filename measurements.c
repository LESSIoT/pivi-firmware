/*
 * This file is part of the Pivi firmware package.
 * Released under the Creative Commons ATT-NC-ShareAlire license
 * http://creativecommons.org/licenses/by-nc-sa/4.0/
 * Copyright (C) 2014, 2015 LESS Industries S.A.
 * Author: Diego Mascialino <dmascialino@gmail.com>
 */

#define BUF_SIZE 80 //only for debug
char buf[BUF_SIZE],buf2[BUF_SIZE];
#include <math.h>
#include <stdio.h>
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
int i=0;
float sum_measure = 0;

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
    if (ioport_get_value(circuit->ct_detector_pin))
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

        // wait until all the samples are taken 
        while (measuring);
        packet.circuit_id = circuit->circuit_id;
        packet.real_power = (power_acc / N_SAMPLES);
        packet.irms = (I_rms_acc / N_SAMPLES);
        //sprintf(buf,"\n irms = %i \n",(int)I_rms_acc); //debug only
        debug_to_pi(buf);
        packet.vrms = (V_rms_acc / N_SAMPLES);
        send_to_pi(&packet);

        dtostrf(packet.vrms, 7, 4, buf);
        debug_to_pi(buf);
    }  
}


float measure_for_calibration(circuit_t *circuit)
{   
    float repeat_measure = 3.0; 
    sum_measure = 0.0;
    for(i=0;i<repeat_measure;i++)
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

        // wait until all the samples are taken 
        while (measuring);
        packet.circuit_id = circuit->circuit_id;
        packet.irms = (I_rms_acc / N_SAMPLES);

        packet.vrms =(V_rms_acc / N_SAMPLES);
        sum_measure += packet.vrms;
        dtostrf(packet.vrms, 7, 4, buf);
        sprintf(buf2,"\n  circuit id = %d \n",packet.circuit_id); //debug only
        debug_to_pi(buf2);
        debug_to_pi(buf);
    }
    dtostrf((sum_measure/repeat_measure),7,4,buf);
    debug_to_pi(buf);
    return(sum_measure/repeat_measure);   
}





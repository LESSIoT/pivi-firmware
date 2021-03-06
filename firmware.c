/*
 * This file is part of the Pivi firmware package.
 * Released under the Creative Commons ATT-NC-ShareAlire license
 * http://creativecommons.org/licenses/by-nc-sa/4.0/
 * Copyright (C) 2014, 2015 LESS Industries S.A.
 * Author: Diego Mascialino <dmascialino@gmail.com>
 */


#include "firmware.h"
#include "communication.h"
#include "time.h"
#include "measurements.h"
#include "conf_board.h"
#include "sysclk.h"
#include "asf.h"
#include "board_calibration.h"
#include <stdio.h>
#define N_CIRCUITS 6

#define SIZE 80 //only for debug
char buf1[SIZE],buf2[SIZE];

circuit_t CIRCUITS[] = {
        { .circuit_id = 1,
          .V_adc = &ADCA, .V_pin = ADCCH_POS_PIN7,
          .I_adc = &ADCA, .I_pin = ADCCH_POS_PIN1,
          .V_dc_offset = C1_V_OFFSET, .I_dc_offset = C1_I_OFFSET,
          .V_ac_offset = C1_VAC_OFFSET, .I_ac_offset = C1_IAC_OFFSET,
          .V_gain = C1_V_GAIN, .I_gain = C1_I_GAIN,
          .delay = C1_DELAY,
          .ct_detector_pin = CT1_DETECTOR,      
          .I_ref = ADC_REF_AREFA,
          .V_ref = ADC_REF_AREFA
        },
        { .circuit_id = 2,
          .V_adc = &ADCA, .V_pin = ADCCH_POS_PIN7,
          .I_adc = &ADCA, .I_pin = ADCCH_POS_PIN2,
          .V_dc_offset = C2_V_OFFSET, .I_dc_offset = C2_I_OFFSET,
          .V_ac_offset = C2_VAC_OFFSET, .I_ac_offset = C2_IAC_OFFSET,
          .V_gain = C2_V_GAIN, .I_gain = C2_I_GAIN,
          .delay = C2_DELAY,
          .ct_detector_pin = CT2_DETECTOR,
          .I_ref = ADC_REF_AREFA,
          .V_ref = ADC_REF_AREFA
        },
        { .circuit_id = 3,
          .V_adc = &ADCB, .V_pin = ADCCH_POS_PIN1,
          .I_adc = &ADCA, .I_pin = ADCCH_POS_PIN3,
          .V_dc_offset = C3_V_OFFSET, .I_dc_offset = C3_I_OFFSET,
          .V_ac_offset = C3_VAC_OFFSET, .I_ac_offset = C3_IAC_OFFSET,
          .V_gain = C3_V_GAIN, .I_gain = C3_I_GAIN,
          .delay = C3_DELAY,
          .ct_detector_pin = CT3_DETECTOR,
          .I_ref = ADC_REF_AREFA,
          .V_ref = ADC_REF_AREFB

        },
        { .circuit_id = 4,
          .V_adc = &ADCB, .V_pin = ADCCH_POS_PIN1,
          .I_adc = &ADCA, .I_pin = ADCCH_POS_PIN4,
          .V_dc_offset = C4_V_OFFSET, .I_dc_offset = C4_I_OFFSET,
          .V_ac_offset = C4_VAC_OFFSET, .I_ac_offset = C4_IAC_OFFSET,
          .V_gain = C4_V_GAIN, .I_gain = C4_I_GAIN,
          .delay = C4_DELAY,
          .ct_detector_pin = CT4_DETECTOR,
          .I_ref = ADC_REF_AREFA,
          .V_ref = ADC_REF_AREFB
        },
        { .circuit_id = 5,
          .V_adc = &ADCB, .V_pin = ADCCH_POS_PIN2,
          .I_adc = &ADCA, .I_pin = ADCCH_POS_PIN5,
          .V_dc_offset = C5_V_OFFSET, .I_dc_offset = C5_I_OFFSET,
          .V_ac_offset = C5_VAC_OFFSET, .I_ac_offset = C5_IAC_OFFSET,
          .V_gain = C5_V_GAIN, .I_gain = C5_I_GAIN,
          .delay = C5_DELAY,
          .ct_detector_pin = CT5_DETECTOR,
          .I_ref = ADC_REF_AREFA,
          .V_ref = ADC_REF_AREFB
        },
        { .circuit_id = 6,
          .V_adc = &ADCB, .V_pin = ADCCH_POS_PIN2,
          .I_adc = &ADCA, .I_pin = ADCCH_POS_PIN6,
          .V_dc_offset = C6_V_OFFSET, .I_dc_offset = C6_I_OFFSET,
          .V_ac_offset = C6_VAC_OFFSET, .I_ac_offset = C6_IAC_OFFSET,
          .V_gain = C6_V_GAIN, .I_gain = C6_I_GAIN,
          .delay = C6_DELAY,
          .ct_detector_pin = CT6_DETECTOR,
          .I_ref = ADC_REF_AREFA,
          .V_ref = ADC_REF_AREFB
        },
};


#ifdef FIRMWARE_FOR_CALIBRATION
#include "analog.h"
#include <math.h>

int main(void)
{
    uint8_t circuit_idx = 0;
    int i,circuits_to_cal[N_CIRCUITS];
    volatile uint32_t v_mean, i_mean;
    float v_measure = 0.0, i_measure = 0.0;
    board_init();
    sysclk_init();
    communication_init();
    time_init();
     /*
     *  V_Offset, I_offset, tension y corriente 0
     *  - esperar sart desde la PC
     *  - Medir y promediar el canal de tension
     *  - Medir y promediar el canal de corriente
     *  - enviar resultados
     *  V_gain, I_gain, se le pone una tension y corriente conocidas
     *  - esperar sart desde la PC
     *  - Medir y promediar el canal de tension
     *  - Medir y promediar el canal de corriente
     *  - enviar resultados
     */

    for(circuit_idx=0; circuit_idx<N_CIRCUITS; circuit_idx++)
    {
        circuits_to_cal[circuit_idx] = (((int)getchar_from_pi() - '0') - 1);
    }


    for(circuit_idx=0; circuit_idx < N_CIRCUITS && circuits_to_cal[circuit_idx] < N_CIRCUITS; circuit_idx++)
    {   
            getchar_from_pi();

            analog_config(&CIRCUITS[circuits_to_cal[circuit_idx]]);
            
            v_mean = analog_get_V_sample_calibration();
            i_mean = analog_get_I_sample_calibration();
            
            CIRCUITS[circuits_to_cal[circuit_idx]].V_dc_offset=v_mean;
            CIRCUITS[circuits_to_cal[circuit_idx]].I_dc_offset=i_mean;
            CIRCUITS[circuits_to_cal[circuit_idx]].V_gain=100;
            CIRCUITS[circuits_to_cal[circuit_idx]].I_gain=100;
            CIRCUITS[circuits_to_cal[circuit_idx]].V_ac_offset= 0.0;
            CIRCUITS[circuits_to_cal[circuit_idx]].I_ac_offset= 0.0;
            
            send_to_pi_mean_calibration(v_mean, i_mean);
            
            for(i=0;i<3;i++)
            {    
                getchar_from_pi(); //se mide offset AC, luego se toma 220 para calcular gain, luego medicion de pruebas
                
                measure_for_calibration(&CIRCUITS[circuits_to_cal[circuit_idx]],&v_measure,&i_measure);
                
                if(i==0)
                {
                    CIRCUITS[circuits_to_cal[circuit_idx]].V_ac_offset = v_measure;
                    CIRCUITS[circuits_to_cal[circuit_idx]].I_ac_offset = i_measure;
                }

                dtostrf(v_measure, 7, 4, buf1);
                dtostrf(i_measure, 7, 4, buf2);

                debug_to_pi(buf1); //TODO Replace this function with a proper one like send_to_pi_mean_calibration()
                debug_to_pi(buf2);
            }
    }
}
#else



void init_pins()
{
    ioport_configure_pin(CT1_DETECTOR, IOPORT_DIR_INPUT | IOPORT_MODE_PULLUP);
    ioport_configure_pin(CT2_DETECTOR, IOPORT_DIR_INPUT | IOPORT_MODE_PULLUP);
    ioport_configure_pin(CT3_DETECTOR, IOPORT_DIR_INPUT | IOPORT_MODE_PULLUP);
    ioport_configure_pin(CT4_DETECTOR, IOPORT_DIR_INPUT | IOPORT_MODE_PULLUP);
    ioport_configure_pin(CT5_DETECTOR, IOPORT_DIR_INPUT | IOPORT_MODE_PULLUP);
    ioport_configure_pin(CT6_DETECTOR, IOPORT_DIR_INPUT | IOPORT_MODE_PULLUP);
}

int main(void)
{
    uint8_t circuit_idx = 0;
    board_init();
    sysclk_init();
    communication_init();
    time_init();
    wdt_set_timeout_period(WDT_TIMEOUT_PERIOD_8KCLK); // 8s.
    wdt_enable();
    init_pins();
    while (true) 
    {
        wdt_reset();
        // if measure takes more than 8s to complete we'll have to add
        // a call to wdt_reset() into measure_I_sample() function.
        measure(&CIRCUITS[circuit_idx]);
        circuit_idx = (circuit_idx + 1) % N_CIRCUITS;
    }   
}
#endif

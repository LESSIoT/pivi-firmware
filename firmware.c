
#include "firmware.h"
#include "communication.h"
#include "time.h"
#include "measurements.h"
#include "conf_board.h"
#include "sysclk.h"
#include "asf.h"
#include "board_calibration.h"

#define N_CIRCUITS 6

circuit_t CIRCUITS[] = {
        { .circuit_id = 1,
          .V_adc = &ADCA, .V_pin = ADCCH_POS_PIN0,
          .I_adc = &ADCA, .I_pin = ADCCH_POS_PIN6,
          .V_dc_offset = C1_V_OFFSET, .I_dc_offset = C1_I_OFFSET,
          .V_gain = C1_V_GAIN, .I_gain = C1_V_GAIN,
          .delay = C1_DELAY
        },
        { .circuit_id = 2,
          .V_adc = &ADCA, .V_pin = ADCCH_POS_PIN1,
          .I_adc = &ADCA, .I_pin = ADCCH_POS_PIN7,
          .V_dc_offset = C2_V_OFFSET, .I_dc_offset = C2_I_OFFSET,
          .V_gain = C2_V_GAIN, .I_gain = C2_V_GAIN,
          .delay = C2_DELAY
        },
        { .circuit_id = 3,
          .V_adc = &ADCA, .V_pin = ADCCH_POS_PIN2,
          .I_adc = &ADCB, .I_pin = ADCCH_POS_PIN0,
          .V_dc_offset = C3_V_OFFSET, .I_dc_offset = C3_I_OFFSET,
          .V_gain = C3_V_GAIN, .I_gain = C3_V_GAIN,
          .delay = C3_DELAY
        },
        { .circuit_id = 4,
          .V_adc = &ADCA, .V_pin = ADCCH_POS_PIN3,
          .I_adc = &ADCB, .I_pin = ADCCH_POS_PIN1,
          .V_dc_offset = C4_V_OFFSET, .I_dc_offset = C4_I_OFFSET,
          .V_gain = C4_V_GAIN, .I_gain = C4_V_GAIN,
          .delay = C4_DELAY
        },
        { .circuit_id = 5,
          .V_adc = &ADCA, .V_pin = ADCCH_POS_PIN4,
          .I_adc = &ADCB, .I_pin = ADCCH_POS_PIN2,
          .V_dc_offset = C5_V_OFFSET, .I_dc_offset = C5_I_OFFSET,
          .V_gain = C5_V_GAIN, .I_gain = C5_V_GAIN,
          .delay = C5_DELAY
        },
        { .circuit_id = 6,
          .V_adc = &ADCA, .V_pin = ADCCH_POS_PIN5,
          .I_adc = &ADCB, .I_pin = ADCCH_POS_PIN3,
          .V_dc_offset = C6_V_OFFSET, .I_dc_offset = C6_I_OFFSET,
          .V_gain = C6_V_GAIN, .I_gain = C6_V_GAIN,
          .delay = C6_DELAY
        },
};

#ifdef FIRMWARE_FOR_CALIBRATION
#include "analog.h"
int main(void)
{
    uint8_t circuit_idx = 0;
    volatile uint16_t v_mean, i_mean, v_gain_rms2, i_gain_rms2;

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
     *  V_gian, I_gain, se le pone una tension y corriente conocidas
     *  - esperar sart desde la PC
     *  - Medir y promediar el canal de tension
     *  - Medir y promediar el canal de corriente
     *  - enviar resultados
     */

    for(circuit_idx=0; circuit_idx<N_CIRCUITS; circuit_idx++)
    {
        getchar_from_pi();
        analog_config(&CIRCUITS[circuit_idx]);
        v_mean = analog_get_V_sample_calibration();
        i_mean = analog_get_I_sample_calibration();
        send_to_pi_mean_calibration(v_mean, i_mean);

        getchar_from_pi();
        v_gain_rms2 = analog_get_V_rms_sample_calibration(v_mean);
        i_gain_rms2 = analog_get_I_rms_sample_calibration(i_mean);
        send_to_pi_gain_calibration(v_gain_rms2, i_gain_rms2);
    }
}
#else
int main(void)
{
    uint8_t circuit_idx = 0;

    board_init();
    sysclk_init();
    communication_init();
    time_init();

    wdt_set_timeout_period(WDT_TIMEOUT_PERIOD_8KCLK); // 8s.
    wdt_enable();
    while (true) {
        wdt_reset();
        // if measure takes more than 8s to complete we'll have to add
        // a call to wdt_reset() into measure_I_sample() function.
        measure(&CIRCUITS[circuit_idx]);
        circuit_idx = (circuit_idx + 1) % N_CIRCUITS;
    }
}
#endif


#include "firmware.h"
#include "communication.h"
#include "time.h"
#include "measurements.h"
#include "conf_board.h"
#include "sysclk.h"
#include "asf.h"

#define N_CIRCUITS 6

circuit_t CIRCUITS[] = {
        { .circuit_id = 1,
          .V_adc = &ADCA, .V_pin = ADCCH_POS_PIN0,
          .I_adc = &ADCA, .I_pin = ADCCH_POS_PIN6,
          .V_dc_offset = 0,
          .V_gain = 1, .I_gain = 1,
          .delay = 100
        },
        { .circuit_id = 2,
          .V_adc = &ADCA, .V_pin = ADCCH_POS_PIN1,
          .I_adc = &ADCA, .I_pin = ADCCH_POS_PIN7,
          .V_dc_offset = 0,
          .V_gain = 1, .I_gain = 1,
          .delay = 100
        },
        { .circuit_id = 3,
          .V_adc = &ADCA, .V_pin = ADCCH_POS_PIN2,
          .I_adc = &ADCB, .I_pin = ADCCH_POS_PIN0,
          .V_dc_offset = 0,
          .V_gain = 1, .I_gain = 1,
          .delay = 100
        },
        { .circuit_id = 4,
          .V_adc = &ADCA, .V_pin = ADCCH_POS_PIN3,
          .I_adc = &ADCB, .I_pin = ADCCH_POS_PIN1,
          .V_dc_offset = 0,
          .V_gain = 1, .I_gain = 1,
          .delay = 100
        },
        { .circuit_id = 5,
          .V_adc = &ADCA, .V_pin = ADCCH_POS_PIN4,
          .I_adc = &ADCB, .I_pin = ADCCH_POS_PIN2,
          .V_dc_offset = 0,
          .V_gain = 1, .I_gain = 1,
          .delay = 100
        },
        { .circuit_id = 6,
          .V_adc = &ADCA, .V_pin = ADCCH_POS_PIN5,
          .I_adc = &ADCB, .I_pin = ADCCH_POS_PIN3,
          .V_dc_offset = 0,
          .V_gain = 1, .I_gain = 1,
          .delay = 100
        },
};

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

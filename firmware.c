
#include "conf_board.h"
#include "sysclk.h"
#include "communication.h"
#include "time.h"
#include "asf.h"
#include "firmware.h"
#include "measurements.h"


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

	while (true) {
		measure(CIRCUITS + circuit_idx);
		circuit_idx = (circuit_idx + 1) % N_CIRCUITS;
	}
}

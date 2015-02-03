
#include "conf_board.h"
#include "sysclk.h"
#include "communication.h"
#include "time.h"
#include "asf.h"
#include "firmware.h"
#include "measurements.h"

int main(void)
{
	circuit_t circuit;
	board_init();
	sysclk_init();
	communication_init();
	time_init();

	circuit.circuit_id = 1;
	circuit.delay = 1000;
	circuit.V_gain = 1;
	circuit.I_gain = 1;
	circuit.V_dc_offset = 0;

	while (true) {
	    measure(&circuit);
	}
}

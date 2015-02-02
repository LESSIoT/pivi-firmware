
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

	/* TEST send_to_pi function */
	measurement_packet_t packet;
	packet.circuit_id = 1;
	packet.real_power = ((uint16_t) 0x7d<<8) | 2;
	packet.irms = ((uint16_t) 0x7e<<8) | 4;
	packet.vrms = ((uint16_t) 0x7f<<8) | 8;

	circuit.circuit_id = 1;
	circuit.delay = 1000;
	circuit.V_gain = 1;
	circuit.I_gain = 1;
	circuit.V_dc_offset = 0;

	while (true) {
		getchar_from_pi();
		debug_to_pi("entering measure");
		measure(&circuit);
		debug_to_pi("end measure");
		//send_to_pi(&packet);
	}
	/* END TEST send_to_pi function */
}

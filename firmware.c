
#include "conf_board.h"
#include "sysclk.h"
#include "communication.h"
#include "asf.h"

int main(void)
{
	/* Initialize the board.
	 * The board-specific conf_board.h file contains the configuration of
	 * the board initialization.
	 */
	board_init();
	sysclk_init();
	communication_init();

	/* TEST send_to_pi function */
	measurement_packet_t packet;
	packet.circuit_id = 1;
	packet.real_power = ((uint16_t) 0x7d<<8) | 2;
	packet.irms = ((uint16_t) 0x7e<<8) | 4;
	packet.vrms = ((uint16_t) 0x7f<<8) | 8;

	while (true) {
		getchar_from_pi();
		send_to_pi(&packet);
	}
	/* END TEST send_to_pi function */
}


#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#include "asf.h"


typedef struct measurement_packet {
	uint16_t circuit_id;
	uint16_t real_power;
	uint16_t irms;
	uint16_t vrms;
} measurement_packet_t;


void communication_init(void);

uint8_t getchar_from_pi(void);
void debug_to_pi_uint16(const uint16_t c);
void debug_to_pi(const char *c);
void send_to_pi(const measurement_packet_t *packet);

#endif //_COMMUNICATION_H

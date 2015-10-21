/*
 * This file is part of the Pivi firmware package.
 * Released under the Creative Commons ATT-NC-ShareAlire license
 * http://creativecommons.org/licenses/by-nc-sa/4.0/
 * Copyright (C) 2014, 2015 LESS Industries S.A.
 * Author: Diego Mascialino <dmascialino@gmail.com>
 */

#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#include <stdint.h>

typedef struct measurement_packet {
    uint16_t circuit_id;
    float real_power;
    float irms;
    float vrms;
} measurement_packet_t;

void communication_init(void);
void send_to_pi(const measurement_packet_t *packet);
void send_to_pi_mean_calibration(const uint16_t v_measure, const uint16_t i_measure);

/* Debug functions */
uint8_t getchar_from_pi(void);
void debug_to_pi(const char *c);


/* USART configuration */
#define USART_SERIAL                     &USARTC0
#define USART_SERIAL_BAUDRATE            115200
#define USART_SERIAL_CHAR_LENGTH         USART_CHSIZE_8BIT_gc
#define USART_SERIAL_PARITY              USART_PMODE_DISABLED_gc
#define USART_SERIAL_STOP_BIT            false

#endif //_COMMUNICATION_H

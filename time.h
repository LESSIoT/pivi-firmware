/*
 * This file is part of the Pivi firmware package.
 * Released under the Creative Commons ATT-NC-ShareAlire license
 * http://creativecommons.org/licenses/by-nc-sa/4.0/
 * Copyright (C) 2014, 2015 LESS Industries S.A.
 * Author: Diego Mascialino <dmascialino@gmail.com>
 */



#ifndef _TIME_H
#define _TIME_H

#include <tc.h>

#define I_TIMER &TCC0
#define V_TIMER &TCC1

#define TIMER_FREQ      4000000 // 0.25us resolution
#define TIMER_PERIOD    1000    // (1/4 Khz) / 0.25us

void time_init(void);
void time_start_timers(uint16_t v_delay);
void time_set_V_period(void);

void time_set_I_callback(tc_callback_t callback);
void time_set_V_callback(tc_callback_t callback);

void time_stop_I_timer(void);
void time_stop_V_timer(void);

#endif // _TIME_H

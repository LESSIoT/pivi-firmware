

#ifndef _TIME_H
#define _TIME_H

#include <tc.h>

#define I_TIMER &TCC0
#define V_TIMER &TCC1

#define TIMER_PERIOD 31250  //FIXME: this value is just for test

void time_init(void);
void time_start_timers(uint16_t v_delay);
void time_set_V_period(void);

void time_set_I_callback(tc_callback_t callback);
void time_set_V_callback(tc_callback_t callback);

void time_stop_I_timer(void);
void time_stop_V_timer(void);

#endif // _TIME_H

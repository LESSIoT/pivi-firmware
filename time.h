

#ifndef _TIME_H
#define _TIME_H

#include "asf.h"

#define I_TIMER &TCC0
#define V_TIMER &TCC1

#define TIMER_PERIOD 31250  //FIXME: this value is just for test

void time_init(void);
void time_start(void);

#endif // _TIME_H


#ifndef _MEASUREMENTS_H
#define _MEASUREMENTS_H

#include "firmware.h"

void measure(circuit_t *circuit);

#define N_SAMPLES_BY_PERIOD 20
#define I_SAMPLES_BUFF_SIZE (N_SAMPLES_BY_PERIOD / 1)
#define N_PERIODS 1
#define N_SAMPLES (N_PERIODS * N_SAMPLES_BY_PERIOD)

#endif // _MEASUREMENTS_H

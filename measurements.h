/*
 * This file is part of the Pivi firmware package.
 * Released under the Creative Commons ATT-NC-ShareAlire license
 * http://creativecommons.org/licenses/by-nc-sa/4.0/
 * Copyright (C) 2014, 2015 LESS Industries S.A.
 * Author: Diego Mascialino <dmascialino@gmail.com>
 */


#ifndef _MEASUREMENTS_H
#define _MEASUREMENTS_H

#include "firmware.h"

void measure(circuit_t *circuit);
void measure_for_calibration(circuit_t *circuit,float *v_sample,float *i_sample);

#define N_SAMPLES_BY_PERIOD 80
#define I_SAMPLES_BUFF_SIZE (N_SAMPLES_BY_PERIOD / 10)
#define N_PERIODS 100
#define N_SAMPLES (N_PERIODS * N_SAMPLES_BY_PERIOD)

#endif // _MEASUREMENTS_H

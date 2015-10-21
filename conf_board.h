/*
 * This file is part of the Pivi firmware package.
 * Released under the Creative Commons ATT-NC-ShareAlire license
 * http://creativecommons.org/licenses/by-nc-sa/4.0/
 * Copyright (C) 2014, 2015 LESS Industries S.A.
 * Author: Diego Mascialino <dmascialino@gmail.com>
 */


#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

#define CONF_BOARD_ENABLE_USARTC0
#define CONF_BOARD_ENABLE_AC_PINS

#define CT1_DETECTOR  IOPORT_CREATE_PIN(PORTF, 0)
#define CT2_DETECTOR  IOPORT_CREATE_PIN(PORTF, 1)
#define CT3_DETECTOR  IOPORT_CREATE_PIN(PORTF, 2)
#define CT4_DETECTOR  IOPORT_CREATE_PIN(PORTF, 3)
#define CT5_DETECTOR  IOPORT_CREATE_PIN(PORTF, 4)
#define CT6_DETECTOR  IOPORT_CREATE_PIN(PORTF, 5)

#endif /* CONF_BOARD_H_INCLUDED */

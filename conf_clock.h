/*
 * This file is part of the Pivi firmware package.
 * Released under the Creative Commons ATT-NC-ShareAlire license
 * http://creativecommons.org/licenses/by-nc-sa/4.0/
 * Copyright (C) 2014, 2015 LESS Industries S.A.
 * Author: Diego Mascialino <dmascialino@gmail.com>
 */


#ifndef CONF_CLOCK_H_INCLUDED
#define CONF_CLOCK_H_INCLUDED

#define CONFIG_SYSCLK_SOURCE                SYSCLK_SRC_RC32MHZ
#define CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC  OSC_ID_XOSC

#define CONFIG_SYSCLK_PSADIV          SYSCLK_PSADIV_1
#define CONFIG_SYSCLK_PSBCDIV         SYSCLK_PSBCDIV_1_1
#define CONFIG_USBCLK_SOURCE          USBCLK_SRC_RCOSC

#endif /* CONF_CLOCK_H_INCLUDED */

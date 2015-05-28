
/*  This file is part of the HWA project.
 *  Copyright (c) 2012,2015 Christophe Duparquet.
 *  All rights reserved. Read LICENSE.TXT for details.
 */

#ifndef CONFIG_H
#define CONFIG_H

#define hw_swuart0_pin_tx		DIABOLO_PIN_TX
#define hw_swuart0_pin_rx		DIABOLO_PIN_RX
#define hw_swuart0_autosync		sync_5_1
#define hw_swuart0_counter		hw_counter1
#define hw_swuart0_counter_clk_div	1
#define hw_swuart0_counter_compare	compare0

#include <boards/attiny84.h>

#endif

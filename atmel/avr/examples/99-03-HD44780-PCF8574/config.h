
/*  This file is part of the HWA project.
 *  Copyright (c) 2018 Christophe Duparquet.
 *  All rights reserved. Read LICENSE.TXT for details.
 */

/*  Include the target board (and device) definitions
 */
#include BOARD_H

#include <hwa/swtwimaster.h>
#include <hwa/pcf8574.h>
#include <hwa/hd44780.h>

/* #define TWI		HW_SWTWIMASTER( scl, pc5,	\ */
/* 					sda, pc4,	\ */
/* 					bps, TWI_BPS ) */

#define TWI		twi0

#define TWI_BPS		100000

#define LCD		HW_HD44780( lines, 2,			\
				    cols,  16,			\
				    e,     (PCF, 1, 2),		\
				    rs,    (PCF, 1, 0),		\
				    rw,    (PCF, 1, 1),		\
				    data,  (PCF, 4, 4) )

/* #define LCD		HW_HD44780( lines, 2,			\ */
/* 				    cols,  16,			\ */
/* 				    e,     pb2,			\ */
/* 				    rs,    pb0,			\ */
/* 				    rw,    pb1,			\ */
/* 				    data,  (port1,4,4) ) */

#define PCF		HW_PCF8574( interface, TWI, address, 0x27 )

#define LCD_LED		(PCF, 1, 3)

HW_DECLARE( TWI );
HW_DECLARE( PCF );
HW_DECLARE( LCD );

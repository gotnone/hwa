
/*  This file is part of the HWA project.
 *  Copyright (c) 2018 Christophe Duparquet.
 *  All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @example
 *
 * @par Drive a LCD through a HD44780 controller connected to a PCF8574 TWI
 *
 * This example shows how HWA deals with external hardware. Here, a
 * HD44780-based LCD controller is driven through a PCF8574 I²C bus expander
 * connected to a two-wire interface (TWI). The TWI can be embedded into the MCU
 * or software-emulated.
 *
 * @par config.h
 * @include 99-03-HD44780-PCF8574/config.h
 *
 * @par main.c
 */

#include "config.h"
#include "xprintf.h"


HW_DEFINE(TWI);
HW_DEFINE(PCF);
HW_DEFINE(LCD);


int main(void)
{
  hw( configure, TWI, bps, TWI_BPS );
  hw( configure, PCF );

  hw( write, LCD_LED, 0 );
  hw( init, LCD );
  hw( write, LCD_LED, 1 );

  while(1) {
    int i = 0;
    int line = 0;
    for(i=0; i<1000 ; i++) {

      if ( line == 0 ) {
	hw( gotoxy, LCD, 0, line );
	xprintf( HW_FUNCTION(LCD,putchar), " g=%3d", i);
      }
      else {
	hw( gotoxy, LCD, 8, line );
	xprintf( HW_FUNCTION(LCD,putchar), " g=%03d", i);
      }

      line++;
      line %= 2;
      hw_waste_cycles( 0.25 * HW_SYSHZ );
    }
  }
}

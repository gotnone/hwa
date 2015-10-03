
/*  This file is part of the HWA project.
 *  Copyright (c) 2012,2015 Christophe Duparquet.
 *  All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @example
 *
 *  Blink a LED using a counter compare interrupt
 */


/*  Include the target board (and device) definitions
 */
#include BOARD_H


/*  The counter
 */
#define COUNTER                 hw_counter0
#define CLKDIV                  64
#define COUNTMODE               loop_up
#define COMPARE                 compare0
#define PERIOD                  0.5


/*  Service the compare-match IRQ
 */
HW_ISR( COUNTER, COMPARE )
{
  hw_write( COUNTER, 0 );

  static uint8_t n ;
  n++ ;
  if ( n >= (uint8_t)(0.5 + 1.0 * PERIOD / 2.0 / 0.001) ) {	/* Added 0.5 for rounding */
    n = 0 ;
    hw_toggle( PIN_LED );
  }
}


int main ( )
{
  /*  Create a HWA context to collect the hardware configuration
   *  Preload this context with RESET values
   */
  hwa_begin_from_reset();

  /*  Configure the LED pin
   */
  hwa_config( PIN_LED, direction, output );

  /*  Have the CPU enter idle mode when the 'sleep' instruction is executed.
   */
  hwa_config( hw_core0,
              sleep,      enabled,
              sleep_mode, idle );

  /*  Configure the compare unit to match every 0.001 s.
   */
  hwa_config( COUNTER,
              clock,     HW_G2(prescaler_output, CLKDIV),
              countmode, COUNTMODE,
              bottom,    0,
              top,       max
              );
  if ( hw_streq(HW_QUOTE(COUNTMODE),"loop_updown") )
    hwa_write( hw_rel(COUNTER, COMPARE), 0.5 + 0.001*hw_syshz/CLKDIV/2 );
  else /* loop_up */
    hwa_write( hw_rel(COUNTER, COMPARE), 0.5 + 0.001*hw_syshz/CLKDIV );

  /*  Enable compare IRQ
   */
  hwa_turn_irq( COUNTER, COMPARE, on );

  /*  Write this configuration into the hardware
   */
  hwa_commit();

  hw_enable_interrupts();

  for(;;)
    hw_sleep();
}

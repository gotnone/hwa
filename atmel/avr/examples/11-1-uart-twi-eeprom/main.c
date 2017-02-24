
/*  This file is part of the HWA project.
 *  Copyright (c) 2012,2015 Christophe Duparquet.
 *  All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @example
 * 
 * This is a translation of the twitest.c example taken from avr-libc (the
 * original file is provided is this directory):
 *
 * > Simple demo program that talks to a 24Cxx I<sup>2</sup>C EEPROM using the
 * > builtin TWI interface of an ATmega device.
 *
 * @note This version leads to a binary file bigger of 20 bytes compared to the
 * orignal `avr-libc/twitest.c`. It seems that GCC does not make the same
 * register assigments and sometimes puts useless code such as `eor r25, r25`
 * and later `ldi r25, 0`. Why?
 *
 * @note Revision 2016-06-29: use `hw_cmd()` instead of `hw_command()`. The
 * binary code is now 3474 bytes long with avr-gcc-4.9.2 (was 3336 with
 * avr-gcc-4.8.2). WHY?
 *
 * @note Revision 2017-02-19 (not tested): use `hw()` instead of `hw_cmd()`. The
 * binary code is now 3338 bytes long with avr-gcc-4.9.2.
 *
 * @par main.c
 **/

#include <stdio.h>
#include <stdlib.h>

#include BOARD_H

#define TWI     twi0

#define DEBUG 1

/*
 * Note [3]
 * TWI address for 24Cxx EEPROM:
 *
 * 1 0 1  0  E2 E1 E0   24C01/24C02
 * 1 0 1  0  E2 E1 A8   24C04
 * 1 0 1  0  E2 A9 A8   24C08
 * 1 0 1  0 A10 A9 A8   24C16
 * 1 0 1  0   0  0  0   24FC512
 */
#define TWI_SLA_24CXX   0xA0    /* 24FC512 */

/*
 * Note [3a]
 * Device word address length for 24Cxx EEPROM
 * Larger EEPROM devices (from 24C32) have 16-bit address
 * Define or undefine according to the used device
 */
#define WORD_ADDRESS_16BIT

/*
 * Maximal number of iterations to wait for a device to respond for a
 * selection.  Should be large enough to allow for a pending write to
 * complete, but low enough to properly abort an infinite loop in case
 * a slave is broken or not present at all.  With 100 kHz TWI clock,
 * transfering the start condition and SLA+R/W packet takes about 10
 * �s.  The longest write period is supposed to not exceed ~ 10 ms.
 * Thus, normal operation should not require more than 100 iterations
 * to get the device to respond to a selection.
 */
#define MAX_ITER        200

/*
 * Number of bytes that can be written in a row, see comments for
 * ee24xx_write_page() below.  Some vendor's devices would accept 16,
 * but 8 seems to be the lowest common denominator.
 *
 * Note that the page size must be a power of two, this simplifies the
 * page boundary calculations below.
 */
#define PAGE_SIZE 8

/*
 * Saved TWI status register, for error messages only.  We need to
 * save it in a variable, since the datasheet only guarantees the TWSR
 * register to have valid contents while the TWINT bit in TWCR is set.
 */
uint8_t twst;

/*
 * Do all the startup-time peripheral initializations: UART (for our
 * debug/test output), and TWI clock.
 */
void
ioinit(void)
{
  hwa_begin_from_reset();
  hwa( configure,   uart0,
       bps,         9600,
       receiver,    disabled,
       transmitter, enabled  );
  hwa( configure, TWI, sclhz, 100000 );
  hwa_commit();
}

/*
 * Note [6]
 * Send character c down the UART Tx, wait until tx holding register
 * is empty.
 */
int
uart_putchar(char c, FILE *unused __attribute__((unused)) )
{
  if (c == '\n')
    uart_putchar('\r',0);

  while ( !hw( stat, uart0 ).txqnf ) {}
  hw( write, uart0, c );
  
  return 0;
}


/*
 * Note [7]
 *
 * Read "len" bytes from EEPROM starting at "eeaddr" into "buf".
 *
 * This requires two bus cycles: during the first cycle, the device
 * will be selected (master transmitter mode), and the address
 * transfered.
 * Address bits exceeding 256 are transfered in the
 * E2/E1/E0 bits (subaddress bits) of the device selector.
 * Address is sent in two dedicated 8 bit transfers
 * for 16 bit address devices (larger EEPROM devices)
 *
 * The second bus cycle will reselect the device (repeated start
 * condition, going into master receiver mode), and transfer the data
 * from the device to the TWI master.  Multiple bytes can be
 * transfered by ACKing the client's transfer.  The last transfer will
 * be NACKed, which the client will take as an indication to not
 * initiate further transfers.
 */
int
ee24xx_read_bytes(uint16_t eeaddr, int len, uint8_t *buf)
{
  uint8_t sla, n = 0;
  int rv = 0;

#ifndef WORD_ADDRESS_16BIT
  /* patch high bits of EEPROM address into SLA */
  sla = TWI_SLA_24CXX | ((eeaddr >> 8) & 0x07);
#else
  /* 16-bit address devices need only TWI Device Address */
  sla = TWI_SLA_24CXX ;
#endif

  /*
   * Note [8]
   * First cycle: master transmitter mode
   */
 restart:
  if (n++ >= MAX_ITER)
    return -1;

 begin:
  hw( tx_start, TWI );                  /* send start condition */

  while( !hw(read, HW_IRQFLAG(TWI)) ) {}   /* wait for transmission */
  switch( (twst=hw(stat,TWI)) )
    {
      case HW_TWI_START:
      case HW_TWI_REP_START:            /* OK, but should not happen */
        break;

      case HW_TWI_MT_ARB_LOST:          /* Note [9] */
        goto begin;

      default:
        return -1;                      /* error: not in start condition */
                                        /* NB: do /not/ send stop condition */
    }

  /* Note [10]
   */
  hw( tx_slaw, TWI, sla>>1 );           /* send SLA+W */
  while( !hw(read, HW_IRQFLAG(TWI)) ) {}   /* wait for transmission */
  switch( (twst=hw(stat,TWI)) )
    {
      case HW_TWI_MT_SLA_ACK:
        break;

      case HW_TWI_MT_SLA_NACK:          /* nack during select: device busy writing */
        goto restart;                   /* Note [11] */

      case HW_TWI_MT_ARB_LOST:          /* re-arbitrate */
        goto begin;

      default:
        goto error;                     /* must send stop condition */
    }
  

#ifdef WORD_ADDRESS_16BIT

  hw( tx_data, TWI, eeaddr >> 8 );	/* highest 8 bits of addr */
  while( !hw(read, HW_IRQFLAG(TWI)) ) {}   /* wait for transmission */
  switch( (twst=hw(stat,TWI)) )
    {
      case HW_TWI_MT_DATA_ACK:
        break;

      case HW_TWI_MT_DATA_NACK:
        goto quit;

      case HW_TWI_MT_ARB_LOST:
        goto begin;

      default:
        goto error;                     /* must send stop condition */
    }

#endif

  hw( tx_data, TWI, eeaddr );           /* lowest 8 bits of addr */
  while( !hw(read, HW_IRQFLAG(TWI)) ) {}   /* wait for transmission */
  switch( (twst=hw(stat,TWI)) )
    {
      case HW_TWI_MT_DATA_ACK:
        break;

      case HW_TWI_MT_DATA_NACK:
        goto quit;

      case HW_TWI_MT_ARB_LOST:
        goto begin;

      default:
        goto error;                     /* must send stop condition */
    }

  /*
   * Note [12]
   * Next cycle(s): master receiver mode
   */
  hw( tx_start, TWI );                  /* send (rep.) start condition */
  while( !hw(read, HW_IRQFLAG(TWI)) ) {}   /* wait for transmission */
  switch( (twst=hw(stat,TWI)) )
    {
      case HW_TWI_START:                /* OK, but should not happen */
      case HW_TWI_REP_START:
        break;

      case HW_TWI_MT_ARB_LOST:
        goto begin;

      default:
        goto error;
    }

  hw( tx_slar, TWI, sla>>1 );           /* send SLA+R */
  while( !hw(read, HW_IRQFLAG(TWI)) ) {}   /* wait for transmission */
  switch( (twst=hw(stat,TWI)) )
    {
      case HW_TWI_MR_SLA_ACK:
        break;

      case HW_TWI_MR_SLA_NACK:
        goto quit;

      case HW_TWI_MR_ARB_LOST:
        goto begin;

      default:
        goto error;
    }

  /* Note [13]
   */
  for ( ; len>0 ; len-- ) {

    if (len == 1)
      hw( tx_read, TWI, nack );         /* ask last byte, send NACK*/
    else
      hw( tx_read, TWI, ack );          /* ask one more byte, send ACK*/

    while( !hw(read, HW_IRQFLAG(TWI)) ) {} /* wait for transmission */
    switch( (twst=hw(stat,TWI)) )
      {
        case HW_TWI_MR_DATA_NACK:
          len = 0;                      /* force end of loop */
          /* FALLTHROUGH */
        case HW_TWI_MR_DATA_ACK:
          *buf++ = hw(read,TWI);
          rv++;
          if(twst == HW_TWI_MR_DATA_NACK) goto quit;
          break;

        default:
          goto error;
      }
  }

 quit:
  /* Note [14]
   */
  hw( tx_stop, TWI );                   /* send stop condition */
  return rv;

 error:
  rv = -1;
  goto quit;
}

/*
 * Write "len" bytes into EEPROM starting at "eeaddr" from "buf".
 *
 * This is a bit simpler than the previous function since both, the
 * address and the data bytes will be transfered in master transmitter
 * mode, thus no reselection of the device is necessary.  However, the
 * EEPROMs are only capable of writing one "page" simultaneously, so
 * care must be taken to not cross a page boundary within one write
 * cycle.  The amount of data one page consists of varies from
 * manufacturer to manufacturer: some vendors only use 8-byte pages
 * for the smaller devices, and 16-byte pages for the larger devices,
 * while other vendors generally use 16-byte pages.  We thus use the
 * smallest common denominator of 8 bytes per page, declared by the
 * macro PAGE_SIZE above.
 *
 * The function simply returns after writing one page, returning the
 * actual number of data byte written.  It is up to the caller to
 * re-invoke it in order to write further data.
 */
int
ee24xx_write_page(uint16_t eeaddr, int len, uint8_t *buf)
{
  uint8_t sla, n = 0;
  int rv = 0;
  uint16_t endaddr;

  if (eeaddr + len <= (eeaddr | (PAGE_SIZE - 1)))
    endaddr = eeaddr + len;
  else
    endaddr = (eeaddr | (PAGE_SIZE - 1)) + 1;
  len = endaddr - eeaddr;

#ifndef WORD_ADDRESS_16BIT
  /* patch high bits of EEPROM address into SLA */
  sla = TWI_SLA_24CXX | ((eeaddr >> 8) & 0x07);
#else
  /* 16-bit address devices need only TWI Device Address */
  sla = TWI_SLA_24CXX ;
#endif

 restart:
  if (n++ >= MAX_ITER)
    return -1;
 begin:

  /* Note [15] */

  hw( tx_start, TWI );                  /* send start condition */
  while( !hw(read, HW_IRQFLAG(TWI)) ) {}   /* wait for transmission */
  switch( (twst=hw(stat,TWI)) )
    {
      case HW_TWI_REP_START:            /* OK, but should not happen */
      case HW_TWI_START:
        break;

      case HW_TWI_MT_ARB_LOST:
        goto begin;

      default:
        return -1;              /* error: not in start condition */
                                /* NB: do /not/ send stop condition */
    }

  hw( tx_slaw, TWI, sla>>1 );           /* send SLA+W */
  while( !hw(read, HW_IRQFLAG(TWI)) ) {}   /* wait for transmission */
  switch( (twst=hw(stat,TWI)) )
    {
      case HW_TWI_MT_SLA_ACK:
        break;

      case HW_TWI_MT_SLA_NACK:  /* nack during select: device busy writing */
        goto restart;

      case HW_TWI_MT_ARB_LOST:  /* re-arbitrate */
        goto begin;

      default:
        goto error;             /* must send stop condition */
    }

#ifdef WORD_ADDRESS_16BIT
  hw( tx_data, TWI, eeaddr>>8 );        /* 16 bit word address device, send high 8 bits of addr */
  while( !hw(read, HW_IRQFLAG(TWI)) ) {}   /* wait for transmission */
  switch( (twst=hw(stat,TWI)) )
    {
      case HW_TWI_MT_DATA_ACK:
        break;

      case HW_TWI_MT_DATA_NACK:
        goto quit;

      case HW_TWI_MT_ARB_LOST:
        goto begin;

      default:
        goto error;             /* must send stop condition */
    }
#endif

  hw( tx_data, TWI, eeaddr );           /* low 8 bits of addr */
  while( !hw(read, HW_IRQFLAG(TWI)) ) {}   /* wait for transmission */
  switch( (twst=hw(stat,TWI)) )
    {
      case HW_TWI_MT_DATA_ACK:
        break;

      case HW_TWI_MT_DATA_NACK:
        goto quit;

      case HW_TWI_MT_ARB_LOST:
        goto begin;

      default:
        goto error;                     /* must send stop condition */
    }


  for ( ; len>0 ; len-- ) {

    hw( tx_data, TWI, (*buf++) );

    while( !hw(read, HW_IRQFLAG(TWI)) ) {} /* wait for transmission */
    switch( (twst=hw(stat,TWI)) )
      {
        case HW_TWI_MT_DATA_NACK:
          goto error;                   /* device write protected -- Note [16] */

        case HW_TWI_MT_DATA_ACK:
          rv++;
          break;

        default:
          goto error;
      }
  }

 quit:
  hw( tx_stop, TWI );                   /* send stop condition */
  return rv;

 error:
  rv = -1;
  goto quit;
}

/*
 * Wrapper around ee24xx_write_page() that repeats calling this
 * function until either an error has been returned, or all bytes
 * have been written.
 */
int
ee24xx_write_bytes(uint16_t eeaddr, int len, uint8_t *buf)
{
  int rv, total;

  total = 0;
  do
    {
#if DEBUG
      printf("Calling ee24xx_write_page(%d, %d, %p)",
             eeaddr, len, buf);
#endif
      rv = ee24xx_write_page(eeaddr, len, buf);
#if DEBUG
      printf(" => %d\n", rv);
#endif
      if (rv == -1)
        return -1;

      eeaddr += rv;
      len -= rv;
      buf += rv;
      total += rv;
    }
  while (len > 0);

  return total;
}

void
error(void)
{
  printf("error: TWI status %#x\n", twst);
  exit(0);
}


FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);


int
main(void)
{
  uint16_t a;
  int rv;
  uint8_t b[16];
  uint8_t x;

  ioinit();

  stdout = &mystdout;

  for (a = 0; a < 256;)
    {
      printf("%#04x: ", a);
      rv = ee24xx_read_bytes(a, 16, b);
      if (rv <= 0)
        error();
      if (rv < 16)
        printf("warning: short read %d\n", rv);
      a += rv;
      for (x = 0; x < rv; x++)
        printf("%02x ", b[x]);
      putchar('\n');
    }

#define EE_WRITE(addr, str) ee24xx_write_bytes(addr, sizeof(str)-1, (uint8_t*)str)

  rv = EE_WRITE(55, "The quick brown fox jumps over the lazy dog.");
  if (rv < 0)
    error();
  printf("Wrote %d bytes.\n", rv);
  for (a = 0; a < 256;)
    {
      printf("%#04x: ", a);
      rv = ee24xx_read_bytes(a, 16, b);
      if (rv <= 0)
        error();
      if (rv < 16)
        printf("warning: short read %d\n", rv);
      a += rv;
      for (x = 0; x < rv; x++)
        printf("%02x ", b[x]);
      putchar('\n');
    }

  printf("done.\n");
  return 0;
}
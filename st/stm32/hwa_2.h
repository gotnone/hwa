
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief Definitions that produce C code specific to Atmel AVR devices
 */

/**
 * @ingroup public_ins_stm32
 * @brief Puts the core in sleep mode.
 */
//#define hw_sleep()			hw_asm("sleep")


/**
 * @ingroup public_ins_stm32
 * @brief Allows program interruption.
 */
//#define hw_enable_interrupts()		hw_asm("sei")


/**
 * @ingroup public_ins_stm32
 * @brief Prevents program interruption.
 */
//#define hw_disable_interrupts()		hw_asm("cli")


/**
 * @ingroup public_ins_stm32
 * @brief Software loop of \c n system clock cycles.
 */
#define hw_waste_cycles(n)		_hw_waste_cycles(n)



#include "../../hwa/hwa_2.h"



HW_INLINE void _hw_waste_cycles ( volatile uint32_t n )
{
  hw_asm("	.align			\n"
	 "1:	subs	%[r], %[r], #4	\n"
	 "	bcs.n	1b		\n"
	 : [r] "+r" (n)
	 :
	 );
}


/* This is a generic method that can be implemented by all peripheral
 * classes. An object supports power management if it has a logical register
 * named `cken`.
 */
#define _hw_mtd_hw_power		, _hw_power
#define _hw_mtd_hwa_power		, _hwa_power

#define _hw_power(c,o,i,a,v,g,...)	HW_X(_hwx_pwr1,g)(_hw,o,v,g)
#define _hwa_power(c,o,i,a,v,g,...)	HW_X(_hwx_pwr1,g)(_hwa,o,v,g)
#define _hwx_pwr1_0(x,o,v,g)		HW_E_G(g)
#define _hwx_pwr1_1(x,o,v,g)		HW_X(_hwx_pwr2,_hw_state_##v)(x,o,v)
#define _hwx_pwr2_0(x,o,v)		HW_E_ST(v)
#define _hwx_pwr2_1(x,o,v)		HW_X(_hwx_pwr3,HW_G2(_hw_isa_reg, _hw_reg_##o##_##cken))(x,o,v)
#define _hwx_pwr3_0(x,o,v)		HW_E(`o` does not support power management)
#define _hwx_pwr3_1(x,o,v)		x##_write_reg(o,cken,HW_A1(_hw_state_##v))


HW_INLINE void _hw_write_r32 ( intptr_t ra, uint32_t rwm, uint32_t rfm, uint32_t mask, uint32_t value )
{
#if defined HWA_CHECK_ACCESS
  if ( ra == ~0 )
    HWA_ERR("invalid access");
#endif

#if !defined HWA_NO_CHECK_USEFUL
  if ( mask == 0 )
    HWA_ERR("no bit to be changed?");
#endif

#if !defined HWA_NO_CHECK_LIMITS
  if ( value & (~mask) ) {
    HWA_ERR("value overflows mask");
  }
#endif

  /*  Verify that we do not try to set non-writeable bits
   */
  if ( (value & mask & rwm) != (value & mask) )
    HWA_ERR("bits not writeable.");

  volatile uint32_t *p = (volatile uint32_t *)ra ;

  /*  TODO: bit set/reset
   */
  uint32_t rm = rwm & ~mask & ~rfm ;

  if ( rm == 0 )
    /*
     *  Nothing to be read, just write the new value
     */
    *p = value ;
  else {
    /*
     *  Read-modify-write
     */
    uint32_t sm = mask & rwm & value ;	/* what has to be set	  */
    uint32_t cm = mask & rwm & (~value) ;	/* what has to be cleared */
    *p = (*p & ~cm) | sm ;
  }
}


HW_INLINE void _hwa_commit__r32 ( hwa_r32_t *r, uint32_t rwm, uint32_t rfm, _Bool commit )
{
  if ( !commit ) {
    r->ovalue = (r->ovalue & r->omask & ~r->mmask) | (r->mvalue & r->mmask) ;
    r->omask |= r->mmask ;
    r->mmask = 0 ;
    return ;
  }

  volatile uint32_t *p = (volatile uint32_t *)r->a ;

  uint32_t wm = rwm & r->mmask & ((r->ovalue ^ r->mvalue) | ~r->omask);

  if ( wm ) {
    uint32_t rm = ~r->mmask & ~rfm & ~r->omask & rwm ;

    if ( rm )
      r->ovalue = *p ;

    r->ovalue = ((r->ovalue & ~wm) | (r->mvalue & wm)) & ~rfm ;

    *p = r->ovalue | (rfm & r->mmask & r->mvalue) ;
  }

  r->omask |= r->mmask ;
  r->mmask = 0 ;
}


HW_INLINE uint32_t _hw_read__r32 ( intptr_t ra, uint8_t rbn, uint8_t rbp )
{
  uint32_t m = (1UL<<rbn)-1 ;
  volatile uint32_t *p = (volatile uint32_t *)ra ;
  return ((*p)>>rbp) & m ;
}

/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief HWA definitions that produce C code
 */

/**
 * @ingroup public
 * @brief Begin a HWA session. Allows the use of the `hwa_...(...)`
 * instructions.
 *
 * Create a HWA context to memorize what the `hwa_...(...)` instructions do.
 *
 * Nothing is written into the hardware until `hwa_commit()` is called.
 * @hideinitializer
 */
/*
 * Calls hwa_begin_all() that must be defined in hwa_<device>_2.h.
 */
#define hwa_begin()							\
  hwa_check_optimizations(0);						\
  hwa_t hwa_st ; hwa_t *hwa = &hwa_st ;					\
  _hwa_begin_all(hwa) ;							\
  uint8_t hwa_xcommit = 0 /* Will warn if hwa_commit() is not called */


/**
 * @ingroup public
 * @brief Begin a HWA session. Allows the use of the `hwa_...(...)`
 * instructions.
 *
 * Create a HWA context to memorize what the `hwa_...(...)` instructions do.
 *
 * The context is initialized with the values the registers have after a
 * system reset.
 *
 * Nothing is written into the hardware until `hwa_commit()` is called.
 * @hideinitializer
 */
#define hwa_begin_from_reset()						\
  hwa_check_optimizations(0);						\
  hwa_t hwa_st ; hwa_t *hwa = &hwa_st ;					\
  _hwa_begin_all(hwa) ;							\
  _hwa_init_all(hwa) ;							\
  hwa_nocommit() ;							\
  uint8_t hwa_xcommit = 0


/**
 * @ingroup public
 * @brief  Commit configuration to hardware.
 *
 * Solve the configuration stored into the HWA context, then do the required
 * hardware register writes.
 *
 * @hideinitializer
 */
/*
 * Calls `_hwa_commit_all()` that must be defined `in devices/hwa_HW_DEVICE_2.h`.
 */
#define hwa_commit()					\
  do {							\
    uint8_t foo __attribute__((unused)) = hwa_xcommit ;	\
    hwa->commit = 1 ;					\
    _hwa_commit_all(hwa);				\
  } while(0)

/**
 * @ingroup public
 * @brief  Same as hwa_commit() but do not write into hardware.
 *
 * This is used to put the HWA context in a known state before modifying it.
 *
 * @hideinitializer
 */
#define hwa_nocommit()				\
  do {						\
    hwa->commit = 0 ;				\
    _hwa_commit_all(hwa);			\
  } while(0)


/**
 * @ingroup public
 * @brief Read from an object (method)
 *
 * Syntax: `hw_read( object [,...] );`
 * @hideinitializer
 */
/*  Variable arguments list is required. E.g. `hw_read( hw_flash0, address )`
 */
#define hw_read(...)			HW_MTHD(hw_read, __VA_ARGS__,)

/**
 * @ingroup public
 * @brief Read multiple bytes from an object (method)
 *
 * Syntax: `hw_read_bytes( object [,...] );`
 * @hideinitializer
 */
#define hw_read_bytes(...)		HW_MTHD(hw_read_bytes, __VA_ARGS__,)

/**
 * @ingroup public
 * @brief Read one register of an object (method)
 *
 * Syntax: `hw_read_reg( object, register );`
 * @hideinitializer
 */
#define hw_read_reg(...)		HW_MTHD(hw_read, hw_reg(__VA_ARGS__))

/**
 * @ingroup public
 * @brief Read from an object with interrupts disabled (method)
 *
 * Syntax: `hw_atomic_read( object [,...] );`
 * @hideinitializer
 */
#define hw_atomic_read(...)		HW_MTHD(hw_atomic_read, __VA_ARGS__)


/**
 * @ingroup private
 * @brief Read one register of an object
 *
 * Syntax: `_hw_read( object, register );`
 * @hideinitializer
 */
#define _hw_read_reg(p,m)		_HW_SPEC(_hw_read, _hw_reg(p,m))


/**
 * @ingroup private
 * @brief Read one register of an object, knowing only the class and having a
 * pointer on the object in the context.
 *
 * Syntax: `_hw_read_creg( pointer, class, register );`
 * @hideinitializer
 */
#define _hw_read_creg(p,c,m)		_HW_SPEC(_hw_readp, _hw_creg(c,m), p)


/**
 * @ingroup public
 * @brief Write into an object (method)
 *
 * Syntax: `hw_write( object [,...], value );`
 * @hideinitializer
 */
/*  Variable arguments list is required. E.g. `hw/hwa_write( hw_flash0, address, value )`
 */
#define hw_write(...)			HW_MTHD(hw_write, __VA_ARGS__,)

/**
 * @ingroup public
 * @brief Write into an object (method)
 *
 * Syntax: `hwa_write( object [,...], value );`
 * @hideinitializer
 */
#define hwa_write(...)			HW_MTHD(hwa_write, __VA_ARGS__,)

/**
 * @ingroup public
 * @brief Write multiple bytes into an object (method)
 *
 * Syntax: `hw_write_bytes( object, ... );`
 * @hideinitializer
 */
#define hw_write_bytes(...)		HW_MTHD(hw_write_bytes, __VA_ARGS__,)

/**
 * @ingroup public
 * @brief Write into one register of an object (method)
 *
 * Syntax: `hw_write_reg( object, register, value );`
 * @hideinitializer
 */
#define hw_write_reg(p,m,v)		HW_MTHD(hw_write, hw_reg(p,m),v)

/**
 * @ingroup public
 * @brief Write into one register of an object (method)
 *
 * Syntax: `hwa_write( object, register, value );`
 * @hideinitializer
 */
#define hwa_write_reg(p,m,v)		HW_MTHD(hwa_write, hw_reg(p,m),v)

/**
 * @ingroup private
 * @brief Write one register of an object
 *
 * Syntax: `_hw_write_reg( object, register, value );`
 * @hideinitializer
 */
#define _hw_write_reg(p,m,v)		_HW_SPEC(_hw_write, hw_reg(p,m), v)

/**
 * @ingroup private
 * @brief Write one register of an object
 *
 * Syntax: `_hwa_write_reg( object, register, value );`
 * @hideinitializer
 */
#define _hwa_write_reg(p,m,v)		_HW_SPEC(_hwa_write, hw_reg(p,m), v)

/**
 * @ingroup private
 * @brief Write one register of an object, knowing only the class and having a
 * pointer on the object in the context.
 *
 * Syntax: `_hw_write_creg( pointer, class, register, value );`
 * @hideinitializer
 */
#define _hw_write_creg(p,c,m,v)		_HW_SPEC(_hw_writep, _hw_creg(c,m), p,v)

/**
 * @ingroup private
 * @brief Write one register of an object, knowing only the class and having a
 * pointer on the object in the context.
 *
 * Syntax: `_hwa_write_creg( pointer, class, register, value );`
 * @hideinitializer
 */
#define _hwa_write_creg(p,c,m,v)	_HW_SPEC(_hwa_writep, _hw_creg(c,m), p,v)


/**
 * @ingroup public
 * @brief Make an object execute one of its special commands (method)
 *
 * Syntax: `hw_command( object, command [,...] );`
 * @hideinitializer
 */
#define hw_command(...)			HW_MTHD(hw_command, __VA_ARGS__,)


/**
 * @ingroup public
 * @brief Configure an object (method)
 *
 * Syntax: `hw_config( object, ... );`
 * @hideinitializer
 */
#define hw_config(...)			HW_MTHD(hw_config, __VA_ARGS__,)

/**
 * @ingroup public
 * @brief Configure an object (method)
 *
 * Syntax: `hwa_config( object, ... );`
 * @hideinitializer
 */
#define hwa_config(...)			HW_MTHD(hwa_config, __VA_ARGS__,)

/**
 * @ingroup public
 * @brief Clear an object (method)
 *
 * Syntax: `hw_clear( object [,...] );`
 * @hideinitializer
 */
#define hw_clear(...)			HW_MTHD(hw_clear, __VA_ARGS__,)

/**
 * @ingroup public
 * @brief Clear an object (method)
 *
 * Syntax: `hwa_clear( object [,...] );`
 * @hideinitializer
 */
#define hwa_clear(...)			HW_MTHD(hwa_clear, __VA_ARGS__,)


/**
 * @ingroup public
 * @brief Reset an object (method)
 *
 * Syntax: `hw_reset( object [,...] );`
 * @hideinitializer
 */
#define hw_reset(...)			HW_MTHD(hw_reset, __VA_ARGS__,)

/**
 * @ingroup public
 * @brief Reset an object (method)
 *
 * Syntax: `hwa_reset( object [,...] );`
 * @hideinitializer
 */
#define hwa_reset(...)			HW_MTHD(hwa_reset, __VA_ARGS__,)


/**
 * @ingroup public
 * @brief Power an object on/off (method)
 *
 * Syntax: `hw_power( object, on | off );`
 * @hideinitializer
 */
#define hw_power(...)			HW_MTHD(hw_power, __VA_ARGS__,)

/**
 * @ingroup public
 * @brief Power an object on/off (method)
 *
 * Syntax: `hwa_power( object, on | off );`
 * @hideinitializer
 */
#define hwa_power(...)			HW_MTHD(hwa_power, __VA_ARGS__,)


/*	hw_lock_to_zero(...): clear something and keep it at 0 (mehod)
 */
#define hw_lock_to_zero(...)		HW_MTHD(hw_lock_to_zero, __VA_ARGS__)
#define hwa_lock_to_zero(...)		HW_MTHD(hwa_lock_to_zero, __VA_ARGS__)


/*	hw/hwa_release(...): release something (method)
 */
#define hw_release(...)			HW_MTHD(hw_release, __VA_ARGS__)
#define hwa_release(...)		HW_MTHD(hwa_release, __VA_ARGS__)


/**
 * @ingroup public
 * @brief Type of the status of an object (method)
 *
 * Syntax: `hw_stat_t( object );`
 * @hideinitializer
 */
#define hw_stat_t(...)			HW_MTHD(hw_stat_t, __VA_ARGS__,)


/**
 * @ingroup public
 * @brief Read the status of an object (method)
 *
 * Syntax: `hw_stat( object [,...] );`
 * @hideinitializer
 */
#define hw_stat(...)			HW_MTHD(hw_stat, __VA_ARGS__,)


/**
 * @ingroup public
 * @brief Toggle an object, probably an i/o pin (method)
 *
 * Syntax: `hw_toggle( object [,...] );`
 * @hideinitializer
 */
#define hw_toggle(...)			HW_MTHD(hw_toggle, __VA_ARGS__,)

/**
 * @ingroup public
 * @brief Toggle an object, probably an i/o pin (method)
 *
 * Syntax: `hwa_toggle( object [,...] );`
 * @hideinitializer
 */
#define hwa_toggle(...)			HW_MTHD(hwa_toggle, __VA_ARGS__,)


/**
 * @ingroup public
 * @brief Turn an object on/off (method)
 *
 * Syntax: `hw_turn( object [,...], on | off );`
 * @hideinitializer
 */
#define hw_turn(...)			HW_MTHD(hw_turn, __VA_ARGS__,)

/**
 * @ingroup public
 * @brief Turn an object on/off (method)
 *
 * Syntax: `hwa_turn( object [,...], on | off );`
 * @hideinitializer
 */
#define hwa_turn(...)			HW_MTHD(hwa_turn, __VA_ARGS__,)


/**
 * @ingroup public
 * @brief Trigger an object (method)
 *
 * Syntax: `hw_trigger( object );`
 * @hideinitializer
 */
#define hw_trigger(p)			HW_MTHD(hw_trigger, p,)

/**
 * @ingroup public
 * @brief Trigger an object (method)
 *
 * Syntax: `hwa_trigger( object );`
 * @hideinitializer
 */
#define hwa_trigger(p)			HW_MTHD(hwa_trigger, p,)


#define _hw_mthd_hw_read__m1		, _hw_read__m1
#define _hw_mthd_hw_read__m2		, _hw_read__m2
#define _hw_mthd_hw_write__m1		, _hw_write__m1
#define _hw_mthd_hw_write__m2		, _hw_write__m2
#define _hw_mthd_hwa_write__m1		, _hwa_write__m1
#define _hw_mthd_hwa_write__m1x		, _hwa_write__m1x
#define _hw_mthd_hwa_write__m2		, _hwa_write__m2

/**
 * @ingroup private
 * @brief Read one group of consecutive bits from one hardware register
 */
#define _hw_read__m1(p,a, r,rw,ra,rwm,rfm, rbn,rbp)	\
  _hw_read_r##rw(a+ra,rbn,rbp)

/**
 * @ingroup private
 * @brief Read two groups of consecutive bits from two hardware registers
 */
#define _hw_read__m2(p,a,					\
		     r1,rw1,ra1,rwm1,rfm1,rbn1,rbp1,vbp1,	\
		     r2,rw2,ra2,rwm2,rfm2,rbn2,rbp2,vbp2)	\
  ((_hw_read_r##rw1(a+ra1,rbn1,rbp1)<<vbp1) |			\
   (_hw_read_r##rw2(a+ra2,rbn2,rbp2)<<vbp2))

/* #define _hw_readp__m1(_0,_1,r,rw,ra,rwm,rfm, rbn,rbp, p)	\ */
/*   _hw_read_r##rw( p->r.a, rbn,rbp ) */


/**
 * @ingroup private
 * @brief Write something
 *
 * Syntax: `_hw_write( ..., value );`
 * @hideinitializer
 */
#define _hw_write(...)			_hw_write_2(__VA_ARGS__) /* Internal use */
#define _hw_write_2(x,...)		_hw_write_##x(__VA_ARGS__)

/**
 * @ingroup private
 * @brief Write one group of consecutive bits into one hardware register
 */
#define _hw_write__m1(cn,ca, rn,rw,ra,rwm,rfm, rbn,rbp, v)	\
  _hw_write_r##rw(ca+ra,rwm,rfm,rbn,rbp,v)

/**
 * @ingroup private
 * @brief Write two groups of consecutive bits into two hardware registers
 * @hideinitializer
 */
#define _hw_write__m2(cn,ca,						\
		      rn1,rw1,ra1,rwm1,rfm1,rbn1,rbp1,vbp1,		\
		      rn2,rw2,ra2,rwm2,rfm2,rbn2,rbp2,vbp2, v)		\
  do {									\
    if ( ra1==ra2 ) {							\
      HWA_ERR("That should not happen");				\
    }									\
    else {								\
      _hw_write_r##rw1(ca+ra1,rwm1,rfm1,rbn1,rbp1, (v>>vbp1)&((1<<rbn1)-1)); \
	_hw_write_r##rw2(ca+ra2,rwm2,rfm2,rbn2,rbp2, (v>>vbp2)&((1<<rbn2)-1)); \
    }									\
  } while(0)

#define _hw_writep__m1(_0,_1,r,rw,ra,rwm,rfm, bn,bp, p,v)	\
  _hw_write_r##rw( p->r.a, rwm,rfm, bn,bp,v)


#define _hwa_write(...)			_hwa_write_2(__VA_ARGS__) /* Internal use */
#define _hwa_write_2(x,...)		_hwa_write_##x(x,__VA_ARGS__)

#define _hwa_write__m1(p,a, r,rw,ra,rwm,rfm, bn,bp, v)	\
  _hwa_write_r##rw( &hwa->p.r, rwm,rfm, bn,bp, v )
/* #define _hwa_write__m1(p,a, r,rw,ra,rwm,rfm, bn,bp, v,...)	\ */
/*   HW_TX(_hwa_write_r##rw( &hwa->p.r, rwm,rfm, bn,bp, v ), __VA_ARGS__) */

//_hwa_write__m1x(hw_core0,0, gimsk,8,0x5B,0x70,0x00, 1,5, 1);
#define _hwa_write__m1x(p,a, r,rw,ra,rwm,rfm, bn,bp, v)	\
  _hwa_write_r##rw( &hwa->p.r, rwm,rfm, bn,bp, v )


#define _hwa_write__m2(cn,ca,					\
			 r1,rw1,ra1,rwm1,rfm1,rbn1,rbp1,vbp1,		\
			 r2,rw2,ra2,rwm2,rfm2,rbn2,rbp2,vbp2, v)	\
  do {									\
    _hwa_write_r##rw1(&hwa->cn.r1, rwm1,rfm1, rbn1,rbp1, ((v)>>(vbp1))&((1U<<rbn1)-1)); \
      _hwa_write_r##rw2(&hwa->cn.r2, rwm2,rfm2, rbn2,rbp2, ((v)>>(vbp2))&((1U<<rbn2)-1)); \
  } while(0)


/*  Functions used to write into the HWA context using a pointer instead of a
 *  name (creg)
 */
#define _hwa_writep__m1(_0,_1,r,rw,ra,rwm,rfm, bn,bp, p,v)	\
  _hwa_write_r##rw( &((p)->r), rwm,rfm, bn,bp,v)

#define _hwa_writep__m1x(p,a,r,rw,ra,rwm,rfm, bn,bp, p0,v)	/* result of an _ext */	\
  _hwa_write_r##rw( &hwa->p.r, rwm,rfm, bn,bp,v)

#define _hwa_writep__m2(_0,_1,						\
		       r1,rw1,ra1,rwm1,rfm1,rbn1,rbp1,vbp1,		\
		       r2,rw2,ra2,rwm2,rfm2,rbn2,rbp2,vbp2, p,v )	\
  do { _hwa_write_r##rw1(&((p)->r1), rwm1,rfm1, rbn1,rbp1, ((v)>>(vbp1))&((1U<<rbn1)-1)); \
      _hwa_write_r##rw2(&((p)->r2), rwm2,rfm2, rbn2,rbp2, ((v)>>(vbp2))&((1U<<rbn2)-1)); } while(0)


/*  Write some bits of a memory definition
 *
 *	_hw_write_bits( mdef, bn, bp, v );
 */
#define _hw_write_bits(...)		_hw_wrb(__VA_ARGS__)
#define _hw_wrb(t,...)			_hw_wrb_##t(__VA_ARGS__)
#define _hw_wrb__m1(p,a,r,rw,ra,rwm,rfm,rbn,rbp, bn,bp, v)\
  _hw_write_r##rw(a+ra,rwm,rfm,bn,bp,v)

#define _hwa_begin(p)			_hwa_begin_2(p, _##p)
#define _hwa_begin_2(...)		_hwa_begin_3(__VA_ARGS__)
#define _hwa_begin_3(p,c,i,a)		__hwa_begin_##c(&hwa->p,a)


#define _hwa_init(n)			_hwa_init_2(n, _##n)
#define _hwa_init_2(...)		_hwa_init_3(__VA_ARGS__)
#define _hwa_init_3(n,c,i,a)		__hwa_init_##c(&hwa->n)


#define _hwa_solve(n)			_hwa_solve_2(n, _##n)
#define _hwa_solve_2(...)		_hwa_solve_3(__VA_ARGS__)
#define _hwa_solve_3(n,c,i,a)		__hwa_solve_##c(hwa, &hwa->n)


#define _hwa_commit(n)			_hwa_commit_2(n, _##n)
#define _hwa_commit_2(...)		_hwa_commit_3(__VA_ARGS__)
#define _hwa_commit_3(n,c,i,a)		__hwa_commit_##c(hwa,&hwa->n)


#define _hwa_begin_reg_p(p,a,c,r)		_hwa_berp_2(p,a,c,r,_hw_##c##_##r)
#define _hwa_berp_2(...)			_hwa_berp_3(__VA_ARGS__)
#define _hwa_berp_3(p,a,c,r, rt,rw,ra,rwm,rfm)	_hwa_begin_r##rw(&((p)->r),a+ra)


HW_INLINE void _hwa_begin_r8 ( hwa_r8_t *r, intptr_t a )
{
  r->a		= a ;
  r->mmask	= 0 ;
  r->mvalue	= 0 ;
  r->omask	= 0 ;
  r->ovalue	= 0 ;
}

HW_INLINE void _hwa_begin_r16 ( hwa_r16_t *r, intptr_t a )
{
  r->a		= a ;
  r->mmask	= 0 ;
  r->mvalue	= 0 ;
  r->omask	= 0 ;
  r->ovalue	= 0 ;
}


/** 
 * @ingroup private
 * @brief  Initialize a HWA register to a specific value (usually the reset value).
 * @hideinitializer
 */
HW_INLINE void _hwa_set_r8 ( hwa_r8_t *r, uint8_t v )
{
  if ( r->mmask )
    HWA_ERR("commit required before resetting.");

  r->mmask = 0xFF ;
  r->mvalue = v ;
}

HW_INLINE void _hwa_set_r16 ( hwa_r16_t *r, uint16_t v )
{
  if ( r->mmask )
    HWA_ERR("commit required before resetting.");

  r->mmask = 0xFFFF ;
  r->mvalue = v ;
}


/** 
 * @ingroup private
 * @brief  Write into a HWA register
 *
 * Write bits according to `bn`/`bp`. `mmask` is set even if the value is not
 * modified. `hwa_commit()` will check if the register has effectively been
 * modified.
 *
 * @param r   pointer to the register structure
 * @param rwm write mask
 * @param rfm interrupt flags mask
 * @param bn  number of consecutive bits
 * @param bp  position of least significant bit
 * @param v   value
 */
HW_INLINE void _hwa_write_r8 ( hwa_r8_t *r, 
			       uint8_t rwm, uint8_t rfm,
			       uint8_t bn, uint8_t bp, uint8_t v )
{
  if (bn == 0)
    HWA_ERR("no bit to be changed?");

  if (v > (1U<<bn)-1)
    HWA_ERR("value too high for number of bits.");

  uint8_t sm = ((1U<<bn)-1) << bp ;	/* shifted mask  */

  //  *((volatile uint8_t*)0) = sm ;

  uint8_t sv = v << bp ;		/* shifted value */

  //  *((volatile uint8_t*)0) = sv ;

  if ((rwm & sm) != sm)
    HWA_ERR("bits not writeable.");

  if ((r->mmask & sm) != 0 && (r->mvalue & sm) != sv)
    HWA_ERR("committing is required before setting a new value.");

  if ( sm & rfm )
    if ( v == 0 )
      HWA_ERR("flag bit can only be cleared by writing 1 into it.");

  r->mmask |= sm ;
  r->mvalue = (r->mvalue & ~sm) | (sm & sv) ;
}

HW_INLINE void _hwa_write_r16 ( hwa_r16_t *r,
				uint16_t rwm, uint16_t rfm,
				uint8_t bn, uint8_t bp, uint16_t v )
{
  if (bn == 0)
    HWA_ERR("no bit to be changed?");

  if (v > (1U<<bn)-1)
    HWA_ERR("value too high for bits number.");

  uint16_t sm = ((1U<<bn)-1) << bp ;	/* shifted mask  */
  uint16_t sv = v << bp ;		/* shifted value */

  if ((rwm & sm) != sm)
    HWA_ERR("bits not writeable.");

  if ((r->mmask & sm) != 0 && (r->mvalue & sm) != sv)
    HWA_ERR("commit required before setting a new value.");

  if ( sm & rfm )
    if ( v == 0 )
      HWA_ERR("flag bit can only be cleared by writing 1 into it.");

  r->mmask |= sm ;
  r->mvalue = (r->mvalue & ~sm) | (sm & sv) ;
}


/** 
 * @ingroup private
 * @brief	Initialize an HWA register
 * @hideinitializer
 */
#define _hwa_begin_reg(...)			_hwa_begin_reg_2(__VA_ARGS__)
#define _hwa_begin_reg_2(n,c,i,a, r)		_hwa_begin_reg_3(n,a,r, _hw_##c##_##r)
#define _hwa_begin_reg_3(...)			_hwa_begin_reg_4(__VA_ARGS__)
#define _hwa_begin_reg_4(n,a,r, rt,rw,ra, ... )	_hwa_begin_r##rw( &hwa->n.r, a+ra )


/**
 * @ingroup private
 * @brief Commit one register to the hardware
 * @hideinitializer
 */
#define _hwa_commit_reg(...)			_hwa_commit_reg_2(__VA_ARGS__)
#define _hwa_commit_reg_2(n,c,i,a, r, co)	_hwa_commit_reg_3(n,a,r, _hw_##c##_##r, co)
#define _hwa_commit_reg_3(...)			_hwa_commit_reg_4(__VA_ARGS__)
#define _hwa_commit_reg_4(n,a,r, rt,rw,ra,rwm,rfm, co )	\
  _hwa_commit_r##rw( &hwa->n.r,rwm,rfm, co )


/**
 * @ingroup private
 * @brief Trigger an error if optimizers failed to remove this code
 * @hideinitializer
 */
HW_INLINE void hwa_check_optimizations ( uint8_t x )
{
  if (x) { HWA_ERR("you may have forgotten to turn optimizations on."); }
}


/*	Include device-specific declarations
 */
#ifndef DOXYGEN
#  include HW_QUOTE(../devices/HW_G2(HW_DEVICE,2).h)
#endif
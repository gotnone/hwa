
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief 16-bit counter
 */

/**
 * @page atmelavr_ctd
 * @section atmelavr_ctd_act Actions
 *
 * <br>
 * `configure`:
 *
 * __Note__ If the optionnal argument `overflow` is not stated, an acceptable value
 * will be selected according to the configuration of the compare units found in
 * the HWA context. If `overflow` is stated, the validity of its value will be
 * verified.
 *
 * @code
 * hwa( configure, counter0,
 *
 *	//  How the counter is clocked
 *	//
 *	clock,	     none			// No clock, the counter is stopped
 *		   | ioclk [/ 8|64|256|1024]	// I/O clock
 *		   | external_rising		// External input, rising edge
 *		   | external_falling,		// External input, falling edge
 *
 *	//  How does this counter count
 *	//
 *	direction,   up_loop			// Count up and loop
 *		   | updown_loop,		// Count up and down alternately
 *
 *	//  Class _ctd counters all count from 0
 *	//
 *    [ bottom,	     0, ]
 *
 *	//  The maximum value the counter reaches (the default is `max`)
 *	//
 *    [ top,	     0xFF | 0x00FF | 255	// Hardware fixed value 0x00FF
 *		   | 0x1FF | 0x01FF | 511	// Hardware fixed value 0x01FF
 *		   | 0x3FF | 0x03FF | 1023	// Hardware fixed value 0x03FF
 *		   | 0xFFFF | 65535		// Hardware fixed value 0xFFFF
 *		   | max			// Hardware fixed value 0xFFFF
 *		   | compare0			// Value stored in the compare0 unit
 *		   | capture0, ]		// Value stored in the capture0 unit
 *
 *	//  When the overflow flag is set
 *	//
 *    [ overflow,    after_bottom		   // When the counter resets to bottom
 *		   | after_top			   // When the counter reaches the top value
 *		   | after_max ]		   // When the counter reaches its max value
 *    );
 * @endcode
 */
#define hwa_configure__ctd		, _hwa_cfctd

/*  Mandatory argument `clock`
 *
 *    Add 2 void arguments to the end of the list so that there are always
 *    3 arguments following the last non-void argument.
 */
#define _hwa_cfctd(o,a,k,...)					\
  do { HW_Y(_hwa_cfctd_kclock_,_hw_is_clock_##k)(o,k,__VA_ARGS__,,) } while(0)

#define _hwa_cfctd_kclock_0(o,k,...)				\
  HW_E_VL(k,clock)

#define _hwa_cfctd_kclock_1(o,k,v,...)				\
  HW_Y(_hwa_cfctd_vclock_,_hw_c1clk_##v)(o,v,__VA_ARGS__)

#define _hwa_cfctd_vclock_0(o,v,...)					\
  HW_E_AVL(clock, v, none | ioclk [/ 8|64|256|1024] | external_falling | external_rising)

#define _hwa_cfctd_vclock_1(o,v,k,...)					\
  hwa->o.config.clock = HW_VF(_hw_c1clk_##v);				\
  HW_Y(_hwa_cfctd_kmode_,_hw_is_direction_##k)(o,k,__VA_ARGS__)


/*  Optionnal argument `direction`
 */
#define _hw_ctd_direction_up_loop	, 1
#define _hw_ctd_direction_updown_loop	, 2

#define _hwa_cfctd_kmode_0(o,k,...)					\
  HW_E_VL(k,direction)

#define _hwa_cfctd_kmode_1(o,k,v,...)					\
  HW_Y(_hwa_cfctd_vmode_,_hw_ctd_direction_##v)(o,v,__VA_ARGS__)

#define _hwa_cfctd_vmode_0(o,v,...)					\
  HW_E_AVL(direction, v, up_loop | updown_loop)

#define _hwa_cfctd_vmode_1(o,v,k,...)					\
  hwa->o.config.direction = HW_A1(_hw_ctd_direction_##v);			\
  HW_Y(_hwa_cfctd_kbottom_,_hw_is_bottom_##k)(o,k,__VA_ARGS__)


/*  Optionnal argument `bottom`
 */
#define _hwa_cfctd_kbottom_1(o,k,v,...)		\
  HW_G2(_hwa_cfctd_vbottom,HW_IS(0,v))(o,v,__VA_ARGS__)

#define _hwa_cfctd_vbottom_0(o,v,...)	\
  HW_E_AVL(bottom, v, `0`)

#define _hwa_cfctd_vbottom_1(o,v,k,...)	\
  HW_Y(_hwa_cfctd_ktop_,_hw_is_top_##k)(o,k,__VA_ARGS__)

#define _hwa_cfctd_kbottom_0(o,k,...)				\
  HW_Y(_hwa_cfctd_ktop_,_hw_is_top_##k)(o,k,__VA_ARGS__)

/*  Optionnal argument `top`
 */
#define _hw_ctd_top_fixed_0xFF		, 1
#define _hw_ctd_top_fixed_0x1FF	, 2
#define _hw_ctd_top_fixed_0x3FF	, 3
#define _hw_ctd_top_fixed_0xFFFF	, 4
#define _hw_ctd_top_max		, 4
#define _hw_ctd_top_capture0		, 5
#define _hw_ctd_top_compare0		, 6

#define _hwa_cfctd_ktop_1(o,k,v,...)					\
  HW_Y(_hwa_cfctd_vtop_,_hw_ctd_top_##v)(o,v,__VA_ARGS__)

#define _hwa_cfctd_vtop_0(o,v,...)					\
  HW_E_AVL(top, v, fixed_0xFF | fixed_0x1FF | fixed_0x3FF | fixed_0xFFFF | max | capture0 | compare0)

#define _hwa_cfctd_vtop_1(o,v,k,...)					\
  hwa->o.config.top = HW_A1(_hw_ctd_top_##v);				\
  HW_Y(_hwa_cfctd_koverflow_,_hw_is_overflow_##k)(o,k,__VA_ARGS__)

#define _hwa_cfctd_ktop_0(o,k,...)					\
  HW_Y(_hwa_cfctd_koverflow_,_hw_is_overflow_##k)(o,k,__VA_ARGS__)

/*  Optionnal argument `overflow`
 */
#define _hw_ctd_overflow_after_bottom	, 0
#define _hw_ctd_overflow_after_top	, 1
#define _hw_ctd_overflow_after_max	, 2

#define _hwa_cfctd_koverflow_1(o,k,v,...)				\
  HW_Y(_hwa_cfctd_voverflow_,_hw_ctd_overflow_##v)(o,v,__VA_ARGS__)

#define _hwa_cfctd_voverflow_0(o,v,...)				\
  HW_E_OAVL(overflow, v, after_bottom | after_top | after_max)

#define _hwa_cfctd_voverflow_1(o,v,...)				\
  hwa->o.config.overflow = HW_A1(_hw_ctd_overflow_##v) HW_EOL(__VA_ARGS__)

#define _hwa_cfctd_koverflow_0(o,...)	\
  HW_EOL(__VA_ARGS__)


/**
 * @brief Solve and check the configuration of the counter and its compare units.
 *
 * Writing code for a HW_INLINE function is more comfortable than for a
 * function-like macro but functions can not use object names. So, this
 * function-like macro expands to the HW_INLINE function _hwa_solve_ctd() which
 * uses pointers on objects in the HWA context to compute values to be written
 * into the registers, stores them into the `solve` structure and returns an
 * error code that is processed by _hwa_solve__ctd() which knows the names of
 * the objects and then can put computed registers values into the context, even
 * in the case of external register access, and display accurate error messages.
 */
#define _hwa_solve__ctd( o, a )						\
  do {									\
    uint8_t r = _hwa_solve_ctd( &hwa->o, &hwa->o.compare0, &hwa->o.compare1, &hwa->o.capture0 ); \
    if ( r == 0 ) {							\
      /*								\
       *  Write solved registers					\
       */								\
      if ( hwa->o.solved.cs != 0xFF ) _hwa_write( o, cs, hwa->o.solved.cs ); \
      if ( hwa->o.solved.wgm != 0xFF ) _hwa_write( o, wgm, hwa->o.solved.wgm ); \
      if ( hwa->o.compare0.solved.com != 0xFF ) _hwa_write( o, com0, hwa->o.compare0.solved.com ); \
      if ( hwa->o.compare1.solved.com != 0xFF ) _hwa_write( o, com1, hwa->o.compare1.solved.com ); \
      if ( hwa->o.capture0.solved.acic != 0xFF ) _hwa_write( o, acic, hwa->o.capture0.solved.acic ); \
      if ( hwa->o.capture0.solved.ices != 0xFF ) _hwa_write( o, ices, hwa->o.capture0.solved.ices ); \
      if ( hwa->o.capture0.solved.icnc != 0xFF ) _hwa_write( o, icnc, hwa->o.capture0.solved.icnc ); \
      /*								\
       *  Configure used compare outputs as i/o outputs			\
       */								\
      if ( hwa->o.compare0.config.output != 0xFF			\
	   && hwa->o.compare0.config.output != HW_A1(_hw_occ_output_disconnected) ) \
	_hwa( configure, hw_##o##_compare0_pin, mode, digital_output );	\
      if ( hwa->o.compare1.config.output != 0xFF			\
	   && hwa->o.compare1.config.output != HW_A1(_hw_occ_output_disconnected) ) \
	_hwa( configure, hw_##o##_compare1_pin, mode, digital_output );	\
    }									\
    else if ( r == 1 )							\
      HWA_ERR("`update` must be the same for both compare units of `" #o "`."); \
    else if ( r == 2 )							\
      HWA_ERR("WGM value could not be solved for `" #o "`.");		\
    else if ( r == 3 )							\
      HWA_ERR("configuration of `" #o "` is required.");		\
    else if ( r == 4 )							\
      HWA_ERR("`mode` of compare0 can be "				\
	      "'disconnected', 'toggle_after_match', 'clear_after_match', or " \
	      "'set_after_match'.");					\
    else if ( r == 5 )							\
      HWA_ERR("`mode` of compare0 can be "				\
	      "'disconnected', 'set_at_bottom_clear_after_match', or "	\
	      "'clear_at_bottom_set_after_match'.");			\
    else if ( r == 6 )							\
      HWA_ERR("`mode` of compare0 can be "				\
	      "'disconnected', 'toggle_after_match', "			\
	      "'set_at_bottom_clear_after_match', or "			\
	      "'clear_at_bottom_set_after_match'.");			\
    else if ( r == 7 )							\
      HWA_ERR("`mode` of compare0 can be "				\
	      "'disconnected', 'clear_after_match_up_set_after_match_down', " \
	      "or 'set_after_match_up_clear_after_match_down'.");	\
    else if ( r == 8 )							\
      HWA_ERR("`mode` of compare0 can be "				\
	      "'disconnected', 'toggle_after_match', "			\
	      "'clear_after_match_up_set_after_match_down', "		\
	      "or 'set_after_match_up_clear_after_match_down'.");	\
    else if ( r == 9 )							\
      HWA_ERR("`mode` of compare1 can be "				\
	      "'disconnected', 'toggle_after_match', 'clear_after_match', or " \
	      "'set_after_match'.");					\
    else if ( r == 10 )							\
      HWA_ERR("`mode` of compare1 can be "				\
	      "'disconnected', 'set_at_bottom_clear_after_match', or "	\
	      "'clear_at_bottom_set_after_match'.");			\
    else if ( r == 11 )							\
      HWA_ERR("`mode` of compare1 can be "				\
	      "'disconnected', 'clear_after_match_up_set_after_match_down', " \
	      "or 'set_after_match_up_clear_after_match_down'.");	\
    else if ( r == 12 )							\
      HWA_ERR("for `" #o "`, `optionnal parameter 'update' must be 'immediately'."); \
    else if ( r == 13 )							\
      HWA_ERR("for `" #o "`, `optionnal parameter 'update' must be 'after_bottom'."); \
    else if ( r == 14 )							\
      HWA_ERR("for `" #o "`, `optionnal parameter 'update' must be 'after_top'."); \
    else if ( r == 15 )							\
      HWA_ERR("for `" #o "`, `optionnal parameter 'overflow' must be 'after_top'."); \
    else if ( r == 16 )							\
      HWA_ERR("for `" #o "`, `optionnal parameter 'overflow' must be 'after_bottom'."); \
    else if ( r == 17 )							\
      HWA_ERR("for `" #o "`, `optionnal parameter 'overflow' must be 'after_max'."); \
    else								\
      HWA_ERR("for `" #o "`, `unknown error code'.");			\
  } while(0)

/*	Solve the configuration of the counter with its compare and capture units
 */
//HW_INLINE void _hwa_solve__ctd ( hwa_t *hwa __attribute__((unused)), hwa_ctd_t *p )
HW_INLINE uint8_t _hwa_solve_ctd ( hwa_ctd_t *c, hwa_occ_t *compare0,
				   hwa_occ_t *compare1, hwa_ica_t *capture0 )
{
  c->solved.cs	 = 0xFF ;
  c->solved.wgm	 = 0xFF ;
  compare0->solved.com = 0xFF ;
  compare1->solved.com = 0xFF ;
  capture0->solved.acic = 0xFF ;
  capture0->solved.ices = 0xFF ;
  capture0->solved.icnc = 0xFF ;

  if ( c->config.clock == 0xFF )
    return 0 ;

  /*  Clock setting
   */
  c->solved.cs = c->config.clock ;

  /*  Default config for top
   */
  if ( c->config.top == 0xFF )
    c->config.top = HW_A1(_hw_ctd_top_max);

  /*  Default config for overflow
   */
  uint8_t overflow = c->config.overflow ;
  if ( overflow == 0xFF && c->config.top == HW_A1(_hw_ctd_top_compare0) ) {
    if ( c->config.direction == HW_A1(_hw_ctd_direction_up_loop) )
      overflow = HW_A1(_hw_ctd_overflow_after_top);
    else /* if ( c->config.direction == HW_A1(_hw_ctd_direction_up_loop) ) */
      overflow = HW_A1(_hw_ctd_overflow_after_bottom);
  }

  /*  Compare update setting
   */
  uint8_t compare_update = 0xFF ;
  if ( compare0->config.update != 0xFF && compare1->config.update != 0xFF
       && compare0->config.update != compare1->config.update )
    return 1 ; // HWA_ERR("optionnal parameter 'update' of class _ctd counter must be'immediately'.");
  compare_update = compare0->config.update ;

  /* Mode WGM	Operation		      COUNTMODE	   TOP	   OCR	OVF  OCF  ICF  OCA		  OCB
   *								   TOP
   *								   UPD
   *
   *   0  0000	Normal			      LOOP_UP	   0xFFFF  IMM	MAX	       CoM,SoM,ToM	  CoM,SoM,ToM
   *   4  0100	CTC (Clear Timer on Match)    LOOP_UP	   ocra	   IMM	MAX  TOP			  CoM,SoM,ToM
   *  12  1100	CTC (Clear Timer on Match)    LOOP_UP	   icr	   IMM	MAX	  TOP  CoM,SoM,ToM	  CoM,SoM,ToM
   *
   *   5  0101	Fast PWM, 8-bit		      LOOP_UP	   0xFF	   TOP	TOP	       SaBCoM,CaBSoM	  SaBCoM,CaBSoM
   *   6  0110	Fast PWM, 9-bit		      LOOP_UP	   0x1FF   TOP	TOP	       SaBCoM,CaBSoM	  SaBCoM,CaBSoM
   *   7  0111	Fast PWM, 10-bit	      LOOP_UP	   0x3FF   TOP	TOP	       SaBCoM,CaBSoM	  SaBCoM,CaBSoM
   *  14  1110	Fast PWM		      LOOP_UP	   icr	   TOP	TOP	  TOP  ToM,SaBCoM,CaBSoM  SaBCoM,CaBSoM
   *  15  1111	Fast PWM		      LOOP_UP	   ocra	   TOP	TOP  TOP			  SaBCoM,CaBSoM
   *
   *   1  0001	PWM, Phase Correct, 8-bit     LOOP_UPDOWN  0xFF	   TOP	BOT	       CMUSMD,SMUCMD	  CMUSMD,SMUCMD
   *   2  0010	PWM, Phase Correct, 9-bit     LOOP_UPDOWN  0x1FF   TOP	BOT	       CMUSMD,SMUCMD	  CMUSMD,SMUCMD
   *   3  0011	PWM, Phase Correct, 10-bit    LOOP_UPDOWN  0x3FF   TOP	BOT	       CMUSMD,SMUCMD	  CMUSMD,SMUCMD
   *  10  1010	PWM, Phase Correct	      LOOP_UPDOWN  icr	   TOP	BOT	  TOP  ToM,CMUSMD,SMUCMD  CMUSMD,SMUCMD
   *  11  1011	PWM, Phase Correct	      LOOP_UPDOWN  ocra	   TOP	BOT  TOP			  CMUSMD,SMUCMD
   *
   *   8  1000	PWM, Phase & Freq. Correct    LOOP_UPDOWN  icr	   BOT	BOT	  TOP  ToM,CMUSMD,SMUCMD  CMUSMD,SMUCMD
   *   9  1001	PWM, Phase & Freq. Correct    LOOP_UPDOWN  ocra	   BOT	BOT  TOP			  CMUSMD,SMUCMD
   *
   *  13  1101	*Reserved*
   */

  /*	Determine WGM
   */
  uint8_t wgm = 0xFF ;
  if ( c->config.direction == HW_A1(_hw_ctd_direction_up_loop)
       && c->config.top == HW_A1(_hw_ctd_top_compare0)
       && overflow == HW_A1(_hw_ctd_overflow_after_top) )
    wgm = 15 ;
  else	  
    if ( c->config.direction == HW_A1(_hw_ctd_direction_up_loop) ) {
      if ( c->config.top == HW_A1(_hw_ctd_top_fixed_0xFFFF) )
	wgm = 0 ;
      else if (c->config.top == HW_A1(_hw_ctd_top_fixed_0xFF) )
	wgm = 5 ;
      else if (c->config.top == HW_A1(_hw_ctd_top_fixed_0x1FF) )
	wgm = 6 ;
      else if (c->config.top == HW_A1(_hw_ctd_top_fixed_0x3FF) )
	wgm = 7 ;
      else if (c->config.top == HW_A1(_hw_ctd_top_compare0) ) {
	if ( compare_update == HW_A1(_hw_occ_update_immediately)
	     || overflow == HW_A1(_hw_ctd_overflow_after_top)
	     || compare1->config.output == HW_A1(_hw_occ_output_set_at_bottom_clear_after_match)
	     || compare1->config.output == HW_A1(_hw_occ_output_clear_at_bottom_set_after_match))
	  wgm = 15 ;
	else 
	  wgm = 4 ;
      }
      else /* if (c->top == HW_A1(_hw_ctd_top_capture0) ) */ {
	if (compare_update == HW_A1(_hw_occ_update_after_top)
	    || overflow == HW_A1(_hw_ctd_overflow_after_top)
	    || compare0->config.output == HW_A1(_hw_occ_output_toggle_after_match)
	    || compare0->config.output == HW_A1(_hw_occ_output_set_at_bottom_clear_after_match)
	    || compare0->config.output == HW_A1(_hw_occ_output_clear_at_bottom_set_after_match)
	    || compare1->config.output == HW_A1(_hw_occ_output_set_at_bottom_clear_after_match)
	    || compare1->config.output == HW_A1(_hw_occ_output_clear_at_bottom_set_after_match))
	  wgm = 14 ;
	else 
	  wgm = 12 ;
      }
    }
    else {
      if (c->config.top == HW_A1(_hw_ctd_top_fixed_0xFF) )
	wgm = 1 ;
      else if (c->config.top == HW_A1(_hw_ctd_top_fixed_0x1FF) )
	wgm = 2 ;
      else if (c->config.top == HW_A1(_hw_ctd_top_fixed_0x3FF) )
	wgm = 3 ;
      else if (c->config.top == HW_A1(_hw_ctd_top_compare0) ) {
	if (compare_update == HW_A1(_hw_occ_update_after_bottom)
	    || compare1->config.output == HW_A1(_hw_occ_output_clear_after_match_up_set_after_match_down)
	    || compare1->config.output == HW_A1(_hw_occ_output_set_after_match_up_clear_after_match_down))
	  wgm = 9 ;
	else 
	  wgm = 11 ;
      }
      else /* if (c->top == HW_A1(_hw_ctd_top_capture0) ) */ {
	if (compare_update == HW_A1(_hw_occ_update_after_bottom)
	    || compare0->config.output == HW_A1(_hw_occ_output_toggle_after_match)
	    || compare0->config.output == HW_A1(_hw_occ_output_clear_after_match_up_set_after_match_down)
	    || compare0->config.output == HW_A1(_hw_occ_output_set_after_match_up_clear_after_match_down)
	    || compare1->config.output == HW_A1(_hw_occ_output_clear_after_match_up_set_after_match_down)
	    || compare1->config.output == HW_A1(_hw_occ_output_set_after_match_up_clear_after_match_down))
	  wgm = 8 ;
	else 
	  wgm = 10 ;
      }
    }
      
  if ( wgm == 0xFF )
    return 2 ; // HWA_ERR("WGM value could not be solved for _ctd class counter.");
  c->solved.wgm = wgm ;

  /*	Solve the configuration of compare output A
   */
  if ( compare0->config.output != 0xFF ) {

    uint8_t	mode = 0xFF ;

    if ( compare0->config.output == HW_A1(_hw_occ_output_disconnected) )
      mode = 0 ;
    else if ( compare0->config.output == HW_A1(_hw_occ_output_toggle_after_match) )
      mode = 1 ;
    else if ( compare0->config.output == HW_A1(_hw_occ_output_clear_after_match)
	      || compare0->config.output == HW_A1(_hw_occ_output_set_at_bottom_clear_after_match)
	      || compare0->config.output == HW_A1(_hw_occ_output_clear_after_match_up_set_after_match_down) )
      mode = 2 ;
    else
      mode = 3 ;

    compare0->solved.com = mode ;
  }

  /*  Solve the configuration of compare output B
   */
  if ( compare1->config.output != 0xFF ) {

    uint8_t	mode = 0xFF ;

    if ( compare1->config.output == HW_A1(_hw_occ_output_disconnected) )
      mode = 0 ;
    else if ( compare1->config.output == HW_A1(_hw_occ_output_toggle_after_match) )
      mode = 1 ;
    else if ( compare1->config.output == HW_A1(_hw_occ_output_clear_after_match)
	      || compare1->config.output == HW_A1(_hw_occ_output_set_at_bottom_clear_after_match)
	      || compare1->config.output == HW_A1(_hw_occ_output_clear_after_match_up_set_after_match_down) )
      mode = 2 ;
    else
      mode = 3 ;

    compare1->solved.com = mode ;
  }

  /*	Solve the configuration of the capture input
   */
  //  if ( capture0->config.input != 0xFF ) {
  if ( capture0->config.input != 0xFF )
    capture0->solved.acic = capture0->config.input-1 ;
  if ( capture0->config.edge != 0xFF )
    capture0->solved.ices = capture0->config.edge-1 ;
  if ( capture0->config.filter != 0xFF )
    capture0->solved.icnc = capture0->config.filter ;
  //}


  /*	Check the validity of the configuration
   */
  if ( c->config.direction != 0xFF || compare0->config.output != 0xFF || compare1->config.output != 0xFF ) {

    if ( c->config.direction == 0xFF )
      return 3 ; // HWA_ERR("configuration of counter is required.");

    /*	Check compare output A
     */
    if ( compare0->config.output != 0xFF ) {
      if ( wgm==0 || wgm==12 ) {
	if ( compare0->config.output != HW_A1(_hw_occ_output_disconnected)
	     && compare0->config.output != HW_A1(_hw_occ_output_clear_after_match)
	     && compare0->config.output != HW_A1(_hw_occ_output_set_after_match)
	     && compare0->config.output != HW_A1(_hw_occ_output_toggle_after_match) )
	  return 4 ;
	/* HWA_ERR("compare output A of class _ctd counter mode must be " */
	/*	  "'disconnected', 'clear_after_match', 'set_after_match', or " */
	/*	  "'toggle_after_match'."); */
      }
      else if ( wgm==5 || wgm==6 || wgm==7 ) {
	if ( compare0->config.output != HW_A1(_hw_occ_output_disconnected)
	     && compare0->config.output != HW_A1(_hw_occ_output_set_at_bottom_clear_after_match)
	     && compare0->config.output != HW_A1(_hw_occ_output_clear_at_bottom_set_after_match) )
	  return 5 ;
	  /* HWA_ERR("compare output A of class _ctd counter mode must be " */
	  /*	  "'disconnected', 'set_at_bottom_clear_after_match', or " */
	  /*	  "'clear_at_bottom_set_after_match'."); */
      }
      else if ( wgm==14 ) {
	if ( compare0->config.output != HW_A1(_hw_occ_output_disconnected)
	     && compare0->config.output != HW_A1(_hw_occ_output_toggle_after_match)
	     && compare0->config.output != HW_A1(_hw_occ_output_set_at_bottom_clear_after_match)
	     && compare0->config.output != HW_A1(_hw_occ_output_clear_at_bottom_set_after_match) )
	  return 6 ;
	  /* HWA_ERR("compare output A of class _ctd counter mode must be " */
	  /*	  "'disconnected', 'toggle_after_match', " */
	  /*	  "'set_at_bottom_clear_after_match', or " */
	  /*	  "'clear_at_bottom_set_after_match'."); */
      }
      else if ( wgm==1 || wgm==2 || wgm==3 ) {
	if ( compare0->config.output != HW_A1(_hw_occ_output_disconnected)
	     && compare0->config.output != HW_A1(_hw_occ_output_clear_after_match_up_set_after_match_down)
	     && compare0->config.output != HW_A1(_hw_occ_output_set_after_match_up_clear_after_match_down) )
	  return 7 ;
	  /* HWA_ERR("compare output A of class _ctd counter mode must be " */
	  /*	  "'disconnected', 'clear_after_match_up_set_after_match_down', " */
	  /*	  "or 'set_after_match_up_clear_after_match_down'."); */
      }
      else if ( wgm==8 || wgm==10 ) {
	if ( compare0->config.output != HW_A1(_hw_occ_output_disconnected)
	     && compare0->config.output != HW_A1(_hw_occ_output_toggle_after_match)
	     && compare0->config.output != HW_A1(_hw_occ_output_clear_after_match_up_set_after_match_down)
	     && compare0->config.output != HW_A1(_hw_occ_output_set_after_match_up_clear_after_match_down) )
	  return 8 ;
	  /* HWA_ERR("compare output A of class _ctd counter mode must be " */
	  /*	  "'disconnected', 'toggle_after_match', " */
	  /*	  "'clear_after_match_up_set_after_match_down', " */
	  /*	  "or 'set_after_match_up_clear_after_match_down'."); */
      }
      //    else if ( c->top == HW_A1(_hw_ctd_top_compare0)
      else if ( compare0->config.output )
	return 9 ;
	/* HWA_ERR("can not use of `compare_a` as both top value and output of class _ctd counter."); */
    }

    /*	Check compare output B
     */
    if ( compare1->config.output != 0xFF ) {
      if ( wgm==0 || wgm==4 || wgm==12 ) {
	if ( compare1->config.output != HW_A1(_hw_occ_output_disconnected)
	     && compare1->config.output != HW_A1(_hw_occ_output_clear_after_match)
	     && compare1->config.output != HW_A1(_hw_occ_output_set_after_match)
	     && compare1->config.output != HW_A1(_hw_occ_output_toggle_after_match) )
	  return 10 ;
	  /* HWA_ERR("compare output B of class _ctd counter mode must be " */
	  /*	  "'disconnected', 'clear_after_match', 'set_after_match', or " */
	  /*	  "'toggle_after_match'."); */
      }
      else if ( wgm==5 || wgm==6 || wgm==7 || wgm==14 || wgm==15 ) {
	if ( compare1->config.output != HW_A1(_hw_occ_output_disconnected)
	     && compare1->config.output != HW_A1(_hw_occ_output_set_at_bottom_clear_after_match)
	     && compare1->config.output != HW_A1(_hw_occ_output_clear_at_bottom_set_after_match) )
	  return 11 ;
	  /* HWA_ERR("compare output B of class _ctd counter mode must be " */
	  /*	  "'disconnected', 'set_at_bottom_clear_after_match', or " */
	  /*	  "'clear_at_bottom_set_after_match'."); */
      }
      else /* if ( wgm==1 || wgm==2 || wgm==3 || wgm==8 || wgm==9 || wgm==10 || wgm==11 ) */ {
	if ( compare1->config.output != HW_A1(_hw_occ_output_disconnected)
	     && compare1->config.output != HW_A1(_hw_occ_output_clear_after_match_up_set_after_match_down)
	     && compare1->config.output != HW_A1(_hw_occ_output_set_after_match_up_clear_after_match_down) )
	  return 12 ;
	  /* HWA_ERR("compare output B of class _ctd counter mode must be " */
	  /*	  "'disconnected', 'clear_after_match_up_set_after_match_down', " */
	  /*	  "or 'set_after_match_up_clear_after_match_down'."); */
      }
    }

    /*	Check compare update
     */
    if ( compare_update != 0xFF ) {
      if ( wgm==0 || wgm==4 || wgm==12 ) {
	if ( compare_update != HW_A1(_hw_occ_update_immediately) )
	  return 13 ;
	  /* HWA_ERR("optionnal parameter 'update' of class _ctd counter must be "\ */
	  /*	  "'immediately'."); */
      }
      else if ( wgm==8 || wgm==9 ) {
	if ( compare_update != HW_A1(_hw_occ_update_after_bottom) )
	  return 14 ;
	  /* HWA_ERR("optionnal parameter 'update' of class _ctd counter must be " */
	  /*	  "'after_bottom'."); */
      }
      else
	if( compare_update != HW_A1(_hw_occ_update_after_top) )
	  return 15 ;
	  /* HWA_ERR("optionnal parameter 'update' of class _ctd counter must be " */
	  /*	  "'after_top'."); */
    }

    /*	Overflow
     */
    if ( overflow != 0xFF ) {
      if ( (wgm==0 || wgm==4 || wgm==12) ) {
	if ( overflow != HW_A1(_hw_ctd_overflow_after_max) )
	  return 16 ;
	  /* HWA_ERR("optionnal parameter 'overflow' of class _ctd counter must be " */
	  /*	  "'after_max'."); */
      }
      else if ( (wgm==5 || wgm==6 || wgm==7 || wgm==14 || wgm==15) ) {
	if ( overflow != HW_A1(_hw_ctd_overflow_after_top) )
	  return 17 ;
	  /* HWA_ERR("optionnal parameter 'overflow' of class _ctd counter must be " */
	  /*	  "'after_top'."); */
      }
      else if ( overflow != HW_A1(_hw_ctd_overflow_after_bottom) )
	return 18 ;
	/* HWA_ERR("optionnal parameter 'overflow' of class _ctd counter must be " */
	/*	"'after_bottom'."); */
    }
  }

  return 0 ;
}


/**
 * @page atmelavr_ctd
 *
 * <br>
 * `read`:
 *
 * The count value is accessible for reading and writing through the following
 * instructions:
 */

/**
 * @page atmelavr_ctd
 * @code
 * hw( read, counter0 );
 * @endcode
 */
#define hw_read__ctd			, _hw_read_ctd
#define _hw_read_ctd(o,a,...)		_hw_read(o,count) HW_EOL(__VA_ARGS__)


/**
 * @page atmelavr_ctd
 *
 * <br>
 * `write`:
 *
 * @code
 * hw( write, counter0, value );
 * @endcode
 */
#define hw_write__ctd			, _hw_write_ctd
#define _hw_write_ctd(o,a,v,...)	_hw_write(o,count,v) HW_EOL(__VA_ARGS__)

/**
 * @page atmelavr_ctd
 * @code
 * hwa( write, counter0, value );
 * @endcode
 */
#define hwa_write__ctd			, _hwa_write_ctd
#define _hwa_write_ctd(o,a,v)		_hwa_write(o,count,v)



/**
 * @page atmelavr_ctd
 * @section atmelavr_ctd_st Status
 *
 * The overflow flag can be accessed through interrupt-related instructions:
 *
 * @code
 * if ( hw( read, (counter0,irq) ) ) {	// Read overflow IRQ flag
 *   hw( clear, (counter0,irq) );		// Clear overflow IRQ flag
 *   hw( disable, (counter0,irq) );	// Disable overflow IRQs
 * }
 * @endcode
 */


/*******************************************************************************
 *									       *
 *	Context management						       *
 *									       *
 *******************************************************************************/

#define _hwa_setup__ctd(o,a)			\
  _hwa_setup_r( o, ccra	        );		\
  _hwa_setup_r( o, ccrb	        );		\
  _hwa_setup_r( o, ocr0 );			\
  _hwa_setup_r( o, ocr1 );			\
  _hwa_setup_r( o, icr0 );			\
  _hwa_setup_r( o, ccrc	        );		\
  _hwa_setup_r( o, count        );		\
  _hwa_setup_r( o, imsk	        );		\
  _hwa_setup_r( o, ifr	        );		\
  hwa->o.config.clock	  = 0xFF ;		\
  hwa->o.config.direction = 0xFF ;		\
  hwa->o.config.top	  = 0xFF ;		\
  hwa->o.config.overflow  = 0xFF ;		\
  hwa->o.compare0.config.update	= 0xFF ;	\
  hwa->o.compare0.config.output	= 0xFF ;	\
  hwa->o.compare1.config.update	= 0xFF ;	\
  hwa->o.compare1.config.output	= 0xFF ;	\
  hwa->o.capture0.config.input  = 0xFF ;	\
  hwa->o.capture0.config.edge   = 0xFF ;	\
  hwa->o.capture0.config.filter = 0xFF

#define _hwa_init__ctd(o,a)			\
  _hwa_init_r( o, ccra,         0 );		\
  _hwa_init_r( o, ccrb,         0 );		\
  _hwa_init_r( o, ocr0, 0 );			\
  _hwa_init_r( o, ocr1, 0 );			\
  _hwa_init_r( o, icr0, 0 );			\
  _hwa_init_r( o, ccrc,         0 );		\
  _hwa_init_r( o, count,        0 );		\
  _hwa_init_r( o, imsk, 	0 );		\
  _hwa_init_r( o, ifr, 		0 )

#define _hwa_commit__ctd(o,a)			\
  _hwa_commit_r( o, ccra         );		\
  _hwa_commit_r( o, ccrb         );		\
  _hwa_commit_r( o, ccrc         );		\
  _hwa_commit_r( o, count        );		\
  _hwa_commit_r( o, ocr0 );			\
  _hwa_commit_r( o, ocr1 );			\
  _hwa_commit_r( o, icr0 );			\
  _hwa_commit_r( o, imsk         );		\
  _hwa_commit_r( o, ifr          )


/**
 * @page atmelavr_ctd
 * @section atmelavr_ctd_internals Internals
 *
 * Class `_ctd` objects hold the following hardware registers:
 *
 *  * `ccra`: control register a
 *  * `ccrb`: control register b
 *  * `ccrc`: control register c
 *  * `count`: count register
 *  * `ifr`: interrupt flag register
 *  * `imsk`: interrupt flag register
 *
 * that hold the following logical registers:
 *
 *  * `wgm`: counting and waveform generation mode
 *  * `cs`: clock selection
 *  * `ie`: overflow interrupt mask
 *  * `if`: overflow interrupt flag
 */

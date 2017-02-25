
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief Atmel AVR ATmega16U4/32U4
 */

#include "../hwa_1.h"

/**
 * @page atmegaxu4 ATmega16U4/32U4
 * @section attinyx8_device Device description symbols
 *
 * HWA defines the following symbols describing the target device and its
 * hardware configuration:
 *
 * Symbol		    | Comments
 * :------------------------|:-----------
 * `HW_DEVICE`		    |The device name as passed to the compiler
 * `HW_DEVICE_ATMEGAXU4`    |Defined void
 * `HW_DEVICE_ATMEGA32U4_MU`|Defined void depending on the HWA header included
 * `HW_DEVICE_PACKAGE_44Q`  |Defined void
 * `HW_DEVICE_RAM_START`    |Address of first RAM byte (after registers and I/O regs)
 * `HW_DEVICE_APP_START`    |Address of first application byte (after IRQ vector table)
 *
 * Symbol		       | ATmega16U4 | ATmega32U4
 * :---------------------------|:------------:|:------------
 * `HW_DEVICE`		       |	      |atmega32u4
 * `HW_DEVICE_SIGNATURE`       |              |0x1E,0x95,0x87
 * `HW_DEVICE_FLASH_SIZE`      |     	      |32768
 * `HW_DEVICE_FLASH_PAGE_SIZE` |   	      |128
 * `HW_DEVICE_EEPROM_SIZE`     |    	      |1024
 * `HW_DEVICE_EEPROM_PAGE_SIZE`| 	      |4
 * `HW_DEVICE_RAM_SIZE`	       |   	      |2560
 * `HW_DEVICE_RAM_START`       |       	      |0x0100
 * `HW_DEVICE_APP_START`       |      	      |0x00AC
 */
#define HW_DEVICE_ATMEGAXU4


/**
 * @page atmegaxu4
 * @section atmegaxu4_fuses Configuration fuses
 *
 * ATmegaXU4 devices hold 3 fuse bytes describing their hardware configuration.
 * HWA computes the value of these fuse bytes from the fuse values (detailed in
 * the following subsections) that must be defined prior to including the HWA
 * header. Factory defaults (shown with a bold face) are used for values that
 * are undefined as the HWA header is included.
 *
 * Symbol		| Comments
 * :--------------------|:-----------
 * `HW_DEVICE_FUSE_EB`	| Fuse extended byte
 * `HW_DEVICE_FUSE_HB`	| Fuse high byte
 * `HW_DEVICE_FUSE_LB`	| Fuse low byte
 */

#if !defined HW_DEVICE_FUSE_EBX
#  define HW_DEVICE_FUSE_EBX		0xF
#endif

#define HW_DEVICE_FUSE_EB			\
  (HW_DEVICE_FUSE_EBX & 0x0F)<<4 |		\
  HW_DEVICE_FUSE_HWBE<<3 |			\
  HW_DEVICE_FUSE_BODLEVEL

#define HW_DEVICE_FUSE_HB			\
  HW_DEVICE_FUSE_OCDEN<<7 |			\
  HW_DEVICE_FUSE_JTAGEN<<6 |			\
  HW_DEVICE_FUSE_SPIEN<<5 |			\
  HW_DEVICE_FUSE_WDTON<<4 |			\
  HW_DEVICE_FUSE_EESAVE<<3 |			\
  HW_DEVICE_FUSE_BOOTSZ<<1 |			\
  HW_DEVICE_FUSE_BOOTRST

#define HW_DEVICE_FUSE_LB			\
  HW_DEVICE_FUSE_CKDIV8<<7 |			\
  HW_DEVICE_FUSE_CKOUT<<6 |			\
  HW_DEVICE_FUSE_SUT10<<4 |			\
  HW_DEVICE_FUSE_CKSEL31<<1 |			\
  HW_DEVICE_FUSE_CKSEL0

/**
 * @page atmegaxu4
 * @subsection atmegaxu4_fuses_clocking Clocking
 *
 * HWA uses these settings to compute the system clock frequency `hw_syshz`.
 *
 * Symbol		 | Valid values | Comments
 * :---------------------|--------------|:-----------
 * `HW_DEVICE_CLK_SRC`	 | `external`<br><b>`rc_8MHz`</b><br>`low_freq_xosc`<br>`low_power_xosc`|External source on pin CLKI<br>Internal RC calibrated oscillator<br>Crystal/resonator oscillator between pins XTAL1 and XTAL2<br>
 * `HW_DEVICE_CLK_SRC_HZ`| Positive integer |Clock frequency for `external` and `xosc` clock source
 * `HW_DEVICE_CLK_PSC`	 | <b>`8`</b><br>`1`|Clock divided by 8<br>Clock not divided
 */
#define _hw_is_external_external			, 1
#define _hw_is_rc_8MHz_rc_8MHz				, 1
#define _hw_is_low_freq_xosc_low_freq_xosc		, 1
#define _hw_is_low_power_xosc_low_power_xosc		, 1


#ifndef HW_DEVICE_CLK_SRC
#  define HW_DEVICE_CLK_SRC				rc_8MHz
#endif

#if HW_IS(external,HW_DEVICE_CLK_SRC)
#  define HW_DEVICE_FUSE_CKSEL31			0
#  define HW_DEVICE_FUSE_CKSEL0				0
#elif HW_IS(rc_8MHz,HW_DEVICE_CLK_SRC)
#  define HW_DEVICE_FUSE_CKSEL31			1
#  define HW_DEVICE_FUSE_CKSEL0				0
#  define hw_syshz_base					8000000
#elif HW_IS(HW_DEVICE_CLK_SRC, low_freq_xosc)
#  define hw_syshz_base					HW_DEVICE_CLK_SRC_HZ
#  define HW_DEVICE_FUSE_CKSEL31			2
#
#elif HW_IS(HW_DEVICE_CLK_SRC, low_power_xosc)
#  ifndef HW_DEVICE_CLK_SRC_HZ
#    error HW_DEVICE_CLK_SRC_HZ must be defined as the frequency of the crystal used for clocking
#  endif
#  define hw_syshz_base					HW_DEVICE_CLK_SRC_HZ
#  if HW_DEVICE_CLK_SRC_HZ < 900000
#    define HW_DEVICE_FUSE_CKSEL31			4
#  elif HW_DEVICE_CLK_SRC_HZ < 3000000
#    define HW_DEVICE_FUSE_CKSEL31			5
#  elif HW_DEVICE_CLK_SRC_HZ < 8000000
#    define HW_DEVICE_FUSE_CKSEL31			6
#  else
#    define HW_DEVICE_FUSE_CKSEL31			7
#  endif
#else
#  error HW_DEVICE_CLK_SRC can be `rc_8MHz`, `low_freq_xosc`, `low_power_xosc`, or `external`.
#endif

/*  Check that we have a valid hw_syshz_base
 */
/* #if HW_IS(,hw_syshz_base) */
/* #  error HW_DEVICE_CLK_SRC_HZ must be defined as the frequency of the crystal used for clocking */
/* #endif */

#if !defined HW_DEVICE_CLK_PSC
#  define HW_DEVICE_CLK_PSC				8
#endif

#if HW_DEVICE_CLK_PSC == 8
#  define HW_DEVICE_FUSE_CKDIV8				0
#  define hw_syshz					hw_syshz_base/8
#elif HW_DEVICE_CLK_PSC == 1
#  define HW_DEVICE_FUSE_CKDIV8				1
#  define hw_syshz					hw_syshz_base
#else
HW_E_AVL('HW_DEVICE_CLK_PSC', HW_DEVICE_CLK_PSC, 1 | 8)
#endif


/**
 * @page atmegaxu4
 * @subsection atmegaxu4_fuses_sut Startup delays
 *
 * Symbol		| Valid values | Comments
 * :--------------------|--------------|:-----------
 * `HW_DEVICE_STARTUP_DELAYS`|`4CK`<br>`4CK_4ms`<br>`4CK_64ms`<br>`6CK_14CK`<br>`6CK_14CK_4ms`<br>`6CK_14CK_64ms`<br>`1KCK_4ms`<br>`1KCK_64ms`<br>`32KCK_64ms`<br>`258CK_14CK_4ms`<br>`258CK_14CK_64ms`<br>`1KCK_14CK`<br>`1KCK_14CK_4ms`<br>`1KCK_14CK_64ms`<br>`16KCK_14CK`<br>`16KCK_14CK_4ms`<br>`16KCK_14CK_64ms`| Valid values depend on the clocking configuration
*/
#define _hw_is_4CK_4CK					, 1
#define _hw_is_4CK_4ms_4CK_4ms				, 1
#define _hw_is_4CK_64ms_4CK_64ms			, 1
#define _hw_is_6CK_14CK_6CK_14CK			, 1
#define _hw_is_6CK_14CK_4ms_6CK_14CK_4ms		, 1
#define _hw_is_6CK_14CK_64ms_6CK_14CK_64ms		, 1
#define _hw_is_1KCK_4ms_1KCK_4ms			, 1
#define _hw_is_1KCK_64ms_1KCK_64ms			, 1
#define _hw_is_32KCK_64ms_32KCK_64ms			, 1
#define _hw_is_258CK_14CK_4ms_258CK_14CK_4ms		, 1
#define _hw_is_258CK_14CK_64ms_258CK_14CK_64ms		, 1
#define _hw_is_1KCK_14CK_1KCK_14CK			, 1
#define _hw_is_1KCK_14CK_4ms_1KCK_14CK_4ms		, 1
#define _hw_is_1KCK_14CK_64ms_1KCK_14CK_64ms		, 1
#define _hw_is_16KCK_14CK_16KCK_14CK			, 1
#define _hw_is_16KCK_14CK_4ms_16KCK_14CK_4ms		, 1
#define _hw_is_16KCK_14CK_64ms_16KCK_14CK_64ms		, 1

/*	SUT
 */
#if defined HW_DEVICE_STARTUP_DELAYS
#  if HW_DEVICE_CKSEL31 < 2
#    define HW_DEVICE_FUSE_CKSEL0				0
#    if HW_IS(HW_DEVICE_STARTUP_DELAYS, 6CK_14CK)
#      define HW_DEVICE_FUSE_SUT10				0
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 6CK_14CK_4ms)
#      define HW_DEVICE_FUSE_SUT10				1
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 6CK_14CK_64ms)
#      define HW_DEVICE_FUSE_SUT10				2
#    else
#      error HW_DEVICE_STARTUP_DELAYS must be defined as one of `6CK_14CK`, `6CK_14CK_4ms`, or `6CK_14CK_64ms`.
#    endif
#  elif HW_DEVICE_CKSEL31 == 2
#    if HW_IS(HW_DEVICE_STARTUP_DELAYS, 4CK)
#      define HW_DEVICE_FUSE_SUT10				0
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 4CK_4ms)
#      define HW_DEVICE_FUSE_SUT10				1
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 4CK_64ms)
#      define HW_DEVICE_FUSE_SUT10				2
#    else
#      error HW_DEVICE_STARTUP_DELAYS must be defined as one of `4CK`, `4CK_4ms`, or `4CK_64ms`.
#    endif
#  else
#    if HW_IS(HW_DEVICE_STARTUP_DELAYS, 258CK_14CK_4ms)
#      define HW_DEVICE_FUSE_CKSEL0				0
#      define HW_DEVICE_FUSE_SUT10				0
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 258CK_14CK_64ms)
#      define HW_DEVICE_FUSE_CKSEL0				0
#      define HW_DEVICE_FUSE_SUT10				1
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 1KCK_14CK)
#      define HW_DEVICE_FUSE_CKSEL0				0
#      define HW_DEVICE_FUSE_SUT10				2
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 1KCK_14CK_4ms)
#      define HW_DEVICE_FUSE_CKSEL0				0
#      define HW_DEVICE_FUSE_SUT10				3
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 1KCK_14CK_64ms)
#      define HW_DEVICE_FUSE_CKSEL0				1
#      define HW_DEVICE_FUSE_SUT10				0
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 16KCK_14CK)
#      define HW_DEVICE_FUSE_CKSEL0				1
#      define HW_DEVICE_FUSE_SUT10				1
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 16KCK_14CK_4ms)
#      define HW_DEVICE_FUSE_CKSEL0				1
#      define HW_DEVICE_FUSE_SUT10				2
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 16KCK_14CK_64ms)
#      define HW_DEVICE_FUSE_CKSEL0				1
#      define HW_DEVICE_FUSE_SUT10				3
#    else
#      error HW_DEVICE_STARTUP_DELAYS must be defined as one of `258CK_14CK_4ms`, `258CK_14CK_64ms`, `1KCK_14CK`, `1KCK_14CK_4ms`, `1KCK_14CK_64ms`, `16KCK_14CK`, `16KCK_14CK_4ms`, or `16KCK_14CK_64ms`.
#    endif
#  endif
#else
#  define HW_DEVICE_FUSE_SUT10					2
#  define HW_DEVICE_FUSE_CKSEL0					1
#endif


/**
 * @page atmegaxu4
 * @subsection atmegaxu4_fuses_other Other fuses
 *
 * Symbol		  | Valid values | Comments
 * :----------------------|--------------|:-----------
 * `HW_DEVICE_PIN_HWB`|<b>`enabled`</b><br>`disabled`|Whether the HWB pin allows to execute the bootloader section after reset when tied to ground during external reset pulse.
 * `HW_DEVICE_BROWNOUT_DETECTION`|<b>`off`</b><br>`1800_2200mV`<br>`2000_2400mV`<br>`2200_2600mV`<br>`2400_2800mV`<br>`3200_3600mV`<br>`3300_3700mV`<br>`4000_4500mV`|Brown-out detection level
 * `HW_DEVICE_ON_CHIP_DEBUG`|`enabled`<br><b>`disabled`</b>|Whether on-chip debugging is enabled
 * `HW_DEVICE_JTAG`|`enabled`<br><b>`disabled`</b>|Whether the JTAG interface is enabled
 * `HW_DEVICE_SERIAL_PROGRAMMING`|<b>`enabled`</b><br>`disabled`|Whether the device can be programmed via the SPI
 * `HW_DEVICE_WATCHDOG_ALWAYS_ON`|`yes`<br><b>`no`</b>		|Whether the watchdog is always running
 * `HW_DEVICE_PRESERVE_EEPROM_THROUGH_CHIP_ERASE`|`enabled`<br><b>`disabled`</b>|Whether the EEPROM memory is erased when a chip erase occurs
 * `HW_DEVICE_BOOT`|<b>`application`</b><br>`bootloader`|Whether the device starts the application or the bootloader after reset
 * `HW_DEVICE_BOOTSECTION_SIZE`|`512`<br>`1024`<br>`2048`<br><b>`4096`</b>|Size of the boot section
 * `HW_DEVICE_CLOCK_OUTPUT`|`enabled`<br><b>`disabled`</b>|Whether the device outputs its clock
 */

#if !defined HW_DEVICE_PIN_HWB
#  define HW_DEVICE_PIN_HWB			enabled
#endif

#if HW_IS(,HW_G2(_hw_state,HW_DEVICE_PIN_HWB))
#  define HW_DEVICE_FUSE_HWBE			\
  HW_A2(HW_G2(_hw_state,HW_DEVICE_PIN_HWB))
#else
#  error HW_DEVICE_PIN_HWB must be defined as `enabled` (default) or `disabled`.
#endif


#if !defined HW_DEVICE_BROWNOUT_DETECTION
#  define HW_DEVICE_BROWNOUT_DETECTION		off
#endif

#define _hw_bodlevel_off			, 7
#define _hw_bodlevel_1800_2200mV		, 6
#define _hw_bodlevel_2000_2400mV		, 5
#define _hw_bodlevel_2200_2600mV		, 4
#define _hw_bodlevel_2400_2800mV		, 3
#define _hw_bodlevel_3200_3600mV		, 2
#define _hw_bodlevel_3300_3700mV		, 1
#define _hw_bodlevel_4000_4500mV		, 0

#if HW_IS(,HW_G2(_hw_bodlevel,HW_DEVICE_BROWNOUT_DETECTION))
#  define HW_DEVICE_FUSE_BODLEVEL	HW_A1(HW_G2(_hw_bodlevel,HW_DEVICE_BROWNOUT_DETECTION))
#else
#  error HW_DEVICE_BROWNOUT_DETECTION must be defined as `off` (default), `1800_2200mV`, `2000_2400mV`, `2200_2600mV`, `2400_2800mV`, `3200_3600mV`, `3300_3700mV`, or `4000_4500mV`
#endif
/*
 *	Fuse high byte
 */
#if !defined HW_DEVICE_ON_CHIP_DEBUG
#  define HW_DEVICE_ON_CHIP_DEBUG		disabled
#endif

#if HW_IS(,HW_G2(_hw_state,HW_DEVICE_ON_CHIP_DEBUG))
#  define HW_DEVICE_FUSE_OCDEN				\
  HW_A2(HW_G2(_hw_state,HW_DEVICE_ON_CHIP_DEBUG))
#else
#  error HW_DEVICE_ON_CHIP_DEBUG must be defined as `enabled` or `disabled` (default).
#endif


#if !defined HW_DEVICE_JTAG
#  define HW_DEVICE_JTAG			enabled
#endif

#if HW_IS(,HW_G2(_hw_state,HW_DEVICE_JTAG))
#  define HW_DEVICE_FUSE_JTAGEN			\
  HW_A2(HW_G2(_hw_state,HW_DEVICE_JTAG))
#else
#  error HW_DEVICE_JTAG must be defined as `enabled` (default) or `disabled`.
#endif


#if !defined HW_DEVICE_SPI
#  define HW_DEVICE_SPI				enabled
#endif

#if HW_IS(,HW_G2(_hw_state,HW_DEVICE_SPI))
#  define HW_DEVICE_FUSE_SPIEN			\
  HW_A2(HW_G2(_hw_state,HW_DEVICE_SPI))
#else
#  error HW_DEVICE_SPI must be defined as `enabled` (default) or `disabled`.
#endif


#if !defined HW_DEVICE_WATCHDOG_ALWAYS_ON
#  define HW_DEVICE_WATCHDOG_ALWAYS_ON		no
#endif

#if HW_IS(,HW_G2(_hw_state,HW_DEVICE_WATCHDOG_ALWAYS_ON))
#  define HW_DEVICE_FUSE_WDTON				\
  HW_A2(HW_G2(_hw_state,HW_DEVICE_WATCHDOG_ALWAYS_ON))
#else
#  error HW_DEVICE_WATCHDOG_ALWAYS_ON must be defined as `yes` or `no` (default).
#endif


#if !defined HW_DEVICE_PRESERVE_EEPROM_THROUGH_CHIP_ERASE
#  define HW_DEVICE_PRESERVE_EEPROM_THROUGH_CHIP_ERASE	no
#endif

#if HW_IS(,HW_G2(_hw_state,HW_DEVICE_PRESERVE_EEPROM_THROUGH_CHIP_ERASE))
#  define HW_DEVICE_FUSE_EESAVE						\
  HW_A2(HW_G2(_hw_state,HW_DEVICE_PRESERVE_EEPROM_THROUGH_CHIP_ERASE))
#else
#  error HW_DEVICE_PRESERVE_EEPROM_THROUGH_CHIP_ERASE must be defined as `yes` or `no` (default).
#endif


#if !defined HW_DEVICE_BOOTSECTION_SIZE
#  define HW_DEVICE_BOOTSECTION_SIZE		4096
#endif

#if HW_DEVICE_BOOTSECTION_SIZE == 512
#  define HW_DEVICE_FUSE_BOOTSZ			3
#elif HW_DEVICE_BOOTSECTION_SIZE == 1024
#  define HW_DEVICE_FUSE_BOOTSZ			2
#elif HW_DEVICE_BOOTSECTION_SIZE == 2048
#  define HW_DEVICE_FUSE_BOOTSZ			1
#elif HW_DEVICE_BOOTSECTION_SIZE == 4096
#  define HW_DEVICE_FUSE_BOOTSZ			0
#else
#  error HW_DEVICE_BOOTSECTION_SIZE must be `512`, `1024`, `2048`, or `4096` (default).
#endif


#if !defined HW_DEVICE_BOOT
#  define HW_DEVICE_BOOT				application
#endif

#define _hw_is_application_application			, 1
#define _hw_is_bootloader_bootloader			, 1

#if HW_IS(HW_DEVICE_BOOT, application)
#  define HW_DEVICE_FUSE_BOOTRST			1
#elif HW_IS(HW_DEVICE_BOOT, bootloader)
#  define HW_DEVICE_FUSE_BOOTRST			0
#else
#  error HW_DEVICE_BOOT must be defined as `bootloader` or `application` (default).
#endif


#if !defined HW_DEVICE_CLOCK_OUTPUT
#  define HW_DEVICE_CLOCK_OUTPUT			disabled
#endif

#if HW_IS(,HW_G2(_hw_state,HW_DEVICE_CLOCK_OUTPUT))
#  define HW_DEVICE_FUSE_CKOUT				\
  HW_A2(HW_G2(_hw_state,HW_DEVICE_CLOCK_OUTPUT))
#else
#  error HW_DEVICE_CLOCK_OUTPUT must be defined as `enabled` or `disabled` (default).
#endif


/*******************************************************************************
 *									       *
 *	Peripherals							       *
 *									       *
 *******************************************************************************/

/**
 * @page atmegaxu4
 * @section atmegaxu4_object Peripherals
 *
 * @note A missing class name means that the peripheral is not implemented yet.
 *
 * Object name   | Class                           | Comments
 * :-------------|---------------------------------|:---------------------------
 * `hw_core0`    | @ref atmelavr_corec "_corec"    | The core
 * `hw_wdog0`    | @ref atmelavr_wdogb "_wdogb"    | Watchdog (WDG)
 * `hw_portb`    | @ref atmelavr_p8a "_p8a"        | General purpose I/O port B (PORTB)
 * `hw_portc`    | @ref atmelavr_p8a "_p8a"        | General purpose I/O port C (PORTC)
 * `hw_portd`    | @ref atmelavr_p8a "_p8a"        | General purpose I/O port D (PORTD)
 * `hw_porte`    | @ref atmelavr_p8a "_p8a"        | General purpose I/O port D (PORTE)
 * `hw_portf`    | @ref atmelavr_p8a "_p8a"        | General purpose I/O port D (PORTF)
 * `hw_int0`     | @ref atmelavr_inta "_inta"      | External interrupt INT0
 * `hw_int1`     | @ref atmelavr_inta "_inta"      | External interrupt INT1
 * `hw_int2`     | @ref atmelavr_inta "_inta"      | External interrupt INT2
 * `hw_int3`     | @ref atmelavr_inta "_inta"      | External interrupt INT3
 * `hw_int6`     | @ref atmelavr_inta "_inta"      | External interrupt INT6
 * `hw_pcic0`    | @ref atmelavr_pcica "_pcica"    | Pin change interrupt controller
 * `hw_psc0`     | @ref atmelavr_psca "_psca"      | hw_counter0/1/3 prescaler (PSC0)
 * `hw_counter0` | @ref atmelavr_c8a "_c8a"        | 8-bit counter-timer (T0)
 * `hw_oc00`     | @ref atmelavr_oc8a "_oc8a"      | Compare unit 0 of hw_counter0 (OC0A)
 * `hw_oc01`     | @ref atmelavr_oc8a "_oc8a"      | Compare unit 1 of hw_counter0 (OC0B)
 * `hw_counter1` | @ref atmelavr_c16b "_c16b"      | 16-bit counter-timer (T1)
 * `hw_oc10`     | @ref atmelavr_oc16a "_oc16a"    | Compare unit 0 of hw_counter1 (OC1A)
 * `hw_oc11`     | @ref atmelavr_oc16a "_oc16a"    | Compare unit 1 of hw_counter1 (OC1B)
 * `hw_oc12`     | @ref atmelavr_oc16a "_oc16a"    | Compare unit 2 of hw_counter1 (OC1C)
 * `hw_ic10`     | @ref atmelavr_ic16a "_ic16a"    | Capture unit of hw_counter1 (ICP1)
 * `hw_counter3` | @ref atmelavr_c16b "_c16b"      | 16-bit counter-timer (T1)
 * `hw_oc30`     | @ref atmelavr_oc16a "_oc16a"    | Compare unit 0 of hw_counter3 (OC3A)
 * `hw_oc31`     | @ref atmelavr_oc16a "_oc16a"    | Compare unit 1 of hw_counter3 (OC3B)
 * `hw_oc32`     | @ref atmelavr_oc16a "_oc16a"    | Compare unit 2 of hw_counter3 (OC3C)
 * `hw_ic30`     | @ref atmelavr_ic16a "_ic16a"    | Capture unit of hw_counter3 (ICP3)
 * `hw_acmp0`    | @ref atmelavr_acmpa "acmpa"     | Analog Comparator
 * `hw_eeprom0`  | @ref atmelavr_eeproma "eeproma" | Eeprom memory
 * `hw_flash0`   | @ref atmelavr_flasha "flasha"   | Flash memory
 */

/*
 * `hw_spi0`     | @ref atmelavr_spia "_spia"   | Serial Peripheral Interface
 * `hw_uart0`    | @ref atmelavr_uarta "_uarta" | Universal Asynchronous Receiver Transmitter
 * `hw_twi0`     | @ref atmelavr_twia "_twia"   | 2-wire Serial Interface
 * `hw_adc0`     | @ref atmelavr_ad10b "ad10b"  | 10-bit Analog to Digital Converter
 */

/*
 * @page atmegaxu4
 * @subsection atmegaxu4_swobj Software-emulated peripherals
 * 
 * HWA provides the following software-emulated peripherals:
 * 
 * Name			  | Class		  | Comments
 * :----------------------|-----------------------|:--------------------------------------
 * `hw_spimaster_swclk0`  | @ref atmelavr_usia_spimaster_swclk "usia_spimaster_swclk" | Universal Serial Interface used as SPI master and clocked by software
 * `hw_swuart0`		  | @ref atmelavr_swuarta "swuarta" | Software UART
 * `hw_swuart1`		  | @ref atmelavr_swuarta "swuarta" | Software UART
 */

/* @subsection atmegaxu4_objrel Aliases and relations
 *
 * Some objects can be accessed from their @ref using_relatives "relatives" or
 * can have more than one name. There are the existing relations between the
 * device's objects and their different names:
 *
 * Name		 | Aliases		 | Relations
 * :-------------|-----------------------|:--------------------------------------
 * `hw_psc0`	 | `hw_counter0prescaler`  | `HW_REL(hw_counter0, prescaler)`
 * `hw_psc0`	 | `hw_counter0prescaler0` | `HW_REL(hw_counter0, prescaler0)`
 * `hw_psc0`	 | `hw_counter1prescaler`  | `HW_REL(hw_counter1, prescaler)`
 * `hw_psc0`	 | `hw_counter1prescaler0` | `HW_REL(hw_counter1, prescaler0)`
 * `hw_oc00`	 | `hw_counter0compare0`   | `HW_REL(hw_counter0, compare0)`
 * `hw_oc01`	 | `hw_counter0compare1`   | `HW_REL(hw_counter0, compare1)`
 * `hw_counter0` | `hw_oc00counter`	   | `HW_REL(hw_oc00, counter)`
 * `hw_counter0` | `hw_oc01counter`	   | `HW_REL(hw_oc01, counter)`
 * `hw_pin_oc00` | `hw_oc00pin`		   | `HW_REL(hw_oc00, pin)`
 * `hw_pin_oc01` | `hw_oc01pin`		   | `HW_REL(hw_oc01, pin)`
 * `hw_oc10`	 | `hw_counter1compare0`   | `HW_REL(hw_counter1, compare0)`
 * `hw_oc11`	 | `hw_counter1compare1`   | `HW_REL(hw_counter1, compare1)`
 * `hw_counter1` | `hw_oc10counter`	   | `HW_REL(hw_oc10, counter)`
 * `hw_counter1` | `hw_oc11counter`	   | `HW_REL(hw_oc11, counter)`
 * `hw_pin_oc10` | `hw_oc10pin`		   | `HW_REL(hw_oc10, pin)`
 * `hw_pin_oc11` | `hw_oc11pin`		   | `HW_REL(hw_oc11, pin)`
 * `hw_pin_ic10` | `hw_ic10pin`		   | `HW_REL(hw_ic10, pin)`
 * `hw_counter2` | `hw_oc20counter`	   | `HW_REL(hw_oc20, counter)`
 * `hw_counter2` | `hw_oc21counter`	   | `HW_REL(hw_oc21, counter)`
 * `hw_pin_oc20` | `hw_oc20pin`		   | `HW_REL(hw_oc20, pin)`
 * `hw_pin_oc21` | `hw_oc21pin`		   | `HW_REL(hw_oc21, pin)`
 */


/*******************************************************************************
 *									       *
 *	Interrupts							       *
 *									       *
 *******************************************************************************/

/**
 * @page atmegaxu4
 * @section atmegaxu4_interrupts Interrupts
 * 
 * Interrupt definition                    | Atmel label  | Comments
 * :---------------------------------------|--------------|------------------------
 * `hw_wdog0`                              | WDT          | Watchdog Time-out Interrupt
 * `hw_eeprom0`                            | EE READY     | EEPROM ready
 * `hw_eeprom0,ready`                      | EE READY     | EEPROM ready
 * `hw_flash0`                             | SPM READY    | Store Program Memory Ready
 */
/**
 * @ingroup atmegaxu4_interrupts
 * @brief Definition of the interrupts
 */
/*					_irq, vector-1, object, ie, if
 */
#define _hw_irq_int0			_irq,  1, int0,     ie,   if
#define _hw_irq_int1			_irq,  2, int1,     ie,   if
#define _hw_irq_int2			_irq,  3, int2,     ie,   if
#define _hw_irq_int3			_irq,  4, int3,     ie,   if
#define _hw_irq_int6			_irq,  7, int6,     ie,   if
#define _hw_irq_pcic0		_irq,  9, pcic0,    ie,   if
#define _hw_irq_usb0			_irq, 10, usb0,     ie,   if
#define _hw_irq_usb0_endpoint	_irq, 11, usb0,     ieep, ifep
#define _hw_irq_wdog0		_irq, 12, wdog0,    ie,   if
#define _hw_irq_ic10			_irq, 16, ic10,     ie,   if
#define _hw_irq_counter1_capture	_irq, 16, ic10,     ie,   if
#define _hw_irq_oc10			_irq, 17, oc10,     ie,   if
#define _hw_irq_counter1_compare0	_irq, 17, oc10,     ie,   if
#define _hw_irq_oc11			_irq, 18, oc11,     ie,   if
#define _hw_irq_counter1_compare1	_irq, 18, oc11,     ie,   if
#define _hw_irq_oc12			_irq, 19, oc12,     ie,   if
#define _hw_irq_counter1_compare2	_irq, 19, oc12,     ie,   if
#define _hw_irq_counter1		_irq, 20, counter1, ie,   if
#define _hw_irq_counter1_overflow	_irq, 20, counter1, ie,   if
#define _hw_irq_oc00			_irq, 21, oc00,     ie,   if
#define _hw_irq_counter0_compare0	_irq, 21, oc00,     ie,   if
#define _hw_irq_oc01			_irq, 22, oc01,     ie,   if
#define _hw_irq_counter0_compare1	_irq, 22, oc01,     ie,   if
#define _hw_irq_counter0		_irq, 23, counter0, ie,   if
#define _hw_irq_counter0_overflow	_irq, 23, counter0, ie,   if
#define _hw_irq_spi0			_irq, 24, spi0,     ie,   if
#define _hw_irq_spi0_txc		_irq, 24, spi0,     ie,   if
#define _hw_irq_acmp0		_irq, 28, acmp0,    ie,   if
#define _hw_irq_adc0			_irq, 29, adc0,     ie,   if
#define _hw_irq_eeprom0		_irq, 30, eeprom0,  ie,   /* no irq flag */
#define _hw_irq_eeprom0_ready	_irq, 30, eeprom0,  ie,
#define _hw_irq_ic30			_irq, 31, ic30,     ie,   if
#define _hw_irq_oc30			_irq, 32, oc30,     ie,   if
#define _hw_irq_oc31			_irq, 33, oc31,     ie,   if
#define _hw_irq_oc32			_irq, 34, oc32,     ie,   if
#define _hw_irq_counter3		_irq, 35, counter3, ie,   if
#define _hw_irq_counter3_overflow	_irq, 35, counter3, ie,   if
#define _hw_irq_twi0			_irq, 36, twi0,     ie,   if
#define _hw_irq_flash0		_irq, 37, flash0,   ie,   if
#define _hw_irq_oc40			_irq, 38, oc40,     ie,   if
#define _hw_irq_oc41			_irq, 39, oc41,     ie,   if
#define _hw_irq_oc42			_irq, 40, oc42,     ie,   if
#define _hw_irq_counter4_overflow	_irq, 41, counter4, ieov, ifov
#define _hw_irq_counter4_fault	_irq, 42, counter4, iefp, iffp


/*******************************************************************************
 *									       *
 *	Shared registers						       *
 *									       *
 *	This is an object used to gather hardware registers shared	       *
 *	by several objects						       *
 *									       *
 *******************************************************************************/

/*	Object				class, id, address
 */
#define _hw_class__shared
#define _hw_def_shared		_shared, 101, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_shared_did1			_r8, 0x7F, 0x01, 0x00	/* DID for AIN0 */
#define _hw_reg_shared_did0			_r8, 0x7E, 0xF3, 0x00
#define _hw_reg_shared_did2			_r8, 0x7D, 0x3F, 0x00
#define _hw_reg_shared_eicrb		_r8, 0x6A, 0x30, 0x00
#define _hw_reg_shared_eicra		_r8, 0x69, 0xFF, 0x00
#define _hw_reg_shared_pcicr		_r8, 0x68, 0x01, 0x00
#define _hw_reg_shared_prr1			_r8, 0x65, 0x99, 0x00
#define _hw_reg_shared_prr0			_r8, 0x64, 0xAD, 0x00
#define _hw_reg_shared_gpior2		_r8, 0x4B, 0xFF, 0x00
#define _hw_reg_shared_gpior1		_r8, 0x4A, 0xFF, 0x00
#define _hw_reg_shared_gtccr		_r8, 0x43, 0x81, 0x00
#define _hw_reg_shared_gpior0		_r8, 0x3E, 0xFF, 0x00
#define _hw_reg_shared_eimsk		_r8, 0x3D, 0x4F, 0x00
#define _hw_reg_shared_eifr			_r8, 0x3C, 0x4F, 0x4F
#define _hw_reg_shared_pcifr		_r8, 0x3B, 0x01, 0x01

#if !defined __ASSEMBLER__
/*
 *  HWA context
 */
typedef struct {

  /*  Hardware registers
   */
  hwa_r8_t	eimsk ;
  hwa_r8_t	eicra, eicrb ;
  hwa_r8_t	eifr ;
  hwa_r8_t	gtccr ;
  hwa_r8_t	prr0, prr1 ;
  hwa_r8_t	did0, did1, did2 ;
  hwa_r8_t	pcicr ;
  hwa_r8_t	pcifr ;
} hwa_shared_t ;
#endif


/*******************************************************************************
 *									       *
 *	Core								       *
 *									       *
 *******************************************************************************/

#include "../classes/corec_1.h"

/*	Object				class, id, address
 */
#define _hw_def_core0		_corec, 102, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_core0_osccal		_r8, 0x66, 0xFF, 0x00
#define _hw_reg_core0_sreg			_r8, 0x5F, 0xFF, 0x00
#define _hw_reg_core0_sph			_r8, 0x5E, 0xFF, 0x00
#define _hw_reg_core0_spl			_r8, 0x5D, 0xFF, 0x00
#define _hw_reg_core0_mcucr			_r8, 0x55, 0x53, 0x00
#define _hw_reg_core0_mcusr			_r8, 0x54, 0x3F, 0x00
#define _hw_reg_core0_smcr			_r8, 0x53, 0x0F, 0x00

/*	Object logical registers
 */
#define _hw_reg_core0_pud			_ob1, mcucr, 1, 4
#define _hw_reg_core0_ivsel			_ob1, mcucr, 1, 1
#define _hw_reg_core0_ivce			_ob1, mcucr, 1, 0

#define _hw_reg_core0_usbrf			_ob1, mcusr, 1, 5
#define _hw_reg_core0_jtrf			_ob1, mcusr, 1, 4
#define _hw_reg_core0_wdrf			_ob1, mcusr, 1, 3
#define _hw_reg_core0_borf			_ob1, mcusr, 1, 2
#define _hw_reg_core0_extrf			_ob1, mcusr, 1, 1
#define _hw_reg_core0_porf			_ob1, mcusr, 1, 0
#define _hw_reg_core0_allrf			_ob1, mcusr, 6, 0	/* convenient */

#define _hw_reg_core0_sm			_ob1, smcr, 3, 1
#define _hw_reg_core0_se			_ob1, smcr, 1, 0


/*******************************************************************************
 *									       *
 *	hw_wdog0: watchdog						       *
 *									       *
 *******************************************************************************/

#include "../classes/wdogb_1.h"

/*	Object				class, id, address
 */
#define _hw_def_wdog0		_wdogb, 103, 0

/*	Class hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg__wdogb_csr			_r8, 0x60, 0xFF, 0x80

/*	Class logical registers
 */
#define _hw_reg__wdogb_wdrf			_xob1, core0, mcusr, 1, 3


/*******************************************************************************
 *									       *
 *	Ports and pins							       *
 *									       *
 *******************************************************************************/

/**
 * @page atmegaxu4
 * @section atmegaxu4_pins I/O ports and pins
 *
 * HWA name	| 44Q         | Class			   | Atmel name
 * -------------|-------------|----------------------------|-----------
 * `hw_port_b`	|	      | @ref atmelavr_io1a "_io1a" | PORTB
 * `hw_pin_pb0` | `hw_pin_8`  | @ref atmelavr_io1a "_io1a" | PB0
 * `hw_pin_pb1` | `hw_pin_9`  | @ref atmelavr_io1a "_io1a" | PB1
 * `hw_pin_pb2` | `hw_pin_10` | @ref atmelavr_io1a "_io1a" | PB2
 * `hw_pin_pb3` | `hw_pin_11` | @ref atmelavr_io1a "_io1a" | PB3
 * `hw_pin_pb4` | `hw_pin_28` | @ref atmelavr_io1a "_io1a" | PB4
 * `hw_pin_pb5` | `hw_pin_29` | @ref atmelavr_io1a "_io1a" | PB5
 * `hw_pin_pb6` | `hw_pin_30` | @ref atmelavr_io1a "_io1a" | PB6
 * `hw_pin_pb7` | `hw_pin_12` | @ref atmelavr_io1a "_io1a" | PB7
 * `hw_port_c`	|	      | @ref atmelavr_io1a "_io1a" | PORTC
 * `hw_pin_pc6` | `hw_pin_31` | @ref atmelavr_io1a "_io1a" | PC6
 * `hw_pin_pc7` | `hw_pin_32` | @ref atmelavr_io1a "_io1a" | PC7
 * `hw_port_d`	|	      | @ref atmelavr_io1a "_io1a" | PORTD
 * `hw_pin_pd0` | `hw_pin_18` | @ref atmelavr_io1a "_io1a" | PD0
 * `hw_pin_pd1` | `hw_pin_19` | @ref atmelavr_io1a "_io1a" | PD1
 * `hw_pin_pd2` | `hw_pin_20` | @ref atmelavr_io1a "_io1a" | PD2
 * `hw_pin_pd3` | `hw_pin_21` | @ref atmelavr_io1a "_io1a" | PD3
 * `hw_pin_pd4` | `hw_pin_25` | @ref atmelavr_io1a "_io1a" | PD4
 * `hw_pin_pd5` | `hw_pin_22` | @ref atmelavr_io1a "_io1a" | PD5
 * `hw_pin_pd6` | `hw_pin_26` | @ref atmelavr_io1a "_io1a" | PD6
 * `hw_pin_pd7` | `hw_pin_27` | @ref atmelavr_io1a "_io1a" | PD7
 * `hw_port_e`	|	      | @ref atmelavr_io1a "_io1a" | PORTE
 * `hw_pin_pe2` | `hw_pin_33` | @ref atmelavr_io1a "_io1a" | PE2
 * `hw_pin_pe6` | `hw_pin_1`  | @ref atmelavr_io1a "_io1a" | PE6
 * `hw_port_f`	|	      | @ref atmelavr_io1a "_io1a" | PORTF
 * `hw_pin_pf0` | `hw_pin_41` | @ref atmelavr_io1a "_io1a" | PF0
 * `hw_pin_pf1` | `hw_pin_40` | @ref atmelavr_io1a "_io1a" | PF1
 * `hw_pin_pf4` | `hw_pin_39` | @ref atmelavr_io1a "_io1a" | PF4
 * `hw_pin_pf5` | `hw_pin_38` | @ref atmelavr_io1a "_io1a" | PF5
 * `hw_pin_pf6` | `hw_pin_37` | @ref atmelavr_io1a "_io1a" | PF6
 * `hw_pin_pf7` | `hw_pin_36` | @ref atmelavr_io1a "_io1a" | PF7
 */

/*	Objects				class, id, address
 */
#include "../classes/p8a_1.h"
#include "../classes/io1a_1.h"

#define _hw_def_portb		_p8a, 104, 0x23
#define _hw_def_portc		_p8a, 105, 0x26
#define _hw_def_portd		_p8a, 106, 0x29
#define _hw_def_porte		_p8a, 107, 0x2C
#define _hw_def_portf		_p8a, 108, 0x2F

/*  Pins				class, id, port, bn, bp
 */
#define _hw_def_pin_pb0		_io1a, 109, portb, 1, 0
#define _hw_def_pin_pb1		_io1a, 110, portb, 1, 1
#define _hw_def_pin_pb2		_io1a, 111, portb, 1, 2
#define _hw_def_pin_pb3		_io1a, 112, portb, 1, 3
#define _hw_def_pin_pb4		_io1a, 113, portb, 1, 4
#define _hw_def_pin_pb5		_io1a, 114, portb, 1, 5
#define _hw_def_pin_pb6		_io1a, 115, portb, 1, 6
#define _hw_def_pin_pb7		_io1a, 116, portb, 1, 7

#define _hw_def_pin_pc6		_io1a, 117, portc, 1, 6
#define _hw_def_pin_pc7		_io1a, 118, portc, 1, 7

#define _hw_def_pin_pd0		_io1a, 119, portd, 1, 0
#define _hw_def_pin_pd1		_io1a, 120, portd, 1, 1
#define _hw_def_pin_pd2		_io1a, 121, portd, 1, 2
#define _hw_def_pin_pd3		_io1a, 122, portd, 1, 3
#define _hw_def_pin_pd4		_io1a, 123, portd, 1, 4
#define _hw_def_pin_pd5		_io1a, 124, portd, 1, 5
#define _hw_def_pin_pd6		_io1a, 125, portd, 1, 6
#define _hw_def_pin_pd7		_io1a, 126, portd, 1, 7

#define _hw_def_pin_pe2		_io1a, 127, porte, 1, 2
#define _hw_def_pin_pe6		_io1a, 128, porte, 1, 6

#define _hw_def_pin_pf0		_io1a, 129, portf, 1, 0
#define _hw_def_pin_pf1		_io1a, 130, portf, 1, 1
#define _hw_def_pin_pf4		_io1a, 131, portf, 1, 4
#define _hw_def_pin_pf5		_io1a, 132, portf, 1, 5
#define _hw_def_pin_pf6		_io1a, 133, portf, 1, 6
#define _hw_def_pin_pf7		_io1a, 134, portf, 1, 7

/*  Not connected pins
 */
#include "../classes/io0a_1.h"

#define _hw_def_pin_oc31		_io0a
#define _hw_def_pin_oc32		_io0a

/*  Digital input disable bits for analog input pins
 */
#define _hw_reg_pin_pf0_did		_xob1, shared, did0, 1, 0	/* ADC0 */
#define _hw_reg_pin_pf1_did		_xob1, shared, did0, 1, 1	/* ADC1 */
#define _hw_reg_pin_pf4_did		_xob1, shared, did0, 1, 4	/* ADC4 */
#define _hw_reg_pin_pf5_did		_xob1, shared, did0, 1, 5	/* ADC5 */
#define _hw_reg_pin_pf6_did		_xob1, shared, did0, 1, 6	/* ADC6 */
#define _hw_reg_pin_pf7_did		_xob1, shared, did0, 1, 7	/* ADC7 */

#define _hw_reg_pin_pd4_did		_xob1, shared, did2, 1, 0	/* ADC8 */
#define _hw_reg_pin_pd6_did		_xob1, shared, did2, 1, 1	/* ADC9 */
#define _hw_reg_pin_pd7_did		_xob1, shared, did2, 1, 2	/* ADC10 */
#define _hw_reg_pin_pb4_did		_xob1, shared, did2, 1, 3	/* ADC11 */
#define _hw_reg_pin_pb5_did		_xob1, shared, did2, 1, 4	/* ADC12 */
#define _hw_reg_pin_pb6_did		_xob1, shared, did2, 1, 5	/* ADC13 */

#define _hw_reg_pin_pe6_did		_xob1, shared, did1, 1, 0	/* AIN0 */

/*  Pin change controller mask
 */
#define _hw_def_pin_pb0_pcic		_xob1, 135, msk, 1, 0
#define _hw_def_pin_pb1_pcic		_xob1, 136, msk, 1, 1
#define _hw_def_pin_pb2_pcic		_xob1, 137, msk, 1, 2
#define _hw_def_pin_pb3_pcic		_xob1, 138, msk, 1, 3
#define _hw_def_pin_pb4_pcic		_xob1, 139, msk, 1, 4
#define _hw_def_pin_pb5_pcic		_xob1, 140, msk, 1, 5
#define _hw_def_pin_pb6_pcic		_xob1, 141, msk, 1, 6
#define _hw_def_pin_pb7_pcic		_xob1, 142, msk, 1, 7

/*  Relative pin change controllers
 */
#define _hw_rel_pin_pb0_pcic			pcic0
#define _hw_rel_pin_pb1_pcic			pcic0
#define _hw_rel_pin_pb2_pcic			pcic0
#define _hw_rel_pin_pb3_pcic			pcic0
#define _hw_rel_pin_pb4_pcic			pcic0
#define _hw_rel_pin_pb5_pcic			pcic0
#define _hw_rel_pin_pb6_pcic			pcic0
#define _hw_rel_pin_pb7_pcic			pcic0

/*  Canonical pin names
 */
#define _hw_pin_adc0			, pin_pf0
#define _hw_pin_adc1			, pin_pf1
#define _hw_pin_adc10			, pin_pd7
#define _hw_pin_adc11			, pin_pb4
#define _hw_pin_adc12			, pin_pb5
#define _hw_pin_adc13			, pin_pb6
#define _hw_pin_adc4			, pin_pf4
#define _hw_pin_adc5			, pin_pf5
#define _hw_pin_adc6			, pin_pf6
#define _hw_pin_adc7			, pin_pf7
#define _hw_pin_adc8			, pin_pd4
#define _hw_pin_adc9			, pin_pd6
#define _hw_pin_ain0			, pin_pe6
#define _hw_pin_clk0			, pin_pc7
#define _hw_pin_icp1			, pin_pd4
#define _hw_pin_icp3			, pin_pc7
#define _hw_pin_miso			, pin_pb3
#define _hw_pin_mosi			, pin_pb2
#define _hw_pin_ncts			, pin_pd5
#define _hw_pin_nhwb			, pin_pe2
#define _hw_pin_nint0			, pin_pd0
#define _hw_pin_nint1			, pin_pd1
#define _hw_pin_nint2			, pin_pd2
#define _hw_pin_nint3			, pin_pd3
#define _hw_pin_nint6			, pin_pe6
#define _hw_pin_noc40			, pin_pc6
#define _hw_pin_noc41			, pin_pb5
#define _hw_pin_noc43			, pin_pd6
#define _hw_pin_nrts			, pin_pb7
#define _hw_pin_oc00			, pin_pb7
#define _hw_pin_oc01			, pin_pd0
#define _hw_pin_oc10			, pin_pb5
#define _hw_pin_oc11			, pin_pb6
#define _hw_pin_oc12			, pin_pb7
#define _hw_pin_oc30			, pin_pc6
#define _hw_pin_oc40			, pin_pc7
#define _hw_pin_oc41			, pin_pb6
#define _hw_pin_oc43			, pin_pd7
#define _hw_pin_pdi			, pin_pb2
#define _hw_pin_pdo			, pin_pb3
#define _hw_pin_rxd1			, pin_pd2
#define _hw_pin_scl			, pin_pd0
#define _hw_pin_sclk			, pin_pb1
#define _hw_pin_sda			, pin_pd1
#define _hw_pin_ss			, pin_pb0
#define _hw_pin_t0			, pin_pd7
#define _hw_pin_t1			, pin_pd6
#define _hw_pin_tck			, pin_pf4
#define _hw_pin_tdi			, pin_pf7
#define _hw_pin_tdo			, pin_pf6
#define _hw_pin_tms			, pin_pf5
#define _hw_pin_txd1			, pin_pd3
#define _hw_pin_xck1			, pin_pd5

#if defined HW_DEVICE_PACKAGE_44Q
#
#  define _hw_pin_1			, pin_pe6
#  define _hw_pin_8			, pin_pb0
#  define _hw_pin_9			, pin_pb1
#  define _hw_pin_10			, pin_pb2
#  define _hw_pin_11			, pin_pb3
#  define _hw_pin_12			, pin_pb7
#  define _hw_pin_18			, pin_pd0
#  define _hw_pin_19			, pin_pd1
#  define _hw_pin_20			, pin_pd2
#  define _hw_pin_21			, pin_pd3
#  define _hw_pin_22			, pin_pd5
#  define _hw_pin_25			, pin_pd4
#  define _hw_pin_26			, pin_pd6
#  define _hw_pin_27			, pin_pd7
#  define _hw_pin_28			, pin_pb4
#  define _hw_pin_29			, pin_pb5
#  define _hw_pin_30			, pin_pb6
#  define _hw_pin_31			, pin_pc6
#  define _hw_pin_32			, pin_pc7
#  define _hw_pin_33			, pin_pe2
#  define _hw_pin_36			, pin_pf7
#  define _hw_pin_37			, pin_pf6
#  define _hw_pin_38			, pin_pf5
#  define _hw_pin_39			, pin_pf4
#  define _hw_pin_40			, pin_pf1
#  define _hw_pin_41			, pin_pf0
#endif


/*******************************************************************************
 *									       *
 *	External interrupt controllers					       *
 *									       *
 *******************************************************************************/

#include "../classes/inta_1.h"

/*	Object				class, id, address
 */
#define _hw_def_int0			_inta, 143, 0

/*	Object logical registers
 */
#define _hw_reg_int0_sc			_xob1, shared, eicra, 2, 0
#define _hw_reg_int0_ie			_xob1, shared, eimsk, 1, 0
#define _hw_reg_int0_if			_xob1, shared, eifr,  1, 0

/*	Object				class, id, address
 */
#define _hw_def_int1			_inta, 144, 0

/*	Object logical registers
 */
#define _hw_reg_int1_sc			_xob1, shared, eicra, 2, 2
#define _hw_reg_int1_ie			_xob1, shared, eimsk, 1, 1
#define _hw_reg_int1_if			_xob1, shared, eifr,  1, 1

/*	Object				class, id, address
 */
#define _hw_def_int2			_inta, 145, 0

/*	Object logical registers
 */
#define _hw_reg_int2_sc			_xob1, shared, eicra, 2, 4
#define _hw_reg_int2_ie			_xob1, shared, eimsk, 1, 2
#define _hw_reg_int2_if			_xob1, shared, eifr,  1, 2

/*	Object				class, id, address
 */
#define _hw_def_int3			_inta, 146, 0

/*	Object logical registers
 */
#define _hw_reg_int3_sc			_xob1, shared, eicra, 2, 6
#define _hw_reg_int3_ie			_xob1, shared, eimsk, 1, 3
#define _hw_reg_int3_if			_xob1, shared, eifr,  1, 3

/*	Object				class, id, address
 */
#define _hw_def_int6			_inta, 147, 0

/*	Object logical registers
 */
#define _hw_reg_int6_sc			_xob1, shared, eicrb, 2, 4
#define _hw_reg_int6_ie			_xob1, shared, eimsk, 1, 6
#define _hw_reg_int6_if			_xob1, shared, eifr,  1, 6


/*******************************************************************************
 *									       *
 *	Pin-change interrupt controller					       *
 *									       *
 *******************************************************************************/

#include "../classes/pcica_1.h"

/*	Object				class, id, address
 */
#define _hw_def_pcic0		_pcica, 148, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_pcic0_msk			_r8, 0x6B, 0xFF, 0x00

/*	Object logical registers
 */
#define _hw_reg_pcic0_ie			_xob1, shared, pcicr, 1, 0
#define _hw_reg_pcic0_if			_xob1, shared, pcifr, 1, 0


/*******************************************************************************
 *									       *
 *	Counter 0/1/3 prescaler						       *
 *									       *
 *******************************************************************************/

#include "../classes/psca_1.h"

/*	Object				class, id, address
 */
#define _hw_def_psc0			_psca, 149, 0

/*	Object logical registers
 */
#define _hw_reg_psc0_tsm			_xob1, shared, gtccr, 1, 7
#define _hw_reg_psc0_psr			_xob1, shared, gtccr, 1, 0

/*  This is a convenient logical register definition for stopping the prescaler
 */
#define _hw_reg_psc0_tsmpsr			_xob2, shared, gtccr, 1, 7, 1, gtccr, 1, 0, 0


/*******************************************************************************
 *									       *
 *	Counter 0 counting unit						       *
 *									       *
 *******************************************************************************/

#include "../classes/c8a_1.h"

/*	Object				class, id, address
 */
#define _hw_def_counter0		_c8a, 150, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_counter0_ccra		_r8, 0x44, 0xF3, 0x00
#define _hw_reg_counter0_ccrb		_r8, 0x45, 0xCF, 0x00
#define _hw_reg_counter0_count		_r8, 0x46, 0xFF, 0x00
#define _hw_reg_counter0_imsk		_r8, 0x6E, 0x07, 0x00
#define _hw_reg_counter0_ifr		_r8, 0x35, 0x07, 0x07

/*	Object logical registers
 */
#define _hw_reg_counter0_wgm		_ob2, ccra, 2, 0, 0, ccrb, 1, 3, 2
#define _hw_reg_counter0_cs			_ob1, ccrb, 3, 0
#define _hw_reg_counter0_ie			_ob1, imsk, 1, 0
#define _hw_reg_counter0_if			_ob1, ifr, 1, 0
#define _hw_reg_counter0_prr		_xob1, shared, prr, 1, 5


/*	Relatives
 */
#define _hw_rel_counter0_prescaler0		psc0
#define _hw_rel_counter0_prescaler		psc0
#define _hw_rel_counter0_compare0		oc00
#define _hw_rel_counter0_compare1		oc01


/*******************************************************************************
 *									       *
 *	Counter 0 compare units hw_oc00, hw_oc01 (OCR0A, OCR0B)		       *
 *									       *
 *******************************************************************************/

#include "../classes/oc8a_1.h"

/*	Object				class, id, address
 */
#define _hw_def_oc00			_oc8a, 151, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_oc00_reg			_r8, 0x47, 0xFF, 0x00	/* OCR0A */

/*	Object logical registers
 */
#define _hw_reg_oc00_com			_xob1, counter0, ccra, 2, 6
#define _hw_reg_oc00_force			_xob1, counter0, ccrb, 1, 7
#define _hw_reg_oc00_ie			_xob1, counter0, imsk, 1, 1
#define _hw_reg_oc00_if			_xob1, counter0, ifr,  1, 1

/*	Relatives
 */
#define _hw_rel_oc00_pin			pin_pb7
#define _hw_rel_oc00_counter			counter0


/*	Object				class, id, address
 */
#define _hw_def_oc01			_oc8a, 152, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_oc01_reg			_r8, 0x48, 0xFF, 0x00	/* OCR0B */

/*	Object logical registers
 */
#define _hw_reg_oc01_com			_xob1, counter0, ccra, 2, 4
#define _hw_reg_oc01_force			_xob1, counter0, ccrb, 1, 6
#define _hw_reg_oc01_ie			_xob1, counter0, imsk, 1, 2
#define _hw_reg_oc01_if			_xob1, counter0, ifr,  1, 2

/*	Relatives
 */
#define _hw_rel_oc01_pin			pin_pd0
#define _hw_rel_oc01_counter			counter0


/*******************************************************************************
 *									       *
 *	Counter 1 counting unit: hw_counter1				       *
 *									       *
 *******************************************************************************/

#include "../classes/c16b_1.h"

/*	Object				class, id, address
 */
#define _hw_def_counter1		_c16b, 153, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_counter1_ccra		_r8,  0x80, 0xF3,   0x00
#define _hw_reg_counter1_ccrb		_r8,  0x81, 0xDF,   0x00
#define _hw_reg_counter1_ccrc		_r8,  0x82, 0xC0,   0x00
#define _hw_reg_counter1_count		_r16, 0x84, 0xFFFF, 0x0000
#define _hw_reg_counter1_imsk		_r8,  0x6F, 0x2F,   0x00
#define _hw_reg_counter1_ifr		_r8,  0x36, 0x2F,   0x2F

/*	Object logical registers
 */
#define _hw_reg_counter1_cs			_ob1, ccrb, 3, 0
#define _hw_reg_counter1_wgm		_ob2, ccrb, 2, 3, 2, ccra, 2, 0, 0
#define _hw_reg_counter1_ie			_ob1, imsk, 1, 0
#define _hw_reg_counter1_if			_ob1, ifr,  1, 0
#define _hw_reg_counter1_prr		_xob1, shared, prr0, 1, 3

/*	Relatives
 */
#define _hw_rel_counter1_prescaler0		psc0
#define _hw_rel_counter1_prescaler		psc0
#define _hw_rel_counter1_compare0		oc10
#define _hw_rel_counter1_compare1		oc11
#define _hw_rel_counter1_compare2		oc12
#define _hw_rel_counter1_capture0		ic10


/*******************************************************************************
 *									       *
 *	Counter 1 compare unit 0: hw_oc10 (OCR1A)			       *
 *									       *
 *******************************************************************************/

#include "../classes/oc16a_1.h"

/*	Object				class, id, address
 */
#define _hw_def_oc10			_oc16a, 154, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_oc10_reg			_r16, 0x88, 0xFFFF, 0x0000	/* OCR1A */

/*	Object logical registers
 */
#define _hw_reg_oc10_com			_xob1, counter1, ccra, 2, 6
#define _hw_reg_oc10_force			_xob1, counter1, ccrc, 1, 7
#define _hw_reg_oc10_ie			_xob1, counter1, imsk, 1, 1
#define _hw_reg_oc10_if			_xob1, counter1, ifr,  1, 1

/*	Relatives
 */
#define _hw_rel_oc10_pin			pin_pb5
#define _hw_rel_oc10_counter			counter1


/*******************************************************************************
 *									       *
 *	Counter 1 compare unit 1: hw_oc11 (OCR1B)			       *
 *									       *
 *******************************************************************************/

/*	Object				class, id, address
 */
#define _hw_def_oc11			_oc16a, 155, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_oc11_reg			_r16, 0x8A, 0xFFFF, 0x0000	/* OCR1B */

/*	Object logical registers
 */
#define _hw_reg_oc11_com			_xob1, counter1, ccra, 2, 4
#define _hw_reg_oc11_force			_xob1, counter1, ccrc, 1, 6
#define _hw_reg_oc11_ie			_xob1, counter1, imsk, 1, 2
#define _hw_reg_oc11_if			_xob1, counter1, ifr,  1, 2

/*	Relatives
 */
#define _hw_rel_oc11_pin			pin_pb6
#define _hw_rel_oc11_counter			counter1


/*******************************************************************************
 *									       *
 *	Counter 1 compare unit 2: hw_oc12 (OCR1C)			       *
 *									       *
 *******************************************************************************/

/*	Object				class, id, address
 */
#define _hw_reg_oc12			_oc16a, 413, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_oc12_reg			_r16, 0x8C, 0xFFFF, 0x0000

/*	Object logical registers
 */
#define _hw_reg_oc12_com			_xob1, counter1, ccra, 2, 2
#define _hw_reg_oc12_force			_xob1, counter1, ccrc, 1, 5
#define _hw_reg_oc12_ie			_xob1, counter1, imsk, 1, 3
#define _hw_reg_oc12_if			_xob1, counter1, ifr,  1, 3

/*	Relatives
 */
#define _hw_rel_oc12_pin			pin_pb7
#define _hw_rel_oc12_counter			counter1


/*******************************************************************************
 *									       *
 *	Counter 1 capture unit: hw_ic10					       *
 *									       *
 *******************************************************************************/

#include "../classes/ic16a_1.h"

/*	Object				class, id, address
 */
#define _hw_def_ic10			_ic16a, 156, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_ic10_reg			_r16, 0x86, 0xFFFF, 0x0000	/* ICR1 */

/*	Object logical registers
 */
#define _hw_reg_ic10_acic			_xob1, acmp0, csr, 1, 2
#define _hw_reg_ic10_icnc			_xob1, counter1, ccrb, 1, 7
#define _hw_reg_ic10_ices			_xob1, counter1, ccrb, 1, 6
#define _hw_reg_ic10_ie			_xob1, counter1, imsk, 1, 5
#define _hw_reg_ic10_if			_xob1, counter1, ifr,  1, 5

/*	Relatives
 */
#define _hw_rel_ic10_pin			pin_pd4
#define _hw_rel_ic10_counter			counter1


/*******************************************************************************
 *									       *
 *	Counter 3 counting unit: hw_counter3				       *
 *									       *
 *******************************************************************************/

/*	Object				class, id, address
 */
#define _hw_def_counter3		_c16b, 157, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_counter3_ccra		_r8,  0x90, 0xF3,   0x00
#define _hw_reg_counter3_ccrb		_r8,  0x91, 0xDF,   0x00
#define _hw_reg_counter3_ccrc		_r8,  0x92, 0xC0,   0x00
#define _hw_reg_counter3_count		_r16, 0x94, 0xFFFF, 0x0000
#define _hw_reg_counter3_imsk		_r8,  0x71, 0x2F,   0x00
#define _hw_reg_counter3_ifr		_r8,  0x38, 0x2F,   0x2F

/*	Object logical registers
 */
#define _hw_reg_counter3_cs			_ob1, ccrb, 3, 0
#define _hw_reg_counter3_wgm		_ob2, ccrb, 2, 3, 2, ccra, 2, 0, 0
#define _hw_reg_counter3_ie			_ob1, imsk, 1, 0
#define _hw_reg_counter3_if			_ob1, ifr,  1, 0
#define _hw_reg_counter3_prr		_xob1, shared, prr1, 1, 3

/*	Relatives
 */
#define _hw_rel_counter3_prescaler0		psc0
#define _hw_rel_counter3_prescaler		psc0
#define _hw_rel_counter3_compare0		oc30
#define _hw_rel_counter3_compare1		oc31
#define _hw_rel_counter3_compare2		oc32
#define _hw_rel_counter3_capture0		ic30


/*******************************************************************************
 *									       *
 *	Counter 3 compare unit 0: hw_oc30 (OCR3A)			       *
 *									       *
 *******************************************************************************/

/*	Object				class, id, address
 */
#define _hw_def_oc30			_oc16a, 158, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_oc30_reg			_r16, 0x98, 0xFFFF, 0x0000	/* OCR1A */

/*	Object logical registers
 */
#define _hw_reg_oc30_com			_xob1, counter3, ccra, 2, 6
#define _hw_reg_oc30_force			_xob1, counter3, ccrc, 1, 7
#define _hw_reg_oc30_ie			_xob1, counter3, imsk, 1, 1
#define _hw_reg_oc30_if			_xob1, counter3, ifr,  1, 1

/*	Relatives
 */
#define _hw_rel_oc30_pin			pin_pc6
#define _hw_rel_oc30_counter			counter3


/*******************************************************************************
 *									       *
 *	Counter 3 compare unit 1: hw_oc31 (OCR3B)			       *
 *									       *
 *******************************************************************************/

/*	Object				class, id, address
 */
#define _hw_def_oc31			_oc16a, 159, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_oc31_reg			_r16, 0x9A, 0xFFFF, 0x0000	/* OCR1B */

/*	Object logical registers
 */
#define _hw_reg_oc31_com			_xob1, counter3, ccra, 2, 4
#define _hw_reg_oc31_force			_xob1, counter3, ccrc, 1, 6
#define _hw_reg_oc31_ie			_xob1, counter3, imsk, 1, 2
#define _hw_reg_oc31_if			_xob1, counter3, ifr,  1, 2

/*	Relatives
 */
#define _hw_rel_oc31_pin			pin_oc31
#define _hw_rel_oc31_counter			counter3


/*******************************************************************************
 *									       *
 *	Counter 3 compare unit 2: hw_oc32 (OCR3C)			       *
 *									       *
 *******************************************************************************/

/*	Object				class, id, address
 */
#define _hw_reg_oc32			_oc16a, 433, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_oc32_reg			_r16, 0x9C, 0xFFFF, 0x0000

/*	Object logical registers
 */
#define _hw_reg_oc32_com			_xob1, counter3, ccra, 2, 2
#define _hw_reg_oc32_force			_xob1, counter3, ccrc, 1, 5
#define _hw_reg_oc32_ie			_xob1, counter3, imsk, 1, 3
#define _hw_reg_oc32_if			_xob1, counter3, ifr,  1, 3

/*	Relatives
 */
#define _hw_rel_oc32_pin			pin_oc32
#define _hw_rel_oc32_counter			counter3


/*******************************************************************************
 *									       *
 *	Counter 3 capture unit: hw_ic30					       *
 *									       *
 *******************************************************************************/

#include "../classes/ic16b_1.h"

/*	Object				class, id, address
 */
#define _hw_def_ic30			_ic16b, 160, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_ic30_reg			_r16, 0x96, 0xFFFF, 0x0000	/* ICR3 */

/*	Object logical registers
 */
#define _hw_reg_ic30_acic			_xob1, acmp0, csr, 1, 2
#define _hw_reg_ic30_icnc			_xob1, counter3, ccrb, 1, 7
#define _hw_reg_ic30_ices			_xob1, counter3, ccrb, 1, 6
#define _hw_reg_ic30_ie			_xob1, counter3, imsk, 1, 5
#define _hw_reg_ic30_if			_xob1, counter3, ifr,  1, 5

/*	Relatives
 */
#define _hw_rel_ic30_pin			pin_pc7
#define _hw_rel_ic30_counter			counter3


/*******************************************************************************
 *									       *
 *	hw_acmp0: analog comparator					       *
 *									       *
 *******************************************************************************/

#include "../classes/acmpa_1.h"

/*	Object				class, id, address
 */					
#define _hw_def_acmp0		_acmpa, 161, 0

/*	Class hardware registers	class, address, write mask, flags mask
 */					
#define _hw_reg__acmpa_csr			_r8, 0x50, 0xDF, 0x10

/*	Object hardware registers	class, address, write mask, flags mask
 *	  Required for _hw_ic10_acic
 */
#define _hw_reg_acmp0_csr			_r8, 0x50, 0xDF, 0x10

/*	Object logical registers
 */
#define _hw_reg_acmp0_acme			_xob1, adc0, srb,   1, 6
#define _hw_reg_acmp0_aden			_xob1, adc0, sra,   1, 7
#define _hw_reg_acmp0_admux			_xob1, adc0, admux, 4, 0


/*******************************************************************************
 *									       *
 *	EEPROM memory							       *
 *									       *
 *******************************************************************************/

#include "../classes/eeproma_1.h"

/*	Object				class, id, address
 */					
#define _hw_def_eeprom0		_eeproma, 162, 0

/*	Class hardware registers	class, address, write mask, flags mask
 */					
#define _hw_reg__eeproma_ar			_r16, 0x41, 0x03FF, 0x00
#define _hw_reg__eeproma_dr			_r8,  0x40,   0xFF, 0x00
#define _hw_reg__eeproma_cr			_r8,  0x3F,   0x3F, 0x00


/*******************************************************************************
 *									       *
 *	Flash memory							       *
 *									       *
 *******************************************************************************/

#include "../classes/flasha_1.h"

/*	Object				class, id, address
 */					
#define _hw_def_flash0		_flasha, 163, 0

/*	Class hardware registers	class, address, write mask, flags mask
 */					
#define _hw_reg__flasha_csr			_r8, 0x57, 0xBF, 0x00


/*******************************************************************************
 *									       *
 *	H W A    C O N T E X T						       *
 *									       *
 *******************************************************************************/

#if !defined __ASSEMBLER__

/**
 * @brief The HWA context
 *
 * This structure is instanciated by hwa_begin() or hwa_begin_from_reset() and
 * used by all HWA asynchronous instructions to bufferize hardware accesses.
 */
typedef struct {
  uint8_t	commit ;	/*!< 1 if commit does write into hardware registers	*/

  hwa_shared_t	shared ;

  hwa_corec_t	core0 ;
  hwa_wdogb_t	wdog0 ;
  hwa_p8a_t	portb ;
  hwa_p8a_t	portc ;
  hwa_p8a_t	portd ;
  hwa_p8a_t	porte ;
  hwa_p8a_t	portf ;

  hwa_c8a_t	counter0 ;
  hwa_oc8a_t	oc00 ;
  hwa_oc8a_t	oc01 ;

  hwa_c16b_t	counter1 ;
  hwa_oc16a_t	oc10 ;
  hwa_oc16a_t	oc11 ;
  hwa_oc16a_t	oc12 ;
  hwa_ic16a_t	ic10 ;

  hwa_c16b_t	counter3 ;
  hwa_oc16a_t	oc30 ;
  hwa_oc16a_t	oc31 ;
  hwa_oc16a_t	oc32 ;
  hwa_ic16a_t	ic30 ;

  hwa_acmpa_t	acmp0 ;
} hwa_t ;


#include "../hwa_2.h"
#include "../classes/corec_2.h"
#include "../classes/wdogb_2.h"
#include "../classes/io0a_2.h"
#include "../classes/io1a_2.h"
#include "../classes/p8a_2.h"

#include "../classes/oc8a_2.h"
#include "../classes/psca_2.h"
#include "../classes/c8a_2.h"

#include "../classes/ic16a_2.h"
#include "../classes/ic16b_2.h"
#include "../classes/oc16a_2.h"
#include "../classes/c16b_2.h"

#include "../classes/acmpa_2.h"


HW_INLINE void _hwa_setup_context( hwa_t *hwa )
{
  _hwa_setup_reg( shared, eimsk );
  _hwa_setup_reg( shared, eifr  );
  _hwa_setup_reg( shared, eicra );
  _hwa_setup_reg( shared, eicrb );
  _hwa_setup_reg( shared, gtccr );
  _hwa_setup_reg( shared, prr0  );
  _hwa_setup_reg( shared, prr1  );
  _hwa_setup_reg( shared, did0  );
  _hwa_setup_reg( shared, did1  );
  _hwa_setup_reg( shared, did2  );
  _hwa_setup_reg( shared, pcicr );
  _hwa_setup_reg( shared, pcifr );

  _hwa_setup( core0 );
  _hwa_setup( wdog0 );
  _hwa_setup( portb );
  _hwa_setup( portc );
  _hwa_setup( portd );
  _hwa_setup( porte );
  _hwa_setup( portf );

  _hwa_setup( oc00 );
  _hwa_setup( oc01 );
  _hwa_setup( counter0 );

  _hwa_setup( oc10 );
  _hwa_setup( oc11 );
  _hwa_setup( oc12 );
  _hwa_setup( ic10 );
  _hwa_setup( counter1 );

  _hwa_setup( oc30 );
  _hwa_setup( oc31 );
  _hwa_setup( oc32 );
  _hwa_setup( ic30 );
  _hwa_setup( counter3 );

  _hwa_setup( acmp0 );
}


HW_INLINE void _hwa_init_context( hwa_t *hwa )
{
  _hwa_init_reg( shared, eimsk, 0 );
  _hwa_init_reg( shared, eifr,  0 );
  _hwa_init_reg( shared, eicra, 0 );
  _hwa_init_reg( shared, eicrb, 0 );
  _hwa_init_reg( shared, gtccr, 0 );
  _hwa_init_reg( shared, prr0,  0 );
  _hwa_init_reg( shared, prr1,  0 );
  _hwa_init_reg( shared, did0,  0 );
  _hwa_init_reg( shared, did1,  0 );
  _hwa_init_reg( shared, did2,  0 );
  _hwa_init_reg( shared, pcicr, 0 );
  _hwa_init_reg( shared, pcifr, 0 );

  _hwa_init( core0 );
  _hwa_init( wdog0 );
  _hwa_init( portb );
  _hwa_init( portc );
  _hwa_init( portd );
  _hwa_init( porte );
  _hwa_init( portf );

  _hwa_init( oc00 );
  _hwa_init( oc01 );
  _hwa_init( counter0 );

  _hwa_init( oc10 );
  _hwa_init( oc11 );
  _hwa_init( oc12 );
  _hwa_init( ic10 );
  _hwa_init( counter1 );

  _hwa_init( oc30 );
  _hwa_init( oc31 );
  _hwa_init( oc32 );
  _hwa_init( ic30 );
  _hwa_init( counter3 );

  _hwa_init( acmp0 );
}


HW_INLINE void _hwa_commit_context( hwa_t *hwa )
{
  _hwa_solve( counter0 );
  _hwa_solve( counter1 );
  _hwa_solve( counter3 );

  _hwa_commit_reg( shared, eimsk );
  _hwa_commit_reg( shared, eifr  );
  _hwa_commit_reg( shared, eicra );
  _hwa_commit_reg( shared, eicrb );
  _hwa_commit_reg( shared, pcifr );
  _hwa_commit_reg( shared, pcicr );
  _hwa_commit_reg( shared, gtccr );
  _hwa_commit_reg( shared, prr0  );
  _hwa_commit_reg( shared, prr1  );
  _hwa_commit_reg( shared, did0  );
  _hwa_commit_reg( shared, did1  );
  _hwa_commit_reg( shared, did2  );

  _hwa_commit( core0 );
  _hwa_commit( wdog0 );
  _hwa_commit( portb );
  _hwa_commit( portc );
  _hwa_commit( portd );
  _hwa_commit( porte );
  _hwa_commit( portf );

  _hwa_commit( counter0 );
  _hwa_commit( oc00 );
  _hwa_commit( oc01 );

  _hwa_commit( counter1 );
  _hwa_commit( oc10 );
  _hwa_commit( oc11 );
  _hwa_commit( oc12 );
  _hwa_commit( ic10 );

  _hwa_commit( counter3 );
  _hwa_commit( oc30 );
  _hwa_commit( oc31 );
  _hwa_commit( oc32 );
  _hwa_commit( ic30 );

  _hwa_commit( acmp0 );
}

#endif /* !defined __ASSEMBLER__ */

/**
 * @page atmegaxu4
 * <br>
 */

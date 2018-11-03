
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief Definitions common to all Espressif devices that do not produce C code
 */

#include "../hwa/hwa_1.h"


/**
 * @ingroup espressif_public_gen_macros
 * @brief Defined for all Atmel AVR devices
 */
#define HW_DEVICE_ESPRESSIF

/**
 * @ingroup espressif_public_gen_macros
 * @brief Defined as `atmel` for all Atmel devices
 */
#define HW_DEVICE_VENDOR	espressif

/**
 * @ingroup espressif_public_gen_macros
 * @brief Defined as `avr` for all Atmel AVR devices
 */
#define HW_DEVICE_ARCH		xtensa

/**
 * @ingroup espressif_public_gen_macros
 * @brief Expands to the CPU clock frequency (cpu cycles per second).
 */
#if defined DOXYGEN
#  define HW_SYSHZ
#endif

/*  Address of a memory definition
 */
#define _HW_A__m1(o,a,r,rc,ra,...)		(a+ra)
#define _HW_A__r32(ra,rwm,rfm,o,c,a,r)		(a+ra)

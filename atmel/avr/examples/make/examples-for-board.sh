#!/bin/bash

examples_for_board() {
    BOARD="$1"
    EXAMPLES=""

    if [ ${BOARD} = "attiny84" ] ; then
	EXAMPLES="${EXAMPLES} 01-1-blink-soft"
	EXAMPLES="${EXAMPLES} 02-1-blink-watchdog-stat"
	EXAMPLES="${EXAMPLES} 02-2-blink-watchdog-irq"
	EXAMPLES="${EXAMPLES} 02-3-blink-watchdog-irq-reset"
	EXAMPLES="${EXAMPLES} 03-1-blink-counter-overflow-flag"
	EXAMPLES="${EXAMPLES} 03-2-blink-counter-compare-flag"
	EXAMPLES="${EXAMPLES} 03-3-blink-counter-overflow-irq"
	EXAMPLES="${EXAMPLES} 03-4-blink-counter-compare-irq"
	EXAMPLES="${EXAMPLES} 03-5-fade-counter-compare-output"
	EXAMPLES="${EXAMPLES} 03-6-fade-counter-compare-output"
	EXAMPLES="${EXAMPLES} 04-1-fade-adc"
	EXAMPLES="${EXAMPLES} 05-1-swuart-tx"
	EXAMPLES="${EXAMPLES} 05-2-swuart-sync-rx-tx"
	EXAMPLES="${EXAMPLES} 05-3-dual-swuart"
	EXAMPLES="${EXAMPLES} 06-1-swuart-adc"
	EXAMPLES="${EXAMPLES} 07-1-swuart-eeprom-read-write"
	EXAMPLES="${EXAMPLES} 08-1-swuart-flash-read-write"
	EXAMPLES="${EXAMPLES} 09-1-swuart-usi-spi-master-nrf24l01+"
	EXAMPLES="${EXAMPLES} 09-2-swuart-usi-spi-master-nrf24l01+"
	EXAMPLES="${EXAMPLES} 10-1-acmp"
    fi
    if [ ${BOARD} = "attiny84-tcs3200" ] ; then
	EXAMPLES="99-01-TCS3200-color-detector"
    fi
    if [ ${BOARD} = "attiny85" ] ; then
	EXAMPLES="${EXAMPLES} 01-1-blink-soft"
	EXAMPLES="${EXAMPLES} 02-1-blink-watchdog-stat"
	EXAMPLES="${EXAMPLES} 02-2-blink-watchdog-irq"
	EXAMPLES="${EXAMPLES} 02-3-blink-watchdog-irq-reset"
	EXAMPLES="${EXAMPLES} 03-1-blink-counter-overflow-flag"
	EXAMPLES="${EXAMPLES} 03-2-blink-counter-compare-flag"
	EXAMPLES="${EXAMPLES} 03-3-blink-counter-overflow-irq"
	EXAMPLES="${EXAMPLES} 03-4-blink-counter-compare-irq"
	EXAMPLES="${EXAMPLES} 03-5-fade-counter-compare-output"
	EXAMPLES="${EXAMPLES} 03-6-fade-counter-compare-output"
	EXAMPLES="${EXAMPLES} 03-7-pwm-dead-time"
	EXAMPLES="${EXAMPLES} 04-1-fade-adc"
	EXAMPLES="${EXAMPLES} 05-1-swuart-tx"
	EXAMPLES="${EXAMPLES} 05-2-swuart-sync-rx-tx"
	EXAMPLES="${EXAMPLES} 06-1-swuart-adc"
	EXAMPLES="${EXAMPLES} 07-1-swuart-eeprom-read-write"
	EXAMPLES="${EXAMPLES} 08-1-swuart-flash-read-write"
	EXAMPLES="${EXAMPLES} 09-1-swuart-usi-spi-master-nrf24l01+"
	EXAMPLES="${EXAMPLES} 09-2-swuart-usi-spi-master-nrf24l01+"
	EXAMPLES="${EXAMPLES} 10-1-acmp"
    fi
    if [ ${BOARD} = "nanodccduino" ] ; then
	EXAMPLES="${EXAMPLES} 01-1-blink-soft"
	EXAMPLES="${EXAMPLES} 02-1-blink-watchdog-stat"
	EXAMPLES="${EXAMPLES} 02-2-blink-watchdog-irq"
	EXAMPLES="${EXAMPLES} 02-3-blink-watchdog-irq-reset"
	EXAMPLES="${EXAMPLES} 03-1-blink-counter-overflow-flag"
	EXAMPLES="${EXAMPLES} 03-2-blink-counter-compare-flag"
	EXAMPLES="${EXAMPLES} 03-3-blink-counter-overflow-irq"
	EXAMPLES="${EXAMPLES} 03-4-blink-counter-compare-irq"
	EXAMPLES="${EXAMPLES} 03-5-fade-counter-compare-output"
	EXAMPLES="${EXAMPLES} 03-6-fade-counter-compare-output"
	EXAMPLES="${EXAMPLES} 04-1-fade-adc"
	EXAMPLES="${EXAMPLES} 05-1-swuart-tx"
	EXAMPLES="${EXAMPLES} 05-2-swuart-sync-rx-tx"
	EXAMPLES="${EXAMPLES} 05-3-dual-swuart"
	EXAMPLES="${EXAMPLES} 06-1-swuart-adc"
	EXAMPLES="${EXAMPLES} 07-1-swuart-eeprom-read-write"
	EXAMPLES="${EXAMPLES} 08-1-swuart-flash-read-write"
	EXAMPLES="${EXAMPLES} 09-3-uart-spi-nrf24l01+"
	EXAMPLES="${EXAMPLES} 10-1-acmp"
	EXAMPLES="${EXAMPLES} 11-1-uart-twi-eeprom"
    fi
}

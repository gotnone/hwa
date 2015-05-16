
/*	A single Atmel AVR ATtiny84 without any external component except a
 *	USB/Serial adapter.
 *
 *	The serial communication uses 1 wire for RX/TX. The AVR communication
 *	pin is connected to the RXD pin of the serial adapter and a 1k resistor
 *	is connected between the RXD and TXD pins.
 *
 *	Install Diabolo with avrdude/usbasp:
 *
 *	USBASP	1	MOSI	7   DEVICE
 *		2	VCC	1
 *		4	GND	14
 *		5	RST	4
 *		7	SCK	9
 *		9	MISO	8
 */
#define HW_DEVICE			attiny84
#define HW_DEVICE_PACKAGE		14pdip
#define HW_DEVICE_CLK_SRC		rc_8MHz
#define HW_DEVICE_CLK_PSC		1
#define HW_DEVICE_EXTERNAL_RESET	enabled
#define HW_DEVICE_SELF_PROGRAMMING	enabled
#define HW_DEVICE_DEBUG_WIRE		disabled
#define HW_DEVICE_WATCHDOG_ALWAYS_ON	no
#define HW_DEVICE_CLOCK_OUTPUT		disabled
#define HW_DEVICE_BROWNOUT_DETECTION	2500_2900mV

/*	Settings for Diabolo
 */
#define DIABOLO_PIN_RX			hw_pin_2
#define DIABOLO_PIN_TX			hw_pin_2
#define DIABOLO_BPS			115200
#define DIABOLO_RESET_SIGNAL		DTR
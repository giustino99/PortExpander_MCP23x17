/*
 Name:		MCP23S17.h
 Created:	24/09/2020 16:33:09
 Author:	LucaD
 Editor:	http://www.visualmicro.com
*/

#ifndef _MCP23S17_h
#define _MCP23S17_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "SPI.h"

#define _READ 0x01
#define _WRITE 0xFE

/* ALL THIS REGISTER ARE FOR BANK = 1 */

/*
* Direction port register
* set: pin as input
* clear: pin as output
*/
#define _IODIR_ADDR[2][2] {{0x00, 0x00}, {0x01, 0x10}}

/*
* Input polarity register
* set: GPIO register bit reflect negate logic state of the input pin
* clear: GPIO register bit reflect logic state of the input pin
*/
#define _IPOL_ADDR[2][2] {{0x02, 0x01}, {0x03, 0x11}}

/*
* Interrupt on change register
* set: enable interrupt on change
* clear: disable interrupt on change
*/
#define _GPINTEN_ADDR[2][2] {{0x04, 0x02}, }

/*
* Default compare register for interrupt on change
* if the associated pin level is the opposite of this register bit interrupt occur
*/
#define _DEFVAL_ADDR[2][2] {{0x06, 0x03}, {0x07, 0x13}}

/*
	* Interrupt control register
	* set: pin value compared to DEFVAL register pin
	* clear: pin value compared to previous pin value
	*/
#define _INTCON_ADDR[2][2] {{0x08, 0x04}, {0x09, 0x14}}

/*
* Configuration register
* MSB                                          LSB
* bank mirror  seqop disslw  haen  odr intpol  --
*
* bank:    set:    register of each port separated in different banks
*          clear:  register in the same bank (sequential addresses)
*
* mirror:  set:    INT pins internally connected
*          clear:  INTA associated with PORTA, same for INTB and PORTB
*
* seqop:   set:    sequential op disabled (addr pointer does not increment)
*          clear:  sequential op enabled(addr pointer increments)
*
* disslw:  set:    slew rate disabled
*          clear:  slew rate enabled
*
* haen:    set:    enable MCP23S17 address pins
*          clear:  disable MCP23S17 address pins
*
* odr:     set:    open drain output (overrides INTPOL bit)
*          clear:  active driver output (INTPOL sets polarity)
*
* intpol:  set:    pin active high
*          clear:  pin active low
*
*/
#define _IOCON_ADDR[2] {0x0A, 0x05}
//#define _IOCON_VAL 0b10101100

/*
* GPIO pull-up register
* set: pull-up enabled
* clear: pull-up disabled
*/
#define _GPPU_ADDR[2][2] {{0x0C, 0x06}, {0x0D, 0x16}}

/*
* Interrupt flag register
* set: this pin can cause interrupt
* clear: pin does not cause interrupt
*/
#define _INTF_ADDR[2][2] {{0x0E, 0x07}, {0x0F, 0x17}}

/*
* Interrupt captured value register
* set: logic high
* clear: logic low
*/
#define _INTCAP_ADDR[2][2] {{0x10, 0x08}, {0x11, 0x18}}

/*
* Port register
* set: logic high
* clear: logic low
*/
#define _GPIO_ADDR[2][2] {{0x12, 09}, {0x13, 0x19}}

#define ALLIN	0xFF
#define ALLOUT	0x00

#define EPORTA 0
#define EPORTB 1
#define E0	0
#define E1	1
#define E2	2
#define E3	3
#define E4	4
#define E5	5
#define E6	6
#define E7	7
#define E8	8
#define E9	9
#define E10	10
#define E11	11
#define E12	12
#define E13	13
#define E14	14
#define E15	15
namespace MCP23S17 {
	const uint8_t registerBank[19][2] = {
		{0x00, 0x00},
		{0x01, 0x10},
		{0x02, 0x01},
		{0x03, 0x11},
		{0x04, 0x02},
		{0x05, 0x12},
		{0x06, 0x03},
		{0x07, 0x13},
		{0x08, 0x04},
		{0x0A, 0x05},
		{0x0B, 0x15},
		{0x0C, 0x06},
		{0x0D, 0x16},
		{0x0E, 0x07},
		{0x0F, 0x17},
		{0x10, 0x08},
		{0x11, 0x18},
		{0x12, 0x09},
		{0x13, 0x19}
	};

	class PortExpanderSPI {
		private:
			uint8_t cs;
			uint8_t address;
			uint8_t bank;

			void write(uint8_t addr, uint8_t value);
			uint8_t read(uint8_t addr);

		public:
			void init(uint8_t CS, uint8_t addr, bool seqAddr, bool openDrain);
			void init(uint8_t CS, uint8_t addr);
			void init(uint8_t CS);

			void setPortDirection(uint8_t port, uint8_t dir);
			//void setPortDirection(uint8_t port, uint8_t dir, uint8_t pullup);
			void setPortDirection(uint8_t port, uint8_t bit, uint8_t dir);

			uint8_t readIO(uint8_t port);
			bool readIO(uint8_t port, uint8_t bit);

			void writeIO(uint8_t port, uint8_t value);
			void writeIO(uint8_t port, uint8_t bit, bool val);
	};
};

#endif


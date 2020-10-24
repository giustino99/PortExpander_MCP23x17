/*
 Name:		MCP23S17.cpp
 Created:	24/09/2020 16:33:09
 Author:	LucaD
 Editor:	http://www.visualmicro.com
*/

#include "MCP23S17.h"
#include "Arduino.h"
#include "SPI.h"
using namespace MCP23S17;

void PortExpanderSPI::init(uint8_t CS, uint8_t addr, bool seqAddr, bool openDrain) {
	cs = CS;
	address = addr | 0x40;
	SPI.begin();
	uint8_t config = 0b10101100;
	if (seqAddr) {
		config |= 0b10000000;
		bank = 1;
	}
	else {
		config &= 0b01111111;
		bank = 0;
	}
	if (openDrain) {
		config |= 0b00000100;
	}
	else {
		config &= 0b11111011;
	}
	pinMode(cs, OUTPUT);
	digitalWrite(cs, HIGH);
	delay(5);
	write(registerBank[9][0], config);
}

void PortExpanderSPI::init(uint8_t CS, uint8_t addr) {
	cs = CS;
	address = addr | 0x40;
	bank = 1;
	Serial.println(address, BIN);
	SPI.begin();
	uint8_t config = 0b10101100;
	pinMode(cs, OUTPUT);
	digitalWrite(cs, HIGH);
	delay(5);
	write(registerBank[9][0], config);
}
void PortExpanderSPI::init(uint8_t CS) {
	cs = CS;
	address = 0x40;
	bank = 1;
	SPI.begin();
	uint8_t config = 0b10101100;
	pinMode(cs, OUTPUT);
	digitalWrite(cs, HIGH);
	delay(5);
	write(registerBank[9][0], config);
}

void PortExpanderSPI::setPortDirection(uint8_t port, uint8_t dir) {
	write(registerBank[port][bank], dir);
}

/*
void PortExpanderSPI::setPortDirection(uint8_t port, uint8_t dir, uint8_t pullup) {
	write(registerBank[port][bank], dir);
	write(registerBank[port + 11][bank], pullup);
}
*/

void PortExpanderSPI::setPortDirection(uint8_t port, uint8_t bit, uint8_t dir) {
	uint8_t b = bit;
	if (port == 1) {
		b -= 8;
	}
	uint8_t temp = read(registerBank[port][bank]);
	if (dir) {
		temp |= (0x01 << b);
	}
	else {
		temp &= (0xFE << b);
	}
	write(registerBank[port][bank], temp);
}

uint8_t PortExpanderSPI::readIO(uint8_t port) {
	return read(registerBank[port + 17][bank]);
}

bool PortExpanderSPI::readIO(uint8_t port, uint8_t bit) {
	uint8_t b = bit;
	if (port == 1) {
		b -= 8;
	}
	if ((read(registerBank[port + 17][bank]) >> b) == 0x01) {
		return true;
	}
	return false;
}

void PortExpanderSPI::writeIO(uint8_t port, uint8_t value) {
	write(registerBank[port + 17][bank], value);
}

void PortExpanderSPI::writeIO(uint8_t port, uint8_t bit, bool val) {
	uint8_t b = bit;
	if (port == 1) {
		b -= 8;
	}
	uint8_t temp = read(registerBank[port + 17][bank]);
	if (val) {
		temp |= (0x01 << b);
	}
	else {
		temp &= (0xFE << b);
	}
	write(registerBank[port + 17][bank], temp);
}



void PortExpanderSPI::write(uint8_t addr, uint8_t value) {
	digitalWrite(cs, LOW);
	SPI.transfer(address & _WRITE);
	SPI.transfer(addr);
	SPI.transfer(value);
	delay(5);
	digitalWrite(cs, HIGH);
}

uint8_t PortExpanderSPI::read(uint8_t addr) {
	digitalWrite(cs, LOW);
	SPI.transfer(address | _READ);
	SPI.transfer(addr);
	uint8_t readVal = SPI.transfer(0x00);
	digitalWrite(cs, HIGH);
	return readVal;
}
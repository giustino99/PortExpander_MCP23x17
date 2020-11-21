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

void PortExpanderSPI::portMode(uint8_t port, uint8_t dir) {
	write(registerBank[port][bank], dir);
}

void PortExpanderSPI::portMode(uint8_t port, uint8_t dir, uint8_t pullup) {
	write(registerBank[port][bank], dir);
	write(registerBank[port + 11][bank], pullup);
}

void PortExpanderSPI::pinMode(uint8_t pin, uint8_t mode) {
	uint8_t temp = read(registerBank[pin / 8][bank]);
	switch (mode) {
		case 0:			// INPUT
			write(registerBank[pin / 8][bank], (temp | (0x01 << (pin % 8))));
		break;

		case 1:			// OUTPUT
			write(registerBank[pin / 8][bank], (temp & (0xFE << (pin % 8))));
		break;

		case 2:			// INPUT_PULLUP
			write(registerBank[pin / 8][bank], (temp & (0xFE << (pin % 8))));
			temp = read(registerBank[(pin / 8) + 11][bank]);
			write(registerBank[(pin / 8) + 11][bank], (temp | (0x01 << (pin % 8))));
		break;
	}
}

uint8_t PortExpanderSPI::readPort(uint8_t port) {
	return read(registerBank[port + 17][bank]);
}

bool PortExpanderSPI::readPin(uint8_t pin) {
	if (((read(registerBank[(pin / 8) + 17][bank]) & (0x01 << (pin % 8))) >> (pin % 8)) == 0x01) {
		return true;
	}
	return false;
}

void PortExpanderSPI::writePort(uint8_t port, uint8_t value) {
	write(registerBank[port + 17][bank], value);
}

void PortExpanderSPI::writePin(uint8_t pin, uint8_t value) {
	uint8_t temp = read(registerBank[(pin / 8) + 17][bank]);
	if (value) {
		temp |= (0x01 << (pin % 8));
	}
	else {
		temp &= (0xFE << (pin % 8));
	}
	write(registerBank[(pin / 8) + 17][bank], temp);
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
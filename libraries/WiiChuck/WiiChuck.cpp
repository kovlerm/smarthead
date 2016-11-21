/*
  WiiChuck.cpp - Arduino/chipKit library support for the WiiChuck
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  This library has been made to easily interface and use the WiiChuck with
  an Arduino or chipKit.

  You can find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/
#include "WiiChuck.h"

// Include hardware-specific functions for the correct MCU
#if defined(__AVR__)
	#include "hardware/avr/HW_AVR.h"
#elif defined(__PIC32MX__)
  #include "hardware/pic32/HW_PIC32.h"
#elif defined(__arm__)
	#include "hardware/arm/HW_ARM.h"
#endif

/* Public */

WiiChuck::WiiChuck(uint8_t data_pin, uint8_t sclk_pin)
{
	_sda_pin = data_pin;
	_scl_pin = sclk_pin;
}

void WiiChuck::readData()
{
	_burstRead();
}

int WiiChuck::getJoyX()
{
	if (_dataarray[0] < _joy_x_center)
	{
		return -((_joy_x_center - _dataarray[0]) / (_joy_x_center / 100.0f));
	}
	else if (_dataarray[0] > _joy_x_center)
	{
		return ((_dataarray[0] - _joy_x_center) / ((255 - _joy_x_center) / 100.0f));
	}
	else
		return 0;
}

int WiiChuck::getJoyY()
{
	if (_dataarray[1] < _joy_y_center)
	{
		return -((_joy_y_center - _dataarray[1]) / (_joy_y_center / 100.0f));
	}
	else if (_dataarray[1] > _joy_y_center)
	{
		return ((_dataarray[1] - _joy_y_center) / ((255 - _joy_y_center) / 100.0f));
	}
	else
		return 0;
}

int WiiChuck::getRollAngle()
{
	return (int)(atan2((double)getAccelX(),(double)getAccelZ()) * 180.0 / PI);
}

int WiiChuck::getPitchAngle()
{
	return (int)(atan2((double)getAccelY(),(double)getAccelZ()) * 180.0 / PI);
}

int WiiChuck::getAccelX()
{
	return ((_dataarray[2] << 2) + ((_dataarray[5] & 0x0C) >> 2)) - 512;
}

int WiiChuck::getAccelY()
{
	return ((_dataarray[3] << 2) + ((_dataarray[5] & 0x30) >> 4)) - 512;
}

int WiiChuck::getAccelZ()
{
	return ((_dataarray[4] << 2) + ((_dataarray[5] & 0xC0) >> 6)) - 512;;
}

boolean WiiChuck::checkButtonC()
{
	return ((_dataarray[5] & 0x02)?false:true);
}

boolean WiiChuck::checkButtonZ()
{
	return ((_dataarray[5] & 0x01)?false:true);
}

/* Private */

void	WiiChuck::_sendStart(byte addr)
{
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_sda_pin, HIGH);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_sda_pin, LOW);
	digitalWrite(_scl_pin, LOW);
	shiftOut(_sda_pin, _scl_pin, MSBFIRST, addr);
}

void	WiiChuck::_sendStop()
{
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_sda_pin, LOW);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_sda_pin, HIGH);
	pinMode(_sda_pin, INPUT);
}

void	WiiChuck::_sendNack()
{
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_scl_pin, LOW);
	digitalWrite(_sda_pin, HIGH);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_scl_pin, LOW);
	pinMode(_sda_pin, INPUT);
}

void	WiiChuck::_sendAck()
{
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_scl_pin, LOW);
	digitalWrite(_sda_pin, LOW);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_scl_pin, LOW);
	pinMode(_sda_pin, INPUT);
}

void	WiiChuck::_waitForAck()
{
	pinMode(_sda_pin, INPUT);
	digitalWrite(_scl_pin, HIGH);
	while (digitalRead(_sda_pin)==HIGH) {}
	digitalWrite(_scl_pin, LOW);
}

uint8_t WiiChuck::_readByte()
{
	pinMode(_sda_pin, INPUT);

	uint8_t value = 0;
	uint8_t currentBit = 0;

	for (int i = 0; i < 8; ++i)
	{
		digitalWrite(_scl_pin, HIGH);
		currentBit = digitalRead(_sda_pin);
		value |= (currentBit << 7-i);
		delayMicroseconds(1);
		digitalWrite(_scl_pin, LOW);
	}
	return value;
}

void WiiChuck::_writeByte(uint8_t value)
{
	pinMode(_sda_pin, OUTPUT);
	shiftOut(_sda_pin, _scl_pin, MSBFIRST, value);
}


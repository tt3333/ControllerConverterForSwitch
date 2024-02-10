/**
 * Controller Converter for Nintendo Switch
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2023 T.T
 */

#include <Wire.h>

#include "config.h"
#include "Wii.h"

void Wii::init()
{
	Wire.setSDA(I2C_SDA_PIN);
	Wire.setSCL(I2C_SCL_PIN);
	Wire.setClock(I2C_CLOCK);
	clearInputData();
}

bool Wii::setFormat()
{
	uint8_t buffer[6];

	// When Wii controller is unplugged and reconnected,
	// writeRegister may repeatedly timeout if the I2C controller is not reinitialized.
	Wire.end();
	Wire.begin();

	// disable encryption
	if (writeRegister(0xF0, 0x55))
	{
		// set data format
		if (writeRegister(0xFB, 0x00) && writeRegister(0xFE, 0x03))
		{
			// get controller type
			if (readRegister(0xFA, buffer, sizeof(buffer)) == sizeof(buffer))
			{
				// verify data format
				if (buffer[4] == 3)
				{
					return true;
				}
			}
		}
	}

	clearInputData();
	return false;
}

bool Wii::calibrate()
{
	uint8_t buffer[4];

	// In the Wii RetroPad Adapter (https://github.com/bootsector/wii-retropad-adapter) and its derivative projects,
	// the first read after a data format change sends data in the format before the change.
	// Therefore, dummy read once and then read again.
	//
	// Note that Classic Controller Pico (https://github.com/tt3333/ClassicControllerPico)
	// is based on the Wii RetroPad Adapter, but version 1.0.1 fixes this issue.
	if (readRegister(0x00, buffer, sizeof(buffer)) != sizeof(buffer))
	{
		return false;
	}
	if (readRegister(0x00, buffer, sizeof(buffer)) != sizeof(buffer))
	{
		return false;
	}

	calib_lx = buffer[0] - 0x80;
	calib_rx = buffer[1] - 0x80;
	calib_ly = buffer[2] - 0x80;
	calib_ry = buffer[3] - 0x80;

	return true;
}

bool Wii::getInput()
{
	// Read 10 bytes to be correctly recognized by RetroSpy.
	uint8_t buffer[10];

	if (readRegister(0x00, buffer, sizeof(buffer)) != sizeof(buffer))
	{
		clearInputData();
		return false;
	}

	buttons = ~(buffer[7] << 8 | buffer[6]);
	lx = buffer[0] - calib_lx;
	rx = buffer[1] - calib_rx;
	ly = 255 - buffer[2] + calib_ly;
	ry = 255 - buffer[3] + calib_ry;

	return true;
}

void Wii::clearInputData()
{
	buttons = 0;
	lx = 0x80;
	rx = 0x80;
	ly = 0x80;
	ry = 0x80;
}

bool Wii::writeRegister(uint8_t reg, uint8_t value)
{
	delayMicroseconds(I2C_WRITE_WAIT);
	Wire.beginTransmission(I2C_ADDRESS);
	Wire.write(reg);
	Wire.write(value);
	if (Wire.endTransmission() != 0)
	{
		DEBUG_PRINTF("%02X : endTransmission failed.\r\n", reg);
		return false;
	}
	return true;
}

size_t Wii::readRegister(uint8_t reg, uint8_t* buffer, size_t size)
{
	delayMicroseconds(I2C_WRITE_WAIT);
	Wire.beginTransmission(I2C_ADDRESS);
	Wire.write(reg);
	if (Wire.endTransmission() != 0)
	{
		DEBUG_PRINTF("%02X : endTransmission failed.\r\n", reg);
		return 0;
	}

	delayMicroseconds(I2C_READ_WAIT);
	size = Wire.requestFrom(I2C_ADDRESS, size);
	if (size == 0)
	{
		DEBUG_PRINTF("%02X : requestFrom failed.\r\n", reg);
		return 0;
	}

	size = Wire.readBytes(buffer, size);
	if (size == 0)
	{
		DEBUG_PRINTF("%02X : readBytes failed.\r\n", reg);
		return 0;
	}

#ifdef DEBUG
	DEBUG_PRINTF("%02X : ", reg);
	for (int i = 0; i < size; i++)
	{
		DEBUG_PRINTF("%02X ", buffer[i]);
	}
	DEBUG_WRITE("\r");
	DEBUG_WRITE("\n");
#endif

	return size;
}

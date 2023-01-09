/**
 * Controller Converter for Nintendo Switch
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2023 T.T
 */

#pragma once

class Wii
{
public:
	uint16_t buttons;
	uint8_t lx;
	uint8_t ly;
	uint8_t rx;
	uint8_t ry;

	void init();
	bool setFormat();
	bool getInput();

private:
	void clearInputData();
	bool writeRegister(uint8_t reg, uint8_t value);
	size_t readRegister(uint8_t reg, uint8_t* buffer, size_t size);
};

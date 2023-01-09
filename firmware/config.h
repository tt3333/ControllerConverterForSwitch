/**
 * Controller Converter for Nintendo Switch
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2023 T.T
 */

#pragma once

#define WII_DETECT_PIN	14
#define MODE_LR_PIN		15		// Replace L+R with ZL+ZR (Low = Enable, HIGH = Disable)
#define TEST_PIN		20		// Test pin for measuring processing time with an oscilloscope

#define I2C_SDA_PIN		12
#define I2C_SCL_PIN 	13
#define I2C_CLOCK		250000	// speed that can be read in 1 ms cycle
#define I2C_ADDRESS		0x52
#define I2C_WRITE_WAIT	20
#define I2C_READ_WAIT	150

#define SPI_RX_PIN		16
#define SPI_CS_PIN		17
#define SPI_SCK_PIN		18
#define SPI_TX_PIN		19
#define SPI_CLOCK		83895	// equivalent to SNES

//#define DEBUG

#ifdef DEBUG
	#define DEBUG_BEGIN()		Serial1.begin(9600)
	#define DEBUG_PRINTF(...)	Serial1.printf(__VA_ARGS__)
	#define DEBUG_WRITE(c)		Serial1.write(c)
#else
	#define DEBUG_BEGIN()
	#define DEBUG_PRINTF(...)
	#define DEBUG_WRITE(c)
#endif

/**
 * Controller Converter for Nintendo Switch
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2023 T.T
 */

#include <SPI.h>

#include "config.h"
#include "SNES.h"

static const SPISettings spiSettings(SPI_CLOCK, MSBFIRST, SPI_MODE2);

void SNES::init()
{
	gpio_pull_up(SPI_RX_PIN);
	gpio_pull_up(SPI_CS_PIN);
	gpio_pull_up(SPI_SCK_PIN);
	gpio_pull_up(SPI_TX_PIN);

	SPI.setRX(SPI_RX_PIN);
	SPI.setCS(SPI_CS_PIN);
	SPI.setSCK(SPI_SCK_PIN);
	SPI.setTX(SPI_TX_PIN);
	SPI.begin(true);
	SPI.beginTransaction(spiSettings);
}

uint16_t SNES::getInput()
{
	return ~SPI.transfer16(0xFFFF);
}

/**
 * Controller Converter for Nintendo Switch
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2023 T.T
 */

#include "config.h"
#include "usb.h"
#include "buttons.h"
#include "SNES.h"
#include "Wii.h"

static SNES snes;
static Wii wii;

static bool isWiiControllerInitialized = false;
static volatile bool sendFlag = false;

static const uint16_t snes_to_wii_buttons[] =
{
	SNES_MASK_R,		WII_MASK_R,
	SNES_MASK_L,		WII_MASK_L,
	SNES_MASK_X,		WII_MASK_X,
	SNES_MASK_A,		WII_MASK_A,
	SNES_MASK_RIGHT,	WII_MASK_RIGHT,
	SNES_MASK_LEFT,		WII_MASK_LEFT,
	SNES_MASK_DOWN,		WII_MASK_DOWN,
	SNES_MASK_UP,		WII_MASK_UP,
	SNES_MASK_START,	WII_MASK_PLUS,
	SNES_MASK_SELECT,	WII_MASK_MINUS,
	SNES_MASK_Y,		WII_MASK_Y,
	SNES_MASK_B,		WII_MASK_B,
};

static const uint16_t wii_to_switch_buttons[] =
{
	WII_MASK_R,			SWITCH_MASK_R,
	WII_MASK_PLUS,		SWITCH_MASK_PLUS,
	WII_MASK_HOME,		SWITCH_MASK_HOME,
	WII_MASK_MINUS,		SWITCH_MASK_MINUS,
	WII_MASK_L,			SWITCH_MASK_L,
	WII_MASK_ZR,		SWITCH_MASK_ZR,
	WII_MASK_X,			SWITCH_MASK_X,
	WII_MASK_A,			SWITCH_MASK_A,
	WII_MASK_Y,			SWITCH_MASK_Y,
	WII_MASK_B,			SWITCH_MASK_B,
	WII_MASK_ZL,		SWITCH_MASK_ZL,
};

static const uint8_t wii_to_switch_dpad[] =
{
	SWITCH_HAT_NOTHING,		// ----
	SWITCH_HAT_DOWN,		// ---D
	SWITCH_HAT_RIGHT,		// --R-
	SWITCH_HAT_DOWN_RIGHT,	// --RD
	SWITCH_HAT_UP,			// -U--
	SWITCH_HAT_NOTHING,		// -U-D
	SWITCH_HAT_UP_RIGHT,	// -UR-
	SWITCH_HAT_RIGHT,		// -URD
	SWITCH_HAT_LEFT,		// L---
	SWITCH_HAT_DOWN_LEFT,	// L--D
	SWITCH_HAT_NOTHING,		// L-R-
	SWITCH_HAT_DOWN,		// L-RD
	SWITCH_HAT_UP_LEFT,		// LU--
	SWITCH_HAT_LEFT,		// LU-D
	SWITCH_HAT_UP,			// LUR-
	SWITCH_HAT_NOTHING,		// LURD
};

static uint16_t convertButtons(uint16_t data, const uint16_t* table, int num)
{
	uint16_t ret = 0;

	for (int i = 0; i < num; i += 2)
	{
		if (data & table[i])
		{
			ret |= table[i + 1];
		}
	}

	return ret;
}

void setup()
{
	pinMode(MODE_LR_PIN, INPUT_PULLDOWN);
	pinMode(LED_BUILTIN, OUTPUT);

	tusb_init();
	snes.init();
}

void setup1()
{
	pinMode(WII_DETECT_PIN, INPUT_PULLDOWN);
	pinMode(TEST_PIN, OUTPUT);

	DEBUG_BEGIN();
	wii.init();
}

void loop()
{
	uint16_t buttons = snes.getInput();
	buttons = convertButtons(buttons, snes_to_wii_buttons, TU_ARRAY_SIZE(snes_to_wii_buttons));
	buttons |= wii.buttons;

	digitalWrite(LED_BUILTIN, buttons ? HIGH : LOW);

	if (digitalRead(MODE_LR_PIN) == HIGH)
	{
		uint16_t lr = buttons & (WII_MASK_L | WII_MASK_R);
		if (lr == (WII_MASK_L | WII_MASK_R))
		{
			buttons &= ~(WII_MASK_L | WII_MASK_R);
			buttons |= (WII_MASK_ZL | WII_MASK_ZR);
		}
	}

	report_t report;
	report.buttons = convertButtons(buttons, wii_to_switch_buttons, TU_ARRAY_SIZE(wii_to_switch_buttons));
	report.hat     = wii_to_switch_dpad[(buttons >> 6) & 0x0F];
	report.lx      = wii.lx;
	report.ly      = wii.ly;
	report.rx      = wii.rx;
	report.ry      = wii.ry;
	report.vendor  = 0;

	tud_task();
	send_report(&report);
	sendFlag = false;
}

void loop1()
{
	while (sendFlag)
	{
	}

	if (digitalRead(WII_DETECT_PIN) == HIGH)
	{
		if (!isWiiControllerInitialized)
		{
			isWiiControllerInitialized = wii.setFormat();
		}
		if (isWiiControllerInitialized)
		{
			digitalWrite(TEST_PIN, HIGH);
			isWiiControllerInitialized = wii.getInput();
			digitalWrite(TEST_PIN, LOW);
		}
	}
	else
	{
		isWiiControllerInitialized = false;
	}

	sendFlag = true;
}

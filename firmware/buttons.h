/**
 * Controller Converter for Nintendo Switch
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2023 T.T
 */

#pragma once

#define SNES_MASK_R			(1U <<  4)
#define SNES_MASK_L			(1U <<  5)
#define SNES_MASK_X			(1U <<  6)
#define SNES_MASK_A			(1U <<  7)
#define SNES_MASK_RIGHT		(1U <<  8)
#define SNES_MASK_LEFT		(1U <<  9)
#define SNES_MASK_DOWN		(1U << 10)
#define SNES_MASK_UP		(1U << 11)
#define SNES_MASK_START		(1U << 12)
#define SNES_MASK_SELECT	(1U << 13)
#define SNES_MASK_Y			(1U << 14)
#define SNES_MASK_B			(1U << 15)

#define WII_MASK_R			(1U <<  1)
#define WII_MASK_PLUS		(1U <<  2)
#define WII_MASK_HOME		(1U <<  3)
#define WII_MASK_MINUS		(1U <<  4)
#define WII_MASK_L			(1U <<  5)
#define WII_MASK_DOWN		(1U <<  6)
#define WII_MASK_RIGHT		(1U <<  7)
#define WII_MASK_UP			(1U <<  8)
#define WII_MASK_LEFT		(1U <<  9)
#define WII_MASK_ZR			(1U << 10)
#define WII_MASK_X			(1U << 11)
#define WII_MASK_A			(1U << 12)
#define WII_MASK_Y			(1U << 13)
#define WII_MASK_B			(1U << 14)
#define WII_MASK_ZL			(1U << 15)

#define SWITCH_MASK_Y		(1U <<  0)
#define SWITCH_MASK_B		(1U <<  1)
#define SWITCH_MASK_A		(1U <<  2)
#define SWITCH_MASK_X		(1U <<  3)
#define SWITCH_MASK_L		(1U <<  4)
#define SWITCH_MASK_R		(1U <<  5)
#define SWITCH_MASK_ZL		(1U <<  6)
#define SWITCH_MASK_ZR		(1U <<  7)
#define SWITCH_MASK_MINUS	(1U <<  8)
#define SWITCH_MASK_PLUS	(1U <<  9)
#define SWITCH_MASK_L3		(1U << 10)
#define SWITCH_MASK_R3		(1U << 11)
#define SWITCH_MASK_HOME	(1U << 12)
#define SWITCH_MASK_CAPTURE	(1U << 13)

enum
{
	SWITCH_HAT_UP,
	SWITCH_HAT_UP_RIGHT,
	SWITCH_HAT_RIGHT,
	SWITCH_HAT_DOWN_RIGHT,
	SWITCH_HAT_DOWN,
	SWITCH_HAT_DOWN_LEFT,
	SWITCH_HAT_LEFT,
	SWITCH_HAT_UP_LEFT,
	SWITCH_HAT_NOTHING,
};

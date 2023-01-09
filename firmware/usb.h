/**
 * Controller Converter for Nintendo Switch
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2023 T.T
 */

#pragma once

#include "tusb_config.h"
#include <tusb.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct TU_ATTR_PACKED
{
	uint16_t	buttons;
	uint8_t		hat;
	uint8_t		lx;
	uint8_t		ly;
	uint8_t		rx;
	uint8_t		ry;
	uint8_t		vendor;
} report_t;

void send_report(const report_t* report);

#ifdef __cplusplus
 }
#endif

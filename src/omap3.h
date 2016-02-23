/*
 * Copyright (C) 2016 Paul Kocialkowski <contact@paulk.fr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _OMAP3_H_
#define _OMAP3_H_

#include <stdint.h>

#include "omap-usb-boot.h"

/*
 * Values
 */

#define USB_DEVICE_OMAP34XX					0xd009
#define USB_DEVICE_OMAP36XX					0xd00e

/*
 * Structures
 */

struct omap3_asic_id {
	uint8_t count;
	uint8_t device_id[7];
	uint32_t reserved;
	uint8_t id[23];
	uint8_t reserved_id[23];
	uint8_t crc[11];
} __attribute__((__packed__));

/*
 * Globals
 */

extern struct omap_description omap3_description;

#endif

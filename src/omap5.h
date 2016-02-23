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

#ifndef _OMAP5_H_
#define _OMAP5_H_

#include "omap-usb-boot.h"

/*
 * Values
 */

#define USB_DEVICE_OMAP5430					0xd011
#define USB_DEVICE_AM572X					0xd013

/*
 * Structures
 */

struct omap5_asic_id_device_id {
	uint8_t subblock_id;
	uint8_t subblock_size;
	uint8_t fixed;
	uint16_t device;
	uint8_t ch;
	uint8_t rom_revision;
} __attribute__((__packed__));

struct omap5_asic_id_crc {
	uint8_t subblock_id;
	uint8_t subblock_size;
	uint8_t fixed;
	uint32_t crc;
	uint32_t gp_blank;
} __attribute__((__packed__));

struct omap5_asic_id {
	uint8_t count;
	struct omap5_asic_id_device_id device_id;
	uint8_t reserved[62];
	struct omap5_asic_id_crc crc;
} __attribute__((__packed__));

/*
 * Globals
 */

extern struct omap_description omap5_description;

#endif

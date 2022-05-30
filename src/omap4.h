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

#ifndef _OMAP4_H_
#define _OMAP4_H_

#include <stdint.h>

#include "omap-usb-boot.h"

/*
 * Values
 */

#define USB_DEVICE_OMAP4430					0xd00f
#define USB_DEVICE_OMAP4460					0xd010

/*
 * Structures
 */

struct omap4_asic_id_device_id {
	uint8_t subblock_id;
	uint8_t subblock_size;
	uint8_t fixed;
	uint16_t device;
	uint8_t ch;
	uint8_t rom_revision;
} __attribute__((__packed__));

struct omap4_asic_id_crc {
	uint8_t subblock_id;
	uint8_t subblock_size;
	uint8_t fixed;
	uint32_t crc0;
	uint32_t crc1;
} __attribute__((__packed__));

struct omap4_asic_id_reserved1 {
	uint8_t subblock_id;
	uint8_t subblock_size;
	uint8_t fixed;
	uint8_t data;
} __attribute__((__packed__));

struct omap4_asic_id_reserved2 {
	uint8_t subblock_id;
	uint8_t subblock_size;
	uint8_t fixed;
	uint8_t data[20];
} __attribute__((__packed__));

struct omap4_asic_id_reserved3 {
	uint8_t subblock_id;
	uint8_t subblock_size;
	uint8_t fixed;
	uint8_t data[32];
} __attribute__((__packed__));

struct omap4_asic_id {
	uint8_t count;
	struct omap4_asic_id_device_id device_id;
	struct omap4_asic_id_reserved1 reserved1;
	struct omap4_asic_id_reserved2 reserved2;
	struct omap4_asic_id_reserved3 reserved3;
	struct omap4_asic_id_crc crc;
} __attribute__((__packed__));

/*
 * Globals
 */

extern struct omap_description omap4_description;

#endif

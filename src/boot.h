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

#ifndef _BOOT_H_
#define _BOOT_H_

#include "omap-usb-boot.h"

/*
 * Values
 */

#define BOOT_PERIPHERAL						0xf0030002
#define BOOT_ASIC_ID						0xf0030003
#define BOOT_DEVICE						0xf0030006
#define BOOT_NEXT						0xffffffff
#define BOOT_MEMORY						0xdeadbeef

/*
 * Functions
 */

int boot_peripheral(struct context *context, size_t length);
int boot_asic_id(struct context *context);
int boot_device(struct context *context, uint8_t value);
int boot_next(struct context *context);
int boot_memory(struct context *context);

#endif

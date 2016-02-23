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

#include <stdlib.h>
#include <stdio.h>
#include <endian.h>

#include "omap-usb-boot.h"
#include "boot.h"
#include "usb.h"
#include "omap5.h"

struct usb_device_id omap5_usb_device_ids[] = {
	{ USB_VENDOR_TI, USB_DEVICE_OMAP5430 },
	{ USB_VENDOR_TI, USB_DEVICE_AM572X }
};

struct omap_boot_device omap5_boot_devices[] = {
	{ "xip",	0x01 },
	{ "xipwait",	0x02 },
	{ "nand",	0x03 },
	{ "onenand",	0x04 },
	{ "sdcard",	0x05 },
	{ "emmcboot",	0x06 },
	{ "emmc",	0x07 },
	{ "sata",	0x09 },
	{ "qspi1",	0x0a },
	{ "qspi4",	0x0b },
	{ "uart",	0x43 },
	{ "usb",	0x45 }
};

int omap5_boot_asic_id(struct context *context)
{
	struct omap5_asic_id asic_id;
	uint32_t command;
	int rc;

	if (context == NULL)
		return -1;

	command = htole32(BOOT_ASIC_ID);

	rc = usb_send(context, &command, sizeof(command));
	if (rc < 0)
		return -1;

	rc = usb_recv(context, &asic_id, sizeof(asic_id));
	if (rc < 0)
		return -1;

	if (context->verbose)
		printf("ASIC device id: %04x, %s device\n", be16toh(asic_id.device_id.device), asic_id.crc.gp_blank == 0 ? "GP" : "HS");

	return 0;
}

struct omap_description omap5_description = {
	.name = "omap5",
	.usb_device_ids = omap5_usb_device_ids,
	.usb_device_ids_count = sizeof(omap5_usb_device_ids) / sizeof(struct usb_device_id),
	.boot_devices = omap5_boot_devices,
	.boot_devices_count = sizeof(omap5_boot_devices) / sizeof(struct omap_boot_device),
	.boot_asic_id = omap5_boot_asic_id
};

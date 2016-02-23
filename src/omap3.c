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
#include <endian.h>

#include "omap-usb-boot.h"
#include "boot.h"
#include "usb.h"
#include "omap3.h"

struct usb_device_id omap3_usb_device_ids[] = {
	{ USB_VENDOR_TI, USB_DEVICE_OMAP34XX },
	{ USB_VENDOR_TI, USB_DEVICE_OMAP36XX }
};

struct omap_boot_device omap3_boot_devices[] = {
	{ "void",	0x00 },
	{ "xip",	0x01 },
	{ "nand",	0x02 },
	{ "onenand",	0x03 },
	{ "doc",	0x04 },
	{ "mmc2",	0x05 },
	{ "mmc1",	0x06 },
	{ "xipwait",	0x07 },
	{ "mmc2hybrid",	0x08 },
	{ "uart",	0x10 },
	{ "usb",	0x11 }
};

int omap3_boot_asic_id(struct context *context)
{
	struct omap3_asic_id asic_id;
	uint32_t command;
	int rc;

	if (context == NULL)
		return -1;

	rc = usb_recv(context, &asic_id, sizeof(asic_id));
	if (rc < 0) {
		command = htole32(BOOT_ASIC_ID);

		rc = usb_send(context, &command, sizeof(command));
		if (rc < 0)
			return -1;

		rc = usb_recv(context, &asic_id, sizeof(asic_id));
		if (rc < 0)
			return -1;
	}

	return 0;
}

struct omap_description omap3_description = {
	.name = "omap3",
	.usb_device_ids = omap3_usb_device_ids,
	.usb_device_ids_count = sizeof(omap3_usb_device_ids) / sizeof(struct usb_device_id),
	.boot_devices = omap3_boot_devices,
	.boot_devices_count = sizeof(omap3_boot_devices) / sizeof(struct omap_boot_device),
	.boot_asic_id = omap3_boot_asic_id
};

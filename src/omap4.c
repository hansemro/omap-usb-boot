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
#include "omap4.h"

struct usb_device_id omap4_usb_device_ids[] = {
	{ USB_VENDOR_TI, USB_DEVICE_OMAP4430 },
	{ USB_VENDOR_TI, USB_DEVICE_OMAP4460 }
};

struct omap_boot_device omap4_boot_devices[] = {
	{ "void",	0x00 },
	{ "xip",	0x01 },
	{ "xipwait",	0x02 },
	{ "nand",	0x03 },
	{ "onenand",	0x04 },
	{ "mmc1",	0x05 },
	{ "mmc2(1)",	0x06 },
	{ "mmc2(2)",	0x07 },
	{ "uart",	0x43 },
	{ "usb1",	0x45 },
	{ "usbulpi",	0x46 },
	{ "usb2",	0x47 }
};

int omap4_boot_asic_id(struct context *context)
{
	struct omap4_asic_id asic_id;
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

	printf("ASIC device id: %04x, %s device\n", be16toh(asic_id.device_id.device), asic_id.crc.crc1 == 0 ? "GP" : "HS");
	if (context->verbose) {
		printf("Number of subblocks: %u\n", asic_id.count);
		printf("ID Subblock:\n");
		printf("\tSubblock ID: 0x%02x\n", asic_id.device_id.subblock_id);
		printf("\tSubblock size: 0x%02x\n", asic_id.device_id.subblock_size);
		printf("\tFixed size: 0x%02x\n", asic_id.device_id.fixed);
		printf("\tDevice: 0x%04x\n", be16toh(asic_id.device_id.device));
		printf("\tCH: 0x%02x\n", asic_id.device_id.ch);
		printf("\tROM revision: 0x%02x\n", asic_id.device_id.rom_revision);
		printf("UNK1 Subblock:\n");
		printf("\tSubblock ID: 0x%02x\n", asic_id.reserved1.subblock_id);
		printf("\tSubblock size: 0x%02x\n", asic_id.reserved1.subblock_size);
		printf("\tFixed size: 0x%02x\n", asic_id.reserved1.fixed);
		printf("\tdata: 0x%02x\n", asic_id.reserved1.data);
		printf("UNK2 Subblock:\n");
		printf("\tSubblock ID: 0x%02x\n", asic_id.reserved2.subblock_id);
		printf("\tSubblock size: 0x%02x\n", asic_id.reserved2.subblock_size);
		printf("\tFixed size: 0x%02x\n", asic_id.reserved2.fixed);
		printf("\tdata: 0x", asic_id.reserved2.data);
		for (int i = 0; i < 20; i++) {
			printf("%02x", asic_id.reserved2.data[20-i]);
		}
		printf("\nUNK3 Subblock:\n");
		printf("\tSubblock ID: 0x%02x\n", asic_id.reserved3.subblock_id);
		printf("\tSubblock size: 0x%02x\n", asic_id.reserved3.subblock_size);
		printf("\tFixed size: 0x%02x\n", asic_id.reserved3.fixed);
		printf("\tdata: 0x");
		for (int i = 0; i < 32; i++) {
			printf("%02x", asic_id.reserved3.data[32-i]);
		}
		printf("\nCRC Subblock:\n");
		printf("\tSubblock ID: 0x%02x\n", asic_id.crc.subblock_id);
		printf("\tSubblock size: 0x%02x\n", asic_id.crc.subblock_size);
		printf("\tFixed size: 0x%02x\n", asic_id.crc.fixed);
		printf("\tCRC0: 0x%08x\n", be32toh(asic_id.crc.crc0));
		printf("\tCRC1: 0x%08x\n", be32toh(asic_id.crc.crc1));
	}

	return 0;
}

struct omap_description omap4_description = {
	.name = "omap4",
	.usb_device_ids = omap4_usb_device_ids,
	.usb_device_ids_count = sizeof(omap4_usb_device_ids) / sizeof(struct usb_device_id),
	.boot_devices = omap4_boot_devices,
	.boot_devices_count = sizeof(omap4_boot_devices) / sizeof(struct omap_boot_device),
	.boot_asic_id = omap4_boot_asic_id
};

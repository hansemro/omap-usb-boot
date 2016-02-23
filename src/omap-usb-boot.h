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

#ifndef _OMAP_USB_LOADER_H_
#define _OMAP_USB_LOADER_H_

#include <stdint.h>
#include <sys/types.h>
#include <libusb.h>

/*
 * Structures
 */

struct usb_device_id;
struct context;

struct omap_boot_device {
	char *name;
	uint8_t value;
};

struct omap_description {
	char *name;

	struct usb_device_id *usb_device_ids;
	size_t usb_device_ids_count;

	struct omap_boot_device *boot_devices;
	size_t boot_devices_count;

	int (*boot_asic_id)(struct context *context);
};

struct context {
	struct libusb_device_handle *usb_handle;
	int verbose;
	int wait;
	char operation;

	char *device;
	char *filename;

	struct omap_description *description;
};

#endif

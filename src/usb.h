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

#ifndef _USB_H_
#define _USB_H_

#include <stdint.h>
#include <sys/types.h>

#include "omap-usb-boot.h"

/*
 * Values
 */

#define USB_VENDOR_TI						0x0451

#define USB_CONFIGURATION					1
#define USB_INTERFACE						0

#define USB_ENDPOINT_OUT					0x01
#define USB_ENDPOINT_IN						0x81

#define USB_TIMEOUT						100
#define USB_SEND_CHUNK						512
#define USB_RECV_CHUNK						512

/*
 * Structures
 */

struct usb_device_id {
	uint16_t vendor_id;
	uint16_t product_id;
};

/*
 * Functions
 */

int usb_open(struct context *context, struct omap_description **descriptions, size_t descriptions_count);
void usb_close(struct context *context);
int usb_send(struct context *context, const void *data, size_t size);
int usb_recv(struct context *context, void *data, size_t size);
int usb_recv_available(struct context *context, void *data, size_t size);

#endif

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
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <libusb.h>

#include "omap-usb-boot.h"
#include "usb.h"

int usb_open(struct context *context, struct omap_description **descriptions, size_t descriptions_count)
{
	struct libusb_device **list = NULL;
	struct libusb_device_handle *handle = NULL;
	struct libusb_device_descriptor descriptor;
	int configuration;
	char device_name[32];
	size_t count;
	size_t i;
	size_t j;
	size_t k;
	int rc;

	if (context == NULL)
		return -1;

	rc = libusb_init(NULL);
	if (rc < 0) {
		fprintf(stderr, "Initializing USB failed\n");
		return -1;
	}

	do {
		rc = libusb_get_device_list(NULL, &list);
		if (rc < 0) {
			fprintf(stderr, "Getting USB device list failed\n");
			goto error;
		}

		count = (size_t) rc;

		for (i = 0; i < count; i++) {
			rc = libusb_get_device_descriptor(list[i], &descriptor);
			if (rc < 0) {
				fprintf(stderr, "Getting USB device descriptor failed\n");
				goto error;
			}

			for (j = 0; j < descriptions_count; j++) {
				for (k = 0; k < descriptions[j]->usb_device_ids_count; k++) {
					if (descriptions[j]->usb_device_ids[k].vendor_id == descriptor.idVendor && descriptions[j]->usb_device_ids[k].product_id == descriptor.idProduct) {
						rc = libusb_open(list[i], &handle);
						if (rc < 0) {
							fprintf(stderr, "Opening USB device failed\n");
							fprintf(stderr, "\nYou may not have the rights to do this\n");
							goto error;
						}

						rc = libusb_get_string_descriptor_ascii(handle, descriptor.iProduct, device_name, sizeof(device_name));
						if (rc < 0) {
							fprintf(stderr, "Getting USB product string descriptor failed\n");
							goto error;
						}

						if (context->verbose)
							printf("Found and opened %s USB device: %s\n", descriptions[j]->name, device_name);

						context->description = descriptions[j];

						goto usb_configure;
					}
				}
			}
		}

		usleep(300000);
	} while (context->wait);

usb_configure:
	if (handle == NULL) {
		fprintf(stderr, "Finding USB device failed\n");
		goto error;
	}

	libusb_free_device_list(list, 1);
	list = NULL;

	rc = libusb_get_configuration(handle, &configuration);
	if (rc < 0) {
		fprintf(stderr, "Getting USB configuration failed\n");
		goto error;
	}

	if (configuration != USB_CONFIGURATION) {
		rc = libusb_set_configuration(handle, USB_CONFIGURATION);
		if (rc < 0) {
			fprintf(stderr, "Setting USB configuration failed\n");
			goto error;
		}
	}

	rc = libusb_claim_interface(handle, USB_INTERFACE);
	if (rc < 0) {
		fprintf(stderr, "Claiming USB interface failed\n");
		goto error;
	}

	context->usb_handle = handle;

	rc = 0;
	goto complete;

error:
	rc = -1;

	if (list != NULL)
		libusb_free_device_list(list, 1);

	if (handle != NULL)
		libusb_close(handle);

	libusb_exit(NULL);

complete:
	return rc;
}

void usb_close(struct context *context)
{
	if (context == NULL)
		return;

	if (context->usb_handle != NULL) {
		libusb_release_interface(context->usb_handle, USB_INTERFACE);
		libusb_close(context->usb_handle);

		context->usb_handle = NULL;

		libusb_exit(NULL);
	}
}

int usb_send(struct context *context, const void *data, size_t size)
{
	size_t count;
	size_t chunk;
	uint8_t *p;
	int transferred;
	int rc;

	if (context == NULL)
		return -1;

	count = 0;
	p = (uint8_t *) data;

	while (count < size) {
		chunk = (size - count) < USB_SEND_CHUNK ? (size - count) : USB_SEND_CHUNK;

		rc = libusb_bulk_transfer(context->usb_handle, USB_ENDPOINT_OUT, p, chunk, &transferred, USB_TIMEOUT);
		if (rc < 0 || transferred <= 0) {
			fprintf(stderr, "Bulk USB transfer failed\n");
			return -1;
		}

		count += transferred;
		p += transferred;
	}

	return 0;
}

int usb_recv(struct context *context, void *data, size_t size)
{
	size_t count;
	size_t chunk;
	uint8_t *p;
	int transferred;
	int rc;

	if (context == NULL)
		return -1;

	count = 0;
	p = (uint8_t *) data;

	while (count < size) {
		chunk = (size - count) < USB_RECV_CHUNK ? (size - count) : USB_RECV_CHUNK;

		rc = libusb_bulk_transfer(context->usb_handle, USB_ENDPOINT_IN, p, chunk, &transferred, USB_TIMEOUT);
		if (rc < 0 || transferred <= 0) {
			fprintf(stderr, "Bulk USB transfer failed\n");
			return -1;
		}

		count += transferred;
		p += transferred;
	}

	return 0;
}

int usb_recv_available(struct context *context, void *data, size_t size)
{
	size_t chunk;
	int transferred;
	int rc;

	if (context == NULL)
		return -1;

	chunk = size < USB_RECV_CHUNK ? size : USB_RECV_CHUNK;

	rc = libusb_bulk_transfer(context->usb_handle, USB_ENDPOINT_IN, data, chunk, &transferred, USB_TIMEOUT);
printf("%d %d\n", rc, transferred);
	if (rc < 0 || transferred <= 0) {
		fprintf(stderr, "Bulk USB transfer failed\n");
		return -1;
	}

	return transferred;
}

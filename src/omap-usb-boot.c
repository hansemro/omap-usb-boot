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
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "omap-usb-boot.h"
#include "boot.h"
#include "omap3.h"
#include "omap4.h"
#include "omap5.h"
#include "usb.h"

static struct omap_description *omap_descriptions[] = {
	&omap3_description,
	&omap4_description,
	&omap5_description
};

static size_t omap_descriptions_count = sizeof(omap_descriptions) / sizeof(struct omap_description *);

static int usage_print(void)
{
	size_t i;
	size_t j;

	printf("Usage: omap-usb-boot [OPTIONS] [OPERATION]\n\n"
		"Options:\n"
		" -h                             help\n"
		" -v                             verbose\n"
		" -w                             wait for device\n"
		" -s                             stall and fetch asic id forever\n\n"
		"Operations:\n"
		" next                           skip to the next booting device\n"
		" memory                         skip to the first memory booting device\n"
		" boot [device]                  boot from device\n"
		" load [filename]                load and execute a file\n\n"
		"Devices:\n");

	for (i = 0; i < omap_descriptions_count; i++) {
		printf(" %s: ", omap_descriptions[i]->name);

		for (j = 0; j < omap_descriptions[i]->boot_devices_count; j++)
			printf("%s%c", omap_descriptions[i]->boot_devices[j].name, (j == (omap_descriptions[i]->boot_devices_count - 1)) ? '\n' : ' ');
	}
}

static int arguments_parse(struct context *context, int argc, char *argv[])
{
	int i;

	if (context == NULL || argc < 2)
		return -1;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "next") == 0) {
			if (context->operation)
				return -1;

			context->operation = 'n';
		} else if (strcmp(argv[i], "memory") == 0) {
			if (context->operation)
				return -1;

			context->operation = 'm';
		} else if (strcmp(argv[i], "boot") == 0) {
			if (context->operation || argc <= i)
				return -1;

			context->operation = 'b';
			context->device = argv[++i];
		} else if (strcmp(argv[i], "load") == 0) {
			if (context->operation || argc <= i)
				return -1;

			context->operation = 'l';
			context->filename = argv[++i];
		} else if (strcmp(argv[i], "-h") == 0) {
			if (context->operation)
				return -1;

			context->operation = 'h';
		} else if (strcmp(argv[i], "-s") == 0) {
			context->operation = 's';
		} else if (strcmp(argv[i], "-v") == 0) {
			context->verbose = 1;
		} else if (strcmp(argv[i], "-w") == 0) {
			context->wait = 1;
		} else {
			return -1;
		}
	}

	if (!context->operation)
		return -1;

	return 0;
}

static int boot(struct context *context)
{
	uint8_t value;
	size_t i;
	int rc;

	if (context == NULL || context->description == NULL || context->device == NULL)
		return -1;

	for (i = 0; i < context->description->boot_devices_count; i++) {
		if (strcmp(context->description->boot_devices[i].name, context->device) == 0) {
			value = context->description->boot_devices[i].value;
			break;
		}
	}

	if (i == context->description->boot_devices_count) {
		fprintf(stderr, "Booting device %s not found\n", context->device);
		return -1;
	}

	rc = boot_device(context, value);
	if (rc < 0)
		return -1;

	return 0;
}

static int load(struct context *context)
{
	void *buffer = NULL;
	size_t length;
	size_t count;
	size_t chunk;
	uint8_t *p;
	size_t i;
	struct stat st;
	int fd = -1;
	int rc;

	if (context == NULL || context->filename == NULL)
		return -1;

	rc = stat(context->filename, &st);
	if (rc < 0) {
		fprintf(stderr, "Stating file failed\n");
		goto error;
	}

	length = st.st_size;

	fd = open(context->filename, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Opening file failed\n");
		goto error;
	}

	if (context->verbose)
		printf("Loading data with length %d bytes\n", (int) length);

	buffer = calloc(1, USB_SEND_CHUNK);

	rc = boot_peripheral(context, length);
	if (rc < 0)
		goto error;

	count = 0;

	while (count < length) {
		chunk = (length - count) < USB_SEND_CHUNK ? (length - count) : USB_SEND_CHUNK;

		p = (uint8_t *) buffer;
		i = 0;

		while (i < chunk) {
			rc = read(fd, p, chunk - i);
			if (rc <= 0)
				goto error;

			i += rc;
			p += rc;
		}

		rc = usb_send(context, buffer, chunk);
		if (rc < 0)
			goto error;

		count += chunk;
	}

	rc = 0;
	goto complete;

error:
	rc = -1;

complete:
	if (buffer != NULL)
		free(buffer);

	if (fd >= 0)
		close(fd);

	return rc;
}

int main(int argc, char *argv[])
{
	struct context context = { 0 };
	int rc;

	rc = arguments_parse(&context, argc, argv);
	if (rc < 0) {
		usage_print();
		return -1;
	}

	if (context.operation == 'h') {
		usage_print();
		return 0;
	}

	printf("Finding and opening USB device\n");

	rc = usb_open(&context, omap_descriptions, omap_descriptions_count);
	if (rc < 0)
		goto error;

	rc = boot_asic_id(&context);
	if (rc < 0) {
		fprintf(stderr, "Getting ASIC ID failed\n");
		return -1;
	}

	switch (context.operation) {
		case 'n':
			printf("Skipping to next booting device...\n");

			rc = boot_next(&context);
			if (rc < 0) {
				fprintf(stderr, "Skipping to next booting device failed\n");
				return -1;
			}

			break;
		case 'm':
			printf("Skipping to next memory booting device...\n");

			rc = boot_memory(&context);
			if (rc < 0) {
				fprintf(stderr, "Skipping to next memory booting device failed\n");
				return -1;
			}

			break;
		case 'b':
			printf("Booting from device %s...\n", context.device);

			rc = boot(&context);
			if (rc < 0) {
				fprintf(stderr, "Booting from device failed\n");
				goto error;
			}

			break;
		case 'l':
			printf("Loading and executing %s...\n", context.filename);

			rc = load(&context);
			if (rc < 0) {
				fprintf(stderr, "Loading and executing failed\n");
				goto error;
			}

			break;
		case 's':
			printf("stalling...\n");

			while (1) {
				libusb_handle_events_completed(NULL, NULL);
				usleep(100);

				if (context.completed) {
					for (int i = 0; i < 10; i++) {
						rc = boot_asic_id(&context);
						if (rc < 0) {
							fprintf(stderr, "Getting ASIC ID failed\n");
						}
						usleep(1000); // change to test different timing.
					}
					// wait until disconnected
					while (1) {
						libusb_handle_events_completed(NULL, NULL);
						if (context.completed == 0) {
							break;
						}
					}
				}
			}

			break;
		default:
			goto error;
	}

	printf("Done!\n");

	rc = 0;
	goto complete;

error:
	rc = 1;

complete:
	usb_close(&context);

	return rc;
}

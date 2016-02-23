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

#include "omap-usb-boot.h"
#include "boot.h"
#include "usb.h"

int boot_peripheral(struct context *context, size_t length)
{
	uint32_t command;
	int rc;

	if (context == NULL)
		return -1;

	command = htole32(BOOT_PERIPHERAL);

	rc = usb_send(context, &command, sizeof(command));
	if (rc < 0)
		return -1;

	command = htole32((uint32_t) length);

	rc = usb_send(context, &command, sizeof(command));
	if (rc < 0)
		return -1;

	return 0;
}

int boot_asic_id(struct context *context)
{
	int rc;

	if (context == NULL || context->description == NULL)
		return -1;

	if (context->description->boot_asic_id != NULL) {
		rc = context->description->boot_asic_id(context);
		if (rc < 0)
			return -1;
	}

	return 0;
}

int boot_device(struct context *context, uint8_t value)
{
	uint32_t command;
	int rc;

	if (context == NULL)
		return -1;

	command = htole32(BOOT_DEVICE | (value << 8));

	rc = usb_send(context, &command, sizeof(command));
	if (rc < 0)
		return -1;

	return 0;
}

int boot_next(struct context *context)
{
	uint32_t command;
	int rc;

	if (context == NULL)
		return -1;

	command = htole32(BOOT_NEXT);

	rc = usb_send(context, &command, sizeof(command));
	if (rc < 0)
		return -1;

	return 0;
}

int boot_memory(struct context *context)
{
	uint32_t command;
	int rc;

	if (context == NULL)
		return -1;

	command = htole32(BOOT_MEMORY);

	rc = usb_send(context, &command, sizeof(command));
	if (rc < 0)
		return -1;

	return 0;
}

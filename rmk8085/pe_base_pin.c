/*
 * pe_base_pin.c
 *
 *  Created on: Mar 2, 2018
 *      Author: santiago
 */

#include "pe_base.h"

void pe_pin_update(pe_pin_t * pin, bool new_data)
{
	if (pin->value != new_data)
	{
		pin->_changed = true;
	}
	pin->_prev_value = pin->value;
	pin->value = new_data;
}

void pe_pin_update_u8_to_pin_array(uint8_t data, pe_pin_t array[])
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		pe_pin_update(&array[i], ((data >> i) & 1) ? true : false);
	}
}

uint8_t pe_pin_array_to_u8(pe_pin_t array[])
{
	uint8_t i;
	uint8_t ret = 0;
	for (i = 0; i < 8; i++)
	{
		if (array[i].value)
		{
			ret |= 1 << i;
		}
	}

	return ret;
}

/*
 * pe_interacion.c
 *
 *  Created on: Mar 4, 2018
 *      Author: santiago
 */

#include "pe_interaction.h"

pe_base1_t 		** pe_peripherals = 0;
uint16_t 		pe_peripherals_count = 0;

bool pe_mem_locate_peripheral(uint16_t addr, pe_base1_t ** p)
{
	uint16_t i;
	for (i = 0; i < pe_peripherals_count; i++)
	{
		if (pe_peripherals[i]->enabled &&
			pe_peripherals[i]->mem_attached &&
			(pe_peripherals[i]->mem_start <= addr) &&
			(pe_peripherals[i]->mem_end >= addr))
		{
			*p = pe_peripherals[i];
			return true;
		}
	}
	return false;
}

bool pe_io_locate_peripheral(uint16_t addr, pe_base1_t ** p)
{
	uint16_t i;
	for (i = 0; i < pe_peripherals_count; i++)
	{
		if (pe_peripherals[i]->enabled &&
			pe_peripherals[i]->io_attached &&
			(pe_peripherals[i]->io_start <= addr) &&
			(pe_peripherals[i]->io_end >= addr))
		{
			*p = pe_peripherals[i];
			return true;
		}
	}
	return false;
}

void pe_mem_write(uint16_t addr, uint8_t data)
{
	pe_base_peri_t * target;
	if (pe_mem_locate_peripheral(addr, (pe_base1_t**)&target))
	{
		if (target->mwrite)
		{
			target->mwrite(addr, data, PE_AS_BASE_P(target));
		}
	}
}

uint8_t pe_mem_read(uint16_t addr)
{
	pe_base_peri_t * target;
	if (pe_mem_locate_peripheral(addr, (pe_base1_t**)&target))
	{
		if (target->mread)
		{
			return target->mread(addr, PE_AS_BASE_P(target));
		}
	}
	return 0;
}

void pe_io_write(uint16_t addr, uint8_t data)
{
	pe_base_peri_t * target;
	if (pe_io_locate_peripheral(addr, (pe_base1_t**)&target))
	{
		target->iwrite(addr, data, PE_AS_BASE_P(target));
	}
}

uint8_t pe_io_read(uint16_t addr)
{
	pe_base_peri_t * target;
	if (pe_io_locate_peripheral(addr, (pe_base1_t**)&target))
	{
		return target->iread(addr, PE_AS_BASE_P(target));
	}
	return 0;
}

void pe_set_devices_array(pe_base1_t * peripherals[], uint16_t count)
{
	pe_peripherals = peripherals;
	pe_peripherals_count = count;
}

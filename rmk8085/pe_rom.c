/*
 * pe_rom.c
 *
 *  Created on: Mar 4, 2018
 *      Author: santiago
 */

#include "pe_base.h"
#include "pe_rom.h"

void pe_rom_mem_write(uint16_t addr, uint8_t data, pe_rom_t * obj)
{
	return;
}

uint8_t pe_rom_mem_read(uint16_t addr, pe_rom_t * obj)
{
	if (fseek(obj->fhandle, addr + obj->offset - obj->base.mem_start, SEEK_SET))
	{
		return 0; /* error */
	}

	uint8_t data;
	fread(&data, 1, 1, obj->fhandle);
	return data;
}

void pe_rom_reset(pe_rom_t * obj)
{

}

void pe_rom_init(pe_rom_t * new_pe, FILE * fhandle, uint32_t offset)
{
	/*
	 * Setup the callbacks
	 */
	new_pe->base.mwrite = (cb_pe_mem_write)pe_rom_mem_write;
	new_pe->base.mread = (cb_pe_mem_read)pe_rom_mem_read;
	new_pe->base.reset = (cb_pe_reset)pe_rom_reset;

	/*
	 * Setup the capabilities of this peripheral
	 */
	new_pe->base.io_attached = false;
	new_pe->base.mem_attached = true;

	new_pe->fhandle = fhandle;
	new_pe->offset = offset;
}

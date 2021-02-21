/*
 * pe_sram.c
 *
 *  Created on: Mar 3, 2018
 *      Author: santiago
 */

#include "pe_base.h"
#include "pe_sram.h"

void pe_sram_mem_write(uint16_t addr, uint8_t data, pe_sram_base_t * obj)
{
	obj->_sram[addr] = data;
}

uint8_t pe_sram_mem_read(uint16_t addr, pe_sram_base_t * obj)
{
	return obj->_sram[addr];
}

void pe_sram_reset(pe_sram_base_t * obj)
{

}

void pe_sram_init(pe_sram_base_t * new_pe, uint32_t ram_size)
{
	/*
	 * Setup the callbacks
	 */
	new_pe->base.mwrite = (cb_pe_mem_write)pe_sram_mem_write;
	new_pe->base.mread = (cb_pe_mem_read)pe_sram_mem_read;
	new_pe->base.reset = (cb_pe_reset)pe_sram_reset;

	/*
	 * Setup the capabilities of this peripheral
	 */
	new_pe->base.io_attached = false;
	new_pe->base.mem_attached = true;
}

/*
 * pe_i8251.c
 *
 *  Created on: Mar 3, 2018
 *      Author: santiago
 */

#include "pe_base.h"
#include "pe_i8251.h"

void pe_i8251_write(uint16_t addr, uint8_t data, pe_i8251_t * obj)
{
	uint16_t lower_addr = addr & 0x0007;

	switch(lower_addr)
	{
		case 0x0000: /* UART Data */
		{
			//fputc(data, stdout);
			//fflush(stdout);
			if (obj->on_new_char)
			{
				void * userdata = obj->callback_userdata;
				obj->on_new_char(data, userdata);
			}
			break;
		}
		case 0x0001: /* UART Status */
		{
			/* discard this write */
			break;
		}
	}
}

uint8_t pe_i8251_read(uint16_t addr, pe_i8251_t * obj)
{
	uint16_t lower_addr = addr & 0x0007;

	switch(lower_addr)
	{
		case 0x0000: /* UART Data */
		{
			if (obj->on_fetch_char)
			{
				void * userdata = obj->callback_userdata;
				uint8_t new_char;
				obj->on_fetch_char(&new_char, userdata);
				return new_char;
			}
			break;
		}
		case 0x0001: /* UART Status */
		{
			if (obj->on_kbhit_char)
			{
				void * userdata = obj->callback_userdata;
				bool kbhit;
				obj->on_kbhit_char(&kbhit, userdata);
				return kbhit ? 3 : 1;
			}
			break;
		}
	}
	return 0;
}

void pe_i8251_reset(pe_i8251_t * obj)
{

}

void pe_i8251_init(pe_i8251_t * new_pe)
{
	/*
	 * Setup the callbacks
	 */
	new_pe->base.mwrite = (cb_pe_mem_write)pe_i8251_write;
	new_pe->base.mread = (cb_pe_mem_read)pe_i8251_read;
	new_pe->base.iwrite = (cb_pe_io_write)pe_i8251_write;
	new_pe->base.iread = (cb_pe_io_read)pe_i8251_read;
	new_pe->base.reset = (cb_pe_reset)pe_i8251_reset;

	/*
	 * Setup the capabilities of this peripheral
	 */
	new_pe->base.io_attached = false;
	new_pe->base.mem_attached = true;
}

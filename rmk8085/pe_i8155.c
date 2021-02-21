/*
 * pe_i8155.c
 *
 *  Created on: Mar 2, 2018
 *      Author: santiago
 */

#include "pe_base.h"
#include "pe_i8155.h"

void pe_i8155_mem_write(uint16_t addr, uint8_t data, pe_i8155_t * obj)
{
	uint16_t lower_addr = addr & 0x00FF;

	obj->_sram[lower_addr] = data;
}

uint8_t pe_i8155_mem_read(uint16_t addr, pe_i8155_t * obj)
{
	uint16_t lower_addr = addr & 0x00FF;

	return obj->_sram[lower_addr];
}

void pe_i8155_io_write(uint16_t addr, uint8_t data, pe_i8155_t * obj)
{
	uint16_t lower_addr = addr & 0x0007;

	switch(lower_addr)
	{
		case PE_I8155_IO_COMMAND_REGISTER:
		{
			obj->_command_register = data;
			break;
		} /* PE_I8155_IO_COMMAND_REGISTER */
		case PE_I8155_IO_PORTA:
		{
			pe_pin_update_u8_to_pin_array(data, obj->port_a_pins);
			break;
		} /* PE_I8155_IO_PORTA */
		case PE_I8155_IO_PORTB:
		{
			pe_pin_update_u8_to_pin_array(data, obj->port_b_pins);
			break;
		} /* PE_I8155_IO_PORTB */
	}
}

uint8_t pe_i8155_io_read(uint16_t addr, pe_i8155_t * obj)
{
	uint16_t lower_addr = addr & 0x0007;

	switch(lower_addr)
	{
		case PE_I8155_IO_STATUS_REGISTER:
		{
			return obj->_status_register;
			break;
		} /* PE_I8155_IO_STATUS_REGISTER */
		case PE_I8155_IO_PORTA:
		{
			return pe_pin_array_to_u8(obj->port_a_pins);
			break;
		} /* PE_I8155_IO_PORTA */
		case PE_I8155_IO_PORTB:
		{
			return pe_pin_array_to_u8(obj->port_b_pins);
			break;
		} /* PE_I8155_IO_PORTB */
	}
	return 0;
}

bool pe_i8155_reset(pe_i8155_t * obj)
{
	uint8_t i;

	//memset(obj->_sram, 0, 256);

	for (i = 0; i < 8; i++)
	{
		obj->port_a_pins[i]._changed = false;
		obj->port_a_pins[i]._prev_value = false;
		obj->port_a_pins[i].direction = PE_PIN_DIR_IN;
		obj->port_a_pins[i].value = false;

		obj->port_b_pins[i]._changed = false;
		obj->port_b_pins[i]._prev_value = false;
		obj->port_b_pins[i].direction = PE_PIN_DIR_IN;
		obj->port_b_pins[i].value = false;
	}

	for (i = 0; i < 6; i++)
	{
		obj->port_c_pins[i]._changed = false;
		obj->port_c_pins[i]._prev_value = false;
		obj->port_c_pins[i].direction = PE_PIN_DIR_IN;
		obj->port_c_pins[i].value = false;
	}

	return true;
}

void pe_i8155_init(pe_i8155_t * new_pe)
{
	/*
	 * Setup the callbacks
	 */
	new_pe->base.mwrite = (cb_pe_mem_write)pe_i8155_mem_write;
	new_pe->base.mread = (cb_pe_mem_read)pe_i8155_mem_read;
	new_pe->base.iwrite = (cb_pe_io_write)pe_i8155_io_write;
	new_pe->base.iread = (cb_pe_io_read)pe_i8155_io_read;
	new_pe->base.reset = (cb_pe_reset)pe_i8155_reset;

	/*
	 * Setup the capabilities of this peripheral
	 */
	new_pe->base.io_attached = true;
	new_pe->base.mem_attached = true;
}

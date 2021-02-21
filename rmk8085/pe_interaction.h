/*
 * pe_interacion.h
 *
 *  Created on: Mar 4, 2018
 *      Author: santiago
 */

#ifndef PE_INTERACTION_H_
#define PE_INTERACTION_H_

#include "pe_base.h"

void pe_mem_write(uint16_t addr, uint8_t data);
uint8_t pe_mem_read(uint16_t addr);
void pe_io_write(uint16_t addr, uint8_t data);
uint8_t pe_io_read(uint16_t addr);


void pe_set_devices_array(pe_base1_t * peripherals[], uint16_t count);

#endif /* PE_INTERACTION_H_ */

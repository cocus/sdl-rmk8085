/*
 * pe_rom.h
 *
 *  Created on: Mar 4, 2018
 *      Author: santiago
 */

#ifndef PE_ROM_H_
#define PE_ROM_H_

#include "pe_base.h"
#include <stdio.h>

typedef struct
{
	pe_base_peri_t			base;
	FILE					* fhandle;
	uint32_t				offset;
} pe_rom_t;

void pe_rom_init(pe_rom_t * new_pe, FILE * fhandle, uint32_t offset);

#endif /* PE_ROM_H_ */

/*
 * pe_sram.h
 *
 *  Created on: Mar 3, 2018
 *      Author: santiago
 */

#ifndef PE_SRAM_H_
#define PE_SRAM_H_

#include "pe_base.h"

#define PE_RAM_DEFINE											\
		pe_base_peri_t		base;								\
		uint32_t			_ram_size;							\

typedef struct
{
	PE_RAM_DEFINE
	uint8_t 				_sram[];
} pe_sram_base_t;


void pe_sram_init(pe_sram_base_t * new_pe, uint32_t ram_size);


#define PE_SRAM_AS_BASE(b)	(*(pe_sram_base_t*)(&b))
#define PE_SRAM_AS_BASE_P(b)((pe_sram_base_t*)(&b))

#define PE_RAM_DEFINE_GENERIC(size_in_k) 						\
	typedef struct												\
	{															\
		PE_RAM_DEFINE											\
		uint8_t				_sram[size_in_k*1024]; 				\
	} pe_sram_##size_in_k##k_t;									\
	void inline pe_sram_##size_in_k##k_init(pe_sram_##size_in_k##k_t * new_pe) { pe_sram_init(PE_SRAM_AS_BASE_P(new_pe), size_in_k*1024); }

PE_RAM_DEFINE_GENERIC(32)
PE_RAM_DEFINE_GENERIC(64)

#endif /* PE_SRAM_H_ */

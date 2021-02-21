/*
 * pe_i8251.h
 *
 *  Created on: Mar 3, 2018
 *      Author: santiago
 */

#ifndef PE_I8251_H_
#define PE_I8251_H_

#include "pe_base.h"

typedef struct
{
	pe_base_peri_t		base;
	int (*on_new_char)(uint8_t new_char, void * userdata);//			callback;
	int (*on_kbhit_char)(bool *have_data, void * userdata);//			callback;
	int (*on_fetch_char)(uint8_t *new_char, void * userdata);//			callback;
	void				*callback_userdata;
} pe_i8251_t;

void pe_i8251_init(pe_i8251_t * new_pe);

#endif /* PE_I8251_H_ */

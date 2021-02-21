/*
 * pe_bq3285.h
 *
 *  Created on: Mar 3, 2018
 *      Author: santiago
 */

#ifndef PE_BQ3285_H_
#define PE_BQ3285_H_

#include "pe_base.h"

typedef struct
{
	pe_base_peri_t		base;

	char				_nvram[64];
} pe_bq3285_t;

void pe_bq3285_init(pe_bq3285_t * new_pe);

#endif /* PE_BQ3285_H_ */

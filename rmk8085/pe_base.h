/*
 * pe_base.h
 *
 *  Created on: Mar 2, 2018
 *      Author: santiago
 */

#ifndef PE_BASE_H_
#define PE_BASE_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * PIN related enums, structs, defines, and decls
 */
typedef enum
{
	PE_PIN_DIR_IN,
	PE_PIN_DIR_OUT,
	PE_PIN_DIR_IO,
	PE_PIN_DIR_FLOAT
} PE_PIN_DIR;

typedef struct
{
	bool 				value;
	bool 				_changed;
	bool				_prev_value;

	PE_PIN_DIR			direction;
} pe_pin_t;

void pe_pin_update(pe_pin_t * pin, bool new_data);
void pe_pin_update_u8_to_pin_array(uint8_t data, pe_pin_t array[]);
uint8_t pe_pin_array_to_u8(pe_pin_t array[]);



/**
 * BASE related enums, structs, defines, and decls
 */

/**
 * Idea taken from
 * https://modelingwithdata.org/arch/00000113.htm
 */
#define PE_BASE_DEFINE  \
	uint16_t			mem_start;		\
	uint16_t			mem_end;		\
	bool				mem_attached;	\
	uint16_t			io_start;		\
	uint16_t			io_end;			\
	bool				io_attached;	\
	bool				enabled;

typedef struct pe_base1
{
	PE_BASE_DEFINE
} pe_base1_t;


/**
 * Please use PE_AS_BASE_P(your_pe_base_peri_t object) as the obj argument
 * in the following functions.
 */
#define PE_AS_BASE(b)	(*(pe_base1_t*)(&b))
#define PE_AS_BASE_P(b)	((pe_base1_t*)(b))

typedef void (*cb_pe_mem_write)(uint16_t addr, uint8_t data, pe_base1_t * obj);
typedef uint8_t (*cb_pe_mem_read)(uint16_t addr, pe_base1_t * obj);
typedef void (*cb_pe_io_write)(uint16_t addr, uint8_t data, pe_base1_t * obj);
typedef uint8_t (*cb_pe_io_read)(uint16_t addr, pe_base1_t * obj);
typedef void (*cb_pe_reset)(pe_base1_t * obj);

typedef struct
{
	PE_BASE_DEFINE

	cb_pe_mem_write		mwrite;
	cb_pe_mem_read		mread;
	cb_pe_io_write		iwrite;
	cb_pe_io_read		iread;
	cb_pe_reset			reset;
} pe_base_peri_t;



#endif /* PE_BASE_H_ */

/*
 * pe_i8279.h
 *
 *  Created on: Mar 5, 2018
 *      Author: santiago
 */

#ifndef PE_I8279_H_
#define PE_I8279_H_

#include "pe_base.h"

/**
 * Command: Keyboard/Display Mode Set
 * 000DDKKK, Where DD is the Display Mode and KKK is the
 * Key-board Mode.
 *
 * D D
 * 0 0  8 8-bit character display-Left entry
 * 0 1  16 8-bit character display-Left entry*
 * 1 0  8 8-bit character display-Right entry
 * 1 1  16 8-bit character display-Right entry
 *
 * K K K
 * 0 0 0  Encoded Scan Keyboard-2 Key Lock-out*
 * 0 0 1  Decoded Scan Keyboard-2 Key Lock-out
 * 0 1 0  Encoded Scan Keyboard-N-Key Roll-over
 * 0 1 1  Decoded Scan Keyboard-N-Key Roll-over
 * 1 0 0  Encoded Scan Sensor Matrix
 * 1 0 1  Decoded Scan Sensor Matrix
 * 1 1 0  Strobed Input, Encoded Display Scan
 * 1 1 1  Strobed Input, Decoded Display Scan
 */
#define PE_I8279_CMD_KB_DISP_MODE_SET		0x00

#define PE_I8279_DD_8x8CHAR_LEFT			0
#define PE_I8279_DD_8x16CHAR_LEFT			1
#define PE_I8279_DD_8x8CHAR_RIGHT			2
#define PE_I8279_DD_8x16CHAR_RIGHT			3

#define PE_I8279_KKK_2KEY_LOCK_ENCODED		0
#define PE_I8279_KKK_2KEY_LOCK_DECODED		1
#define PE_I8279_KKK_NKEY_ROLL_ENCODED		2
#define PE_I8279_KKK_NKEY_ROLL_DECODED		3
#define PE_I8279_KKK_SCAN_ENCODED			4
#define PE_I8279_KKK_SCAN_DECODED			5
#define PE_I8279_KKK_STROBED_IN_ENCODED		6
#define PE_I8279_KKK_STROBED_IN_DECODED		7

/**
 * Command: Program Clock
 * 001PPPPP.
 * All timing and multiplexing signals for the 8279
 * are generated by an internal prescaler. This
 * prescaler divides the external clock by a
 * Programmable integer. Bits PPPPP determine the
 * value of this integer which ranges from 2 to 31.
 * Choosing a divisor that yields 100kHz will give the
 * specified scan and debounce times. For instance,
 * if Pin 3 of the 8279 is being clocked by a 2 MHz
 * signal, PPPPP should be set to 10100 to divide
 * the clock by 20 to yield the proper 100 kHz operating
 * frequency.
 *
 * This parameter is ignored
 */
#define PE_I8279_CMD_PROGRAM_CLOCK			0x10

/**
 * Command: Read FIFO/Sensor RAM
 * 010AIXAAA.
 * The CPU sets the 8279 for a read of the FIFO/
 * Sensor RAM by first writing this command. In
 * the Scan Keyboard Mode, the Auto-Increment flag
 * (AI) and the RAM address bits (AAA) are irrelevant.
 * The 8279 will automatically drive the data bus for
 * each subsequent read (A0 = 0) in the same sequence
 * in which the data first entered the FIFO. All
 * subsequent reads will be from the FIFO until another
 * command is issued.
 * In the Sensor Matrix Mode, the RAM address bits
 * AAA select one of the 8 rows of the Sensor RAM.
 * If the AI flag is set, each successive read will
 * be from the subsequent row of the sensor RAM.
 */
#define PE_I8279_CMD_READ_FIFO				0x20


typedef struct
{
	pe_base_peri_t		base;
} pe_i8279_t;

void pe_i8279_init(pe_i8279_t * new_pe);

#endif /* PE_I8279_H_ */
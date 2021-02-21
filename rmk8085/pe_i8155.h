/*
 * pe_8155.h
 *
 *  Created on: Mar 2, 2018
 *      Author: santiago
 */

#ifndef PE_I8155_H_
#define PE_I8155_H_

#include "pe_base.h"

#define PE_I8155_IO_COMMAND_REGISTER		0
#define PE_I8155_IO_STATUS_REGISTER			0

#define PE_I8155_IO_PORTA					1
#define PE_I8155_IO_PORTB					2
#define PE_I8155_IO_PORTC					3

typedef struct
{
	pe_base_peri_t		base;

	char				_sram[256];

	union
	{
		char _command_register;				/* IO @ 0x00, W */
		struct
		{
			char PA 		: 1; /* Port A IN/OUT, 0-Input, 1-Output */
			char PB			: 1; /* Port B IN/OUT, 0-Input, 1-Output */
			char PC			: 2; /* Port C ALT 1, ALT 2, ALT 3, ALT 4 */

			char IEA		: 1; /* Enable Interrupts from port A */
			char IEB		: 1; /* Enable Interrupts from port B */

			char TM			: 2; /* Timer command, 00-NOP, 01-Stop, 10-Stop after TC, 11-Start */
		};
	};

	union
	{
		char _status_register;				/* IO @ 0x00, R */
		struct
		{
			char INTRA		: 1; /* Port A interrupt request */
			char ABF		: 1; /* Port A buffer full/empty (input/output) */
			char INTEA		: 1; /* Port A interrupt enable */

			char INTRB		: 1; /* Port B interrupt request */
			char BBF		: 1; /* Port B buffer full/empty (input/output) */
			char INTEB		: 1; /* Port B interrupt enable */

			char TIMER		: 1; /* Timer interrupt */
		};
	};
	pe_pin_t			port_a_pins[8];		/* IO @ 0x01, RW */
	pe_pin_t			port_b_pins[8];		/* IO @ 0x02, RW */
	pe_pin_t			port_c_pins[6];		/* IO @ 0x03, RW */

	uint8_t				_timer_lsb;			/* IO @ 0x04, RW */

	struct
	{
		char T			: 6; /* MSB of CNT Length */
		char M			: 2; /* Timer Mode */
	} _timer_msb;							/* IO @ 0x05, RW */
} pe_i8155_t;

void pe_i8155_init(pe_i8155_t * new_pe);

#endif /* PE_I8155_H_ */

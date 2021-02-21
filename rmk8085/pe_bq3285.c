/*
 * pe_bq3285.c
 *
 *  Created on: Mar 3, 2018
 *      Author: santiago
 */

#include "pe_base.h"
#include "pe_bq3285.h"

static unsigned char hex2bcd(unsigned char x)
{
	unsigned char y;
	y = (x / 10) << 4;
	y = y | (x % 10);
	return (y);
}

#ifdef WIN32
#include <Windows.h>

#define time_common		SYSTEMTIME time; GetLocalTime(&time);

#define time_seconds()	hex2bcd(time.wSecond)
#define time_minutes()	hex2bcd(time.wMinute)
#define time_hours()	hex2bcd(time.wHour)

#define time_day()		hex2bcd(time.wDay)
#define time_month()	hex2bcd(time.wMonth)
#define time_year()		hex2bcd(time.wYear % 2000)
#else
#include <time.h>

#define time_common		time_t t = time(NULL); struct tm tm = *localtime(&t);

#define time_seconds()	hex2bcd(tm.tm_sec)
#define time_minutes()	hex2bcd(tm.tm_min)
#define time_hours()	hex2bcd(tm.tm_hour)

#define time_day()		hex2bcd(tm.tm_mday)
#define time_month()	hex2bcd(tm.tm_mon + 1)
#define time_year()		hex2bcd((tm.tm_year + 1900) % 2000)
#endif

void pe_bq3285_mem_write(uint16_t addr, uint8_t data, pe_bq3285_t * obj)
{
	uint16_t lower_addr = addr & 0x00FF;

	switch (lower_addr)
	{
		case 0x00: { break; }
		case 0x02: { break; }
		case 0x04: { break; }
		case 0x07: { break; }
		case 0x08: { break; }
		case 0x09: { break; }
		default:
		{
			obj->_nvram[lower_addr] = data;
		}
	}
}

uint8_t pe_bq3285_mem_read(uint16_t addr, pe_bq3285_t * obj)
{
	uint16_t lower_addr = addr & 0x000F;

	time_common;

	switch (lower_addr)
	{
		case 0x00: { return time_seconds(); break; }
		case 0x02: { return time_minutes(); break; }
		case 0x04: { return time_hours(); break; }
		case 0x07: { return time_day(); break; }
		case 0x08: { return time_month(); break; }
		case 0x09: { return time_year(); break; }
		default:
		{
			return obj->_nvram[lower_addr];
		}
	}
}

void pe_bq3285_reset(pe_bq3285_t * obj)
{

}

void pe_bq3285_init(pe_bq3285_t * new_pe)
{
	/*
	 * Setup the callbacks
	 */
	new_pe->base.mwrite = (cb_pe_mem_write)pe_bq3285_mem_write;
	new_pe->base.mread = (cb_pe_mem_read)pe_bq3285_mem_read;
	new_pe->base.reset = (cb_pe_reset)pe_bq3285_reset;

	/*
	 * Setup the capabilities of this peripheral
	 */
	new_pe->base.io_attached = false;
	new_pe->base.mem_attached = true;
}

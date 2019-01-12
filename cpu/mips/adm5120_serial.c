/*
 * ADM5120 UART support
 *
 * Hardcoded to UART 0 for now
 * Speed and options also hardcoded to 115200 8N1
 *
 *  Copyright (c) 2006	SATO, Masuhiro <thomas@fenix.ne.jp>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>

#if 0
#ifdef CONFIG_ADM5120

#include <common.h>

#include <asm/adm5120.h>


#define UART_BASE	0xb2600000
#define UART_DATA	0x00
#define UART_RSR	0x04
#define UART_LCR_H	0x08
#define UART_LCR_M	0x0c
#define UART_LCR_L	0x10
#define UART_CR		0x14 
#define UART_FR		0x18
#define UART_IR		0x1c


static inline unsigned uart_readreg(int reg)
{
	return *(volatile unsigned *)(UART_BASE + reg);
}

static inline void uart_writereg(int reg, unsigned long value)
{
	*(volatile unsigned *)(UART_BASE + reg) = value;
}

/******************************************************************************
*
* serial_init - initialize a channel
*
* This routine initializes the number of data bits, parity
* and set the selected baud rate. Interrupts are disabled.
* Set the modem control signals if the option is selected.
*
* RETURNS: N/A
*/

int serial_init (void)
{
// LED debug
*(volatile unsigned *)(0xb20000b8) = 0x08080000;

	serial_setbrg();

	return 0;
}


void serial_setbrg(void)
{
	unsigned clkdiv;
	DECLARE_GLOBAL_DATA_PTR;

	/* disable the uart */
	uart_writereg(UART_CR, 0);

	/* 8bit, fifo enable */
	uart_writereg(UART_LCR_H, 0x70);
	clkdiv = 62500000 / 16 / gd->baudrate - 1;
	uart_writereg(UART_LCR_M, clkdiv >> 8);
	uart_writereg(UART_LCR_L, clkdiv & 0xff);

	/* clear modem status interrupts */
	uart_writereg(UART_IR, 0x80);
	
	/* enable */
	uart_writereg(UART_CR, 0x01);
}


void serial_putc (const char c)
{
	if (c == '\n')
		serial_putc ('\r');

	while ((uart_readreg(UART_FR) & 0x20) != 0)
		;

	uart_writereg(UART_DATA, c);
}

void serial_puts (const char *s)
{
	while (*s)
		serial_putc (*s++);
}


int serial_getc(void)
{
	while ((uart_readreg(UART_FR) & 0x10) != 0)
		;

	return uart_readreg(UART_DATA);
}



int serial_tstc (void)
{
	if ((uart_readreg(UART_FR) & 0x10) != 0)
		return 0;

	return 1;
}

#endif /* CONFIG_ADM5120 */
#endif

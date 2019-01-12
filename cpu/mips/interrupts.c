/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
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

#include <common.h>

void enable_interrupts(void)
{
	asm volatile (
	"	mfc0	$8, $12\n"
	"	ori	$8, 1\n"
	"	mtc0	$8, $12\n"
	"	nop\n"
	: : : "$8");
}

int disable_interrupts(void)
{
	u32 cp0_status;

	asm volatile (
	"	mfc0	%0, $12\n"
	"	ori	%0, 1\n"
	"	xori	%0, 1\n"
	"	mtc0	%0, $12\n"
	"	nop\n"
	: "=r" (cp0_status));

	return cp0_status & 1;
}


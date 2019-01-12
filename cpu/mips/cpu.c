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
#include <command.h>
#include <asm/cacheops.h>
#include <asm/inca-ip.h>
#include <asm/mipsregs.h>
#include <asm/adm5120.h>

static void dump_regs(unsigned long *regs);

static inline void
icache_hit_invalidate(register ulong addr)
{
	asm volatile(
	"	.set	push\n"	
	"	.set	mips3\n"
	"	.set	noreorder\n"
	"	cache	0x10,0(%0)\n"
	"	nop\n"
	"	nop\n"
	"	.set	pop\n"
	: : "r" (addr)); 						\
}


static inline void
dcache_hit_writeback_invalidate(register ulong addr)
{
	asm volatile(
	"	.set	push\n"	
	"	.set	mips3\n"
	"	.set	noreorder\n"
	"	cache	0x15,0(%0)\n"
	"	nop\n"
	"	nop\n"
	"	.set	pop\n"
	: : "r" (addr)); 						\
}


static inline void
dcache_hit_invalidate(register ulong addr)
{
	asm volatile(
	"	.set	push\n"	
	"	.set	mips3\n"
	"	.set	noreorder\n"
	"	cache	0x11,0(%0)\n"
	"	nop\n"
	"	nop\n"
	"	.set	pop\n"
	: : "r" (addr)); 						\
}



int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#if defined(CONFIG_INCA_IP)
	*INCA_IP_WDT_RST_REQ = 0x3f;
#elif defined(CONFIG_ADM5120)
	/* Disable All ports*/
	scr_write(SCR_PORT_CONF0,
			scr_read(SCR_PORT_CONF0) | SW_DISABLE_PORT_MASK);

	/* Disable CPU port */
	scr_write(SCR_CPUP_CONF, scr_read(SCR_CPUP_CONF) | SW_CPU_PORT_DISABLE);

	/* Wait until switch DMA idle. At least 1ms is required!!!! */
	udelay(10000);

	scr_write(SCR_SFTRESET, SOFTWARE_RESET);

#elif defined(CONFIG_PURPLE) || defined(CONFIG_TB0229)
	void (*f)(void) = (void *) 0xbfc00000;

	f();
#endif
	fprintf(stderr, "*** reset failed ***\n");
	return 0;
}


void write_one_tlb( int index, u32 pagemask, u32 hi, u32 low0, u32 low1 ){
	write_32bit_cp0_register(CP0_ENTRYLO0, low0);
	write_32bit_cp0_register(CP0_PAGEMASK, pagemask);
	write_32bit_cp0_register(CP0_ENTRYLO1, low1);
	write_32bit_cp0_register(CP0_ENTRYHI, hi);
	write_32bit_cp0_register(CP0_INDEX, index);
	tlb_write_indexed();
}


void flush_cache (ulong start, ulong size)
{
	ulong addr;
	ulong end = start + size;

	for (addr = start & ~(CFG_CACHELINE_SIZE - 1);
				addr < end; addr += CFG_CACHELINE_SIZE)
		dcache_hit_writeback_invalidate(addr);

	for (addr = start & ~(CFG_CACHELINE_SIZE - 1);
				addr < end; addr += CFG_CACHELINE_SIZE)
		icache_hit_invalidate(addr);
}


void dcache_wback_inv(unsigned long start, unsigned long size)
{
	ulong addr;
	ulong end = start + size;

	for (addr = start & ~(CFG_CACHELINE_SIZE - 1);
				addr < end; addr += CFG_CACHELINE_SIZE)
		dcache_hit_writeback_invalidate(addr);
}


void dcache_inv(unsigned long start, unsigned long size)
{
	ulong addr;
	ulong end = start + size;

	for (addr = start & ~(CFG_CACHELINE_SIZE - 1);
				addr < end; addr += CFG_CACHELINE_SIZE)
		dcache_hit_invalidate(addr);
}



void
exception_handler(u32 exception_stack)
{
	int exception_num;
	u32 status, cause, epc, badva;
	char *exception_name;

	asm volatile("mfc0	%0, $12\n" : "=r" (status));
	asm volatile("mfc0	%0, $13\n" : "=r" (cause));

	exception_num = (cause & 0x7c) >> 2;

	switch (exception_num) {
	case 0:
		exception_name = "Interrupt";
		break;
	case 1:
		exception_name = "TLB modify";
		break;
	case 2:
		exception_name = "TLB unmatch (load)";
		break;
	case 3:
		exception_name = "TLB unmatch (store)";
		break;
	case 4:
		exception_name = "address error (load)";
		break;
	case 5:
		exception_name = "address error (store)";
		break;
	case 6:
		exception_name = "bus error (instruction)";
		break;
	case 7:
		exception_name = "bus error (data)";
		break;
	case 8:
		exception_name = "system call";
		break;
	case 9:
		exception_name = "break point";
		break;
	case 10:
		exception_name = "reserved instruction";
		break;
	case 11:
		exception_name = "coproccessor unavailable";
		break;
	case 12:
		exception_name = "over flow";
		break;
	case 13:
		exception_name = "trap";
		break;
	case 23:
		exception_name = "watch";
		break;
	default:
		exception_name = "unknown";
		break;
	}

	printf("*** Exception %d: %s\n",
			exception_num, exception_name);
	asm volatile("mfc0	%0, $14\n" : "=r" (epc));
	asm volatile("mfc0	%0, $8\n" : "=r" (badva));
	printf("epc=%08x, badva=%08x, cause=%08x, status=%08x\n",
		epc, badva, cause, status);
	dump_regs(exception_stack);
	panic("");
}


static void
dump_regs(unsigned long *regs)
{
	int i;

	printf("registers\n");
	for (i = 0; i < 32; i++) {
		printf("$%d:%08lx ", i, regs[i]);
		if ((i % 4) == 3)
			printf("\n");
	}
	printf("hi:%08x lo:%08x\n", regs[32], regs[33]);
	printf("\n");
}


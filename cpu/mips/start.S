/*
 *  Startup Code for MIPS32 CPU-core
 *
 *  Copyright (c) 2003	Wolfgang Denk <wd@denx.de>
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
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>

#ifdef CFG_RAMBOOT
#define EXCEPTION_ENTRY_PTR	0x80000080
#endif



#define RVECENT(f,n) \
   b f; nop
#define XVECENT(f,bev) \
   b f     ;           \
   li k0,bev

#define PUTCHAR(c)	bal	_putchar; ori	a0, zero, c
#define PUTS(S)		bal	_puts; nop; .asciiz	S; .balign 8

	.text
	.set noreorder
	.globl _start
_start:
#ifndef CFG_RAMBOOT
	RVECENT(reset,0)	/* U-boot entry point */
	RVECENT(reset,1)	/* software reboot */
#if defined(CONFIG_INCA_IP)
	.word INFINEON_EBU_BOOTCFG /* EBU init code, fetched during booting */
	.word 0x00000000           /* phase of the flash                    */
#elif defined(CONFIG_PURPLE)
	.word INFINEON_EBU_BOOTCFG /* EBU init code, fetched during booting */
	.word INFINEON_EBU_BOOTCFG /* EBU init code, fetched during booting */
#else
	RVECENT(romReserved,2)
#endif
	RVECENT(romReserved,3)
	RVECENT(romReserved,4)
	RVECENT(romReserved,5)
	RVECENT(romReserved,6)
	RVECENT(romReserved,7)
	RVECENT(romReserved,8)
	RVECENT(romReserved,9)
	RVECENT(romReserved,10)
	RVECENT(romReserved,11)
	RVECENT(romReserved,12)
	RVECENT(romReserved,13)
	RVECENT(romReserved,14)
	RVECENT(romReserved,15)
	RVECENT(romReserved,16)
	RVECENT(romReserved,17)
	RVECENT(romReserved,18)
	RVECENT(romReserved,19)
	RVECENT(romReserved,20)
	RVECENT(romReserved,21)
	RVECENT(romReserved,22)
	RVECENT(romReserved,23)
	RVECENT(romReserved,24)
	RVECENT(romReserved,25)
	RVECENT(romReserved,26)
	RVECENT(romReserved,27)
	RVECENT(romReserved,28)
	RVECENT(romReserved,29)
	RVECENT(romReserved,30)
	RVECENT(romReserved,31)
	RVECENT(romReserved,32)
	RVECENT(romReserved,33)
	RVECENT(romReserved,34)
	RVECENT(romReserved,35)
	RVECENT(romReserved,36)
	RVECENT(romReserved,37)
	RVECENT(romReserved,38)
	RVECENT(romReserved,39)
	RVECENT(romReserved,40)
	RVECENT(romReserved,41)
	RVECENT(romReserved,42)
	RVECENT(romReserved,43)
	RVECENT(romReserved,44)
	RVECENT(romReserved,45)
	RVECENT(romReserved,46)
	RVECENT(romReserved,47)
	RVECENT(romReserved,48)
	RVECENT(romReserved,49)
	RVECENT(romReserved,50)
	RVECENT(romReserved,51)
	RVECENT(romReserved,52)
	RVECENT(romReserved,53)
	RVECENT(romReserved,54)
	RVECENT(romReserved,55)
	RVECENT(romReserved,56)
	RVECENT(romReserved,57)
	RVECENT(romReserved,58)
	RVECENT(romReserved,59)
	RVECENT(romReserved,60)
	RVECENT(romReserved,61)
	RVECENT(romReserved,62)
	RVECENT(romReserved,63)
	XVECENT(romExcHandle,0x200)	/* bfc00200: R4000 tlbmiss vector */
	RVECENT(romReserved,65)
	RVECENT(romReserved,66)
	RVECENT(romReserved,67)
	RVECENT(romReserved,68)
	RVECENT(romReserved,69)
	RVECENT(romReserved,70)
	RVECENT(romReserved,71)
	RVECENT(romReserved,72)
	RVECENT(romReserved,73)
	RVECENT(romReserved,74)
	RVECENT(romReserved,75)
	RVECENT(romReserved,76)
	RVECENT(romReserved,77)
	RVECENT(romReserved,78)
	RVECENT(romReserved,79)
	XVECENT(romExcHandle,0x280)	/* bfc00280: R4000 xtlbmiss vector */
	RVECENT(romReserved,81)
	RVECENT(romReserved,82)
	RVECENT(romReserved,83)
	RVECENT(romReserved,84)
	RVECENT(romReserved,85)
	RVECENT(romReserved,86)
	RVECENT(romReserved,87)
	RVECENT(romReserved,88)
	RVECENT(romReserved,89)
	RVECENT(romReserved,90)
	RVECENT(romReserved,91)
	RVECENT(romReserved,92)
	RVECENT(romReserved,93)
	RVECENT(romReserved,94)
	RVECENT(romReserved,95)
	XVECENT(romExcHandle,0x300)	/* bfc00300: R4000 cache vector */
	RVECENT(romReserved,97)
	RVECENT(romReserved,98)
	RVECENT(romReserved,99)
	RVECENT(romReserved,100)
	RVECENT(romReserved,101)
	RVECENT(romReserved,102)
	RVECENT(romReserved,103)
	RVECENT(romReserved,104)
	RVECENT(romReserved,105)
	RVECENT(romReserved,106)
	RVECENT(romReserved,107)
	RVECENT(romReserved,108)
	RVECENT(romReserved,109)
	RVECENT(romReserved,110)
	RVECENT(romReserved,111)
	XVECENT(romExcHandle,0x380)	/* bfc00380: R4000 general vector */
	RVECENT(romReserved,113)
	RVECENT(romReserved,114)
	RVECENT(romReserved,115)
	RVECENT(romReserved,116)
	RVECENT(romReserved,116)
	RVECENT(romReserved,118)
	RVECENT(romReserved,119)
	RVECENT(romReserved,120)
	RVECENT(romReserved,121)
	RVECENT(romReserved,122)
	RVECENT(romReserved,123)
	RVECENT(romReserved,124)
	RVECENT(romReserved,125)
	RVECENT(romReserved,126)
	RVECENT(romReserved,127)

	/* We hope there are no more reserved vectors!
	 * 128 * 8 == 1024 == 0x400
	 * so this is address R_VEC+0x400 == 0xbfc00400
	 */
#ifdef CONFIG_PURPLE
/* 0xbfc00400 */
	.word	0xdc870000
	.word	0xfca70000
	.word	0x20840008
	.word	0x20a50008
	.word	0x20c6ffff
	.word	0x14c0fffa
	.word	0x00000000
	.word	0x03e00008
	.word	0x00000000
	.word   0x00000000
/* 0xbfc00428 */
	.word	0xdc870000
	.word	0xfca70000
	.word	0x20840008
	.word	0x20a50008
	.word	0x20c6ffff
	.word	0x14c0fffa
	.word	0x00000000
	.word	0x03e00008
	.word	0x00000000
	.word   0x00000000
#endif /* CONFIG_PURPLE */
	.align 4
#endif

reset:

	/* Clear watch registers.
	 */
	mtc0	zero, CP0_WATCHLO
	mtc0	zero, CP0_WATCHHI

	/* STATUS register */
#ifdef  CONFIG_TB0229
	li	k0, ST0_CU0
#else
	mfc0	k0, CP0_STATUS
#endif
	li	k1, ~ST0_IE
	and	k0, k1
	mtc0	k0, CP0_STATUS

	/* CAUSE register */
	mtc0	zero, CP0_CAUSE

	/* Init Timer */
	mtc0	zero, CP0_COUNT
	mtc0	zero, CP0_COMPARE

#ifndef CFG_RAMBOOT
	/* CONFIG0 register */
	li	t0, CONF_CM_UNCACHED
	mtc0	t0, CP0_CONFIG
#endif

	/* Initialize GOT pointer.
	*/
	bal     1f
	nop
	.word   _GLOBAL_OFFSET_TABLE_
1:
	lw      gp, 0(ra)

#ifndef CFG_RAMBOOT
#ifdef CONFIG_INCA_IP
	/* Disable INCA-IP Watchdog.
	 */
	la      t9, disable_incaip_wdt
	jalr    t9
	nop
#endif

	/* Initialize any external memory.
	 */
	la      t9, lowlevel_init
	jalr    t9
	nop

	/* Initialize caches...
	 */
	la      t9, mips_cache_reset
	jalr    t9
	nop

	/* ... and enable them.
	 */
	li	t0, CONF_CM_CACHABLE_NONCOHERENT
	mtc0	t0, CP0_CONFIG

	/* Set up temporary stack.
	 */
	li	a0, CFG_INIT_SP_OFFSET
	la      t9, mips_cache_lock
	jalr    t9
	nop

#else
	bal	1f
	  nop
1:
	lui	t0, %hi(1b)
	ori	t0, %lo(1b)
	beq	ra, t0,	1f
	  nop
	li	a1, CFG_MONITOR_BASE
	sub	t0, a1
	sub	a0, ra, t0
	b	bad_load_address
	  nop
1:
#endif
	li	t0, CFG_SDRAM_BASE + CFG_INIT_SP_OFFSET
	la	sp, 0(t0)

	la	t9, board_init_f
	j	t9
	nop


/*
 * void relocate_code (addr_sp, gd, addr_moni)
 *
 * This "function" does not return, instead it continues in RAM
 * after relocating the monitor code.
 *
 * a0 = addr_sp
 * a1 = gd
 * a2 = destination address
 */
_addr_data:
	.word	uboot_end_data
	.word	uboot_end
	.word	num_got_entries

	.globl	relocate_code
	.ent	relocate_code
relocate_code:
	move	sp, a0		/* Set new stack pointer		*/

	li	t0, CFG_MONITOR_BASE
	la	t3, _addr_data
	lw	t2, 0(t3)	/* t2 <-- uboot_end_data	*/
	move	t1, a2

	/*
	 * Fix GOT pointer:
	 *
	 * New GOT-PTR = (old GOT-PTR - CFG_MONITOR_BASE) + Destination Address
	 */
	la	t5, CFG_MONITOR_BASE
	move	t6, gp
	sub	gp, t5
	addu	gp, a2			/* gp now adjusted		*/
	sub	t6, gp, t6		/* t6 <-- relocation offset	*/

	/*
	 * t0 = source address
	 * t1 = target address
	 * t2 = source end address
	 */
	/* On the purple board we copy the code earlier in a special way
	 * in order to solve flash problems
	 */
#ifndef CONFIG_PURPLE
1:
	lw	t3, 0(t0)
	sw	t3, 0(t1)
	addu	t0, 4
	ble	t0, t2, 1b
	addu	t1, 4			/* delay slot			*/
#endif

	/* If caches were enabled, we would have to flush them here.
	 */

	li	t0, 0x80000000
	li	t1, CFG_DCACHE_SIZE
	addu	t1, t0, t1
1:
	cache	1, 0(t0)		/* D cache index writeback invalidate */
	addiu	t0, t0, CFG_CACHELINE_SIZE
	bne	t0, t1, 1b
	  nop

	li	t0, 0x80000000
	li	t1, CFG_ICACHE_SIZE
	addu	t1, t0, t1
1:
	cache	0, 0(t0)		/* Icache index invalidate */
	addiu	t0, t0, CFG_CACHELINE_SIZE
	bne	t0, t1, 1b
	  nop
	
	/* Jump to where we've relocated ourselves.
	 */
	addiu	t0, a2, in_ram - _start
	j	t0
	  nop

in_ram:
	/* Now we want to update GOT.
	 */
	addiu	t0, a2, _addr_data - _start
	lw	t3, 8(t0)	/* t3 <-- num_got_entries	*/
	addiu	t4, gp, 8	/* Skipping first two entries.	*/
	li	t2, 2
1:
	lw	t1, 0(t4)
	beqz	t1, 2f
	addu	t1, t6
	sw	t1, 0(t4)
2:
	addiu	t2, 1
	blt	t2, t3, 1b
	addiu	t4, 4		/* delay slot			*/

	/* Clear BSS.
	 */
	lw	t1, 0(t0)	/* t1 <-- uboot_end_data	*/
	lw	t2, 4(t0)	/* t2 <-- uboot_end		*/
	addu	t1, t6		/* adjust pointers		*/
	addu	t2, t6

	sub	t1, 4
1:	addiu	t1, 4
	bltl	t1, t2, 1b
	  sw	zero, 0(t1)	/* delay slot			*/

#ifdef CFG_RAMBOOT
	/* store pointer to exception_entry into EXCEPTION_ENTRY_PTR */
	la	t0, EXCEPTION_ENTRY_PTR
	la	t1, exception_entry
	b	1f
	  sw	t1, 0(t0)

	/* forrowing 5 instructions are copied into vectors on RAM */
  __ram_exception_entry:
	lui	k0, %hi(EXCEPTION_ENTRY_PTR)
	ori	k0, %lo(EXCEPTION_ENTRY_PTR)
	lw	k0, 0(k0)
	jr	k0
	  nop

1:
	la	t0, __ram_exception_entry	
	la	t1, 0x80000000

	lw	t2, 0(t0)
	lw	t3, 4(t0)
	lw	t4, 8(t0)
	lw	t5, 12(t0)
	lw	t6, 16(t0)
	sw	t2, 0(t1)
	sw	t3, 4(t1)
	sw	t4, 8(t1)
	sw	t5, 12(t1)
	sw	t6, 16(t1)
	cache	0x15, 0(t1)
	nop
	nop
	cache	0x10, 0(t1)
	nop
	nop

	sw	t2, 0x180(t1)
	sw	t3, 0x184(t1)
	sw	t4, 0x188(t1)
	sw	t5, 0x18c(t1)
	sw	t6, 0x190(t1)
	cache	0x15, 0x180(t1)
	nop
	nop
	cache	0x10, 0x180(t1)
	nop
	nop

	sw	t2, 0x200(t1)
	sw	t3, 0x204(t1)
	sw	t4, 0x208(t1)
	sw	t5, 0x20c(t1)
	sw	t6, 0x210(t1)
	cache	0x15, 0x200(t1)
	nop
	nop
	cache	0x10, 0x200(t1)
	nop
	nop

	/* clear BEV to enable RAM vector */
	mfc0	t0, $12
	li	t1, ~0x00400000
	and	t0, t1
	mtc0	t0, $12
	  nop
#endif

	move	a0, a1
	la	t9, board_init_r
	j	t9
	  move	a1, a2		/* delay slot			*/

	.end	relocate_code


	.set	push
	.set	noat
exception_stack:
	.space 64 * 4

	/* Exception handlers.
	 */
exception_entry:
romReserved:
romExcHandle:
	bal	1f
	  move	k0, ra
1:
	addiu	k1, ra, exception_stack - 1b
	move	ra, k0
	sw	$0, 0*4(k1)
	sw	$1, 1*4(k1)
	sw	$2, 2*4(k1)
	sw	$3, 3*4(k1)
	sw	$4, 4*4(k1)
	sw	$5, 5*4(k1)
	sw	$6, 6*4(k1)
	sw	$7, 7*4(k1)
	sw	$8, 8*4(k1)
	sw	$9, 9*4(k1)
	sw	$10, 10*4(k1)
	sw	$11, 11*4(k1)
	sw	$12, 12*4(k1)
	sw	$13, 13*4(k1)
	sw	$14, 14*4(k1)
	sw	$15, 15*4(k1)
	sw	$16, 16*4(k1)
	sw	$17, 17*4(k1)
	sw	$18, 18*4(k1)
	sw	$19, 19*4(k1)
	sw	$20, 20*4(k1)
	sw	$21, 21*4(k1)
	sw	$22, 22*4(k1)
	sw	$23, 23*4(k1)
	sw	$24, 24*4(k1)
	sw	$25, 25*4(k1)
	/* don't save $26:k1 */
	sw	$27, 27*4(k1)
	sw	$28, 28*4(k1)
	sw	$29, 29*4(k1)
	sw	$30, 30*4(k1)
	sw	$31, 31*4(k1)
	mflo	t0
	sw	t0, 32*4(k1)
	mfhi	t0
	sw	t0, 33*4(k1)
	la	t9, exception_handler
	jr	t9
	  move	a0, k1

	.set	pop

#ifdef CFG_RAMBOOT

bad_load_address:
	move	s0, a0
	move	s1, a1
	PUTS("UNEXPECTED LOAD ADDRESS!!!\r\n")
	PUTS("LOADED   0x")
	bal	_printhex
	  move	a0, s0
	PUTS("\r\n")
	PUTS("EXPECTED 0x")
	bal	_printhex
	  move	a0, s1
	PUTS("\r\n")

	bal	1f
	  nop
	.word	uboot_end_data
1:
	lw	v0, 0(ra)
	move	a0, s0
	move	a1, s1
1:
	lw	t0, 0(a0)	
	sw	t0, 0(a1)
	addiu	a1, 4
	bne	a1, v0, 1b
	  addiu	a0, 4

	la	a0, CFG_MONITOR_BASE
	jr	a0
	  nop


_printhex:
	addiu	sp, sp, -16
	sw	ra, 4(sp)
	sw	s0, 8(sp)
	sw	s1, 12(sp)

	move	s0, a0
	li	s1, 8
2:
	srl	a0, s0, 28
	andi	a0, a0, 0x0f
	addiu	a0, a0, -9
	blez	a0, 1f
	  addiu	a0, a0, 9 + 0x30

	addiu	a0, a0, 0x41 - 0x30 - 10
1:
	bal	_putchar
	  nop
	addiu	s1, -1
	bne	s1, zero, 2b
	  sll	s0, s0, 4

	lw	s1, 12(sp)
	lw	s0, 8(sp)
	lw	ra, 4(sp)
	jr	ra
	  addiu	sp, sp, 16


	.global _puts
	.type _puts,@function
_puts:
	addiu	sp, -8
	sw	s0, 4(sp)
	move	s0, ra
1:
	lb	a0, 0(s0)
	beqz	a0, 2f
	  addiu	s0, 1
	bal	_putchar
	  nop
	b	1b
	  nop
2:
	addiu	s0, 7
	li	a0, ~7
	and	ra, s0, a0
	lw	s0, 4(sp)
	jr	ra
	  addiu	sp, 8



/*
 * early stage character printing function for debugging
 */
	.global	_putchar
	.type _putchar,@function

#define UART_DATA	0x00
#define UART_RSR	0x04
#define UART_LCR_H	0x08
#define UART_LCR_M	0x0c
#define UART_LCR_L	0x10
#define UART_CR		0x14 
#define UART_FR		0x18
#define UART_IR		0x1c

_putchar:
	la	t0, 0xb2600000
1:
	lw	t1, UART_FR(t0)
	andi	t1, t1, 0x20	/* tx fifo full */
	bnez	t1, 1b
	  nop
	jr	ra
	  sb	a0, UART_DATA(t0)	/* delay slot */

#endif


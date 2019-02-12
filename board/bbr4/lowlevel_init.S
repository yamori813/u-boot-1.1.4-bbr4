/*
 * Low level initialization routine for ADMtek/Infineon adm5120 SoC.
 * original file name is memsetup.S
 *
 * (C) Copyright 2006
 * Robert Delien, <robert@delien.nl>
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
 *
 *
 * Reverse-engineered from register contents of adm5120 in
 * Sweex LB000021 'Broadband router' and Infineon data sheet.
 */


#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <adm5120_mpmc.h>
#include <adm51xx_switch.h>

	.globl  lowlevel_init
lowlevel_init:
	/* Setup pointers */
	la	a0, MPMC
	la	a1, SWITCH

	/*
	 * Initialize SRAM/ROM
	 */

	/* Set MEM_CONT__SR0S to 2MiB, set MEM_CONT__SDRS to 2Mib x32*/
	li	t0, (SWITCH_MEM_CONT__SR0S_2MB |	\
		     SWITCH_MEM_CONT__SDRS_2Mx32)
	sw	t0, SWITCH_MEM_CONT(a1)

	/* MPMC_SC1 */
	li	t0, (MPMC_SCx__BLS |			\
		     MPMC_SCx__MW_16BIT)
	sw	t0, MPMC_SC1(a0)
	/* MPMC_SWW1 */
	li	t0, (MPMC_SWWx__WWE & 0)
	sw	t0, MPMC_SWW1(a0)
	/* MPMC_SWO1 */
	li	t0, (MPMC_SWOx__SWO0 & 0)
	sw	t0, MPMC_SWO1(a0)
	/* MPMC_SWR1 */
	li	t0, (MPMC_SWRx__NMRW & 9)
	sw	t0, MPMC_SWR1(a0)
	/* MPMC_SWP1 */
	li	t0, (MPMC_SWPx__WPS & 31)
	sw	t0, MPMC_SWP1(a0)
	/* MPMC_SWWR1 */
	li	t0, (MPMC_SWWRx__WWS & 9)
	sw	t0, MPMC_SWWR1(a0)
	/* MPMC_SWT1 */
	li	t0, (MPMC_SWTx__WAITTURN & 5)
	sw	t0, MPMC_SWT1(a0)

#if (TEXT_BASE >= 0xBFC00000)
	/*
	 * Initialize SDRAM (if running from ROM)
	 */

	/* Disable address mirroring bit */
	lw	t0, MPMC_C(a0)
	li	t1, ~(MPMC_C__AM)
	and	t0, t0, t1
	sw	t0, MPMC_C(a0)

	/* Set MPMC_DRP__PCP to 1 */
	li	t0, (MPMC_DRP__PCP & 1)
	sw	t0, MPMC_DRP(a0)
	/* Set MPMC_DRAS__APCP to 4 */
	li	t0, (MPMC_DRAS__APCP & 4)
	sw	t0, MPMC_DRAS(a0)
	/* Set MPMC_DSREX__SRET to 16 */
	li	t0, (MPMC_DSREX__SRET & 15)
	sw	t0, MPMC_DSREX(a0)
	/* Set MPMC_DAPR__LACT */
	li	t0, (MPMC_DAPR__LACT & 1)
	sw	t0, MPMC_DAPR(a0)
	/* Set MPMC_DDAL__DACT */
	li	t0, (MPMC_DDAL__DACT & 3)
	sw	t0, MPMC_DDAL(a0)
	/* Set MPMC_DWR__WRT */
	li	t0, (MPMC_DWR__WRT & 1)
	sw	t0, MPMC_DWR(a0)
	/* Set MPMC_DRC__AACP */
	li	t0, (MPMC_DRC__AACP & 6)
	sw	t0, MPMC_DRC(a0)
	/* Set MPMC_DRFC__ARACP */
	li	t0, (MPMC_DRFC__ARACP & 6)
	sw	t0, MPMC_DRFC(a0)
	/* Set MPMC_DXSR__EACP */
	li	t0, (MPMC_DXSR__EACP & 7)
	sw	t0, MPMC_DXSR(a0)
	/* Set MPMC_DRRD__ABL */
	li	t0, (MPMC_DRRD__ABL & 1)
	sw	t0, MPMC_DRRD(a0)
	/* Set MPMC_DMRD__LACT */
	li	t0, (MPMC_DMRD__LACT & 1)
	sw	t0, MPMC_DMRD(a0)

	/* Switch to normal mode of operation, enable continueos clock, enable clock drive high */
	lw	t0, MPMC_DC(a0)
	li	t1, ~(MPMC_DC__SI)
	and	t0, t0, t1
	li	t1, (MPMC_DC__SI_NORMAL |	\
		     MPMC_DC__DMC | 		\
		     MPMC_DC__CE)
	or	t0, t0, t1
	sw	t0, MPMC_DC(a0)

	/* Disable bank 0 buffers */
	lw	t0, MPMC_DC0(a0)
	li	t1, ~(MPMC_DCx__BE)
	and	t0, t0, t1
	sw	t0, MPMC_DC0(a0)

	/* Disable bank 1 buffers */
	lw	t0, MPMC_DC1(a0)
	li	t1, ~(MPMC_DCx__BE)
	and	t0, t0, t1
	sw	t0, MPMC_DC1(a0)

	/* Wait for 1ms to make sure SDRAM power and clocks have stabilized */
	mtc0	zero, CP0_COUNT
	li	t0, CFG_CP0_COUNT_RATE/1000
1:
	mfc0	t1, CP0_COUNT
	ble	t1, t0, 1b
	nop

	/* Issue a NOP instruction to SDRAM modules */
	lw	t0, MPMC_DC(a0)
	li	t1, ~(MPMC_DC__SI)
	and	t0, t0, t1
	li	t1, MPMC_DC__SI_NOP
	or	t0, t0, t1
	sw	t0, MPMC_DC(a0)

	/* Wait 200us for NOP instruction to execute */
	mtc0	zero, CP0_COUNT
	li	t0, CFG_CP0_COUNT_RATE/5000
1:
	mfc0	t1, CP0_COUNT
	ble	t1, t0, 1b
	nop

	/* Precharge all SDRAM modules */
	lw	t0, MPMC_DC(a0)
	li	t1, ~(MPMC_DC__SI)
	and	t0, t0, t1
	li	t1, MPMC_DC__SI_PALL
	or	t0, t0, t1
	sw	t0, MPMC_DC(a0)

	/* Set dynamic refresh to every 32 HCLK cycles */
	li	t0, (MPMC_DR__RT & (32/0x10))
	sw	t0, MPMC_DR(a0)

	/* Wait 128 AHB cycles */
	li	t0,64
1:
	nop
	nop
	nop
	nop
	nop
	addiu	t0,t0,-1
	bnez	t0,1b
	nop

	/* Set dynamic refresh to every 1360 HCLK cycles */
	li	t0, (MPMC_DR__RT & (1360/0x10))
	sw	t0, MPMC_DR(a0)

	/* Set MPMC_DRC0__CASL and MPMC_DRC0__RASL to 2 */
	li	t0, 0x00000202
	sw	t0, MPMC_DRC0(a0)

	/* Set MPMC_DC0 to 0x14804280 same as original boot */
	li	t0, 0x14804280
	sw	t0, MPMC_DC0(a0)

#if (CONFIG_NR_DRAM_BANKS > 1)
	/* Set MPMC_DRC1__CASL and MPMC_DRC1__RASL to 2 */
	li	t0, 0x00000202
	sw	t0, MPMC_DRC1(a0)

	/* Set MPMC_DC1 to 0x14804500 */
	li	t0, 0x14804500
	sw	t0, MPMC_DC1(a0)
#endif /* (CONFIG_NR_DRAM_BANKS > 1) */

	/* Switch to mode programming mode */
	lw	t0, MPMC_DC(a0)
	li	t1, ~(MPMC_DC__SI)
	and	t0, t0, t1
	li	t1, MPMC_DC__SI_MODE
	or	t0, t0, t1
	sw	t0, MPMC_DC(a0)

	/* Set mode of bank 0 */
	li	t0,0xA0022000
	lw	t1,0(t0)

#if (CONFIG_NR_DRAM_BANKS > 1)
	/* Set mode of bank 1 */
	li	t0,0xA1022000
	lw	t1,0(t0)
#endif /* (CONFIG_NR_DRAM_BANKS > 1) */

	/* Switch to normal operation mode, use dynamic memory clock */
	lw	t0, MPMC_DC(a0)
	li	t1, ~(MPMC_DC__SI)
	and	t0, t0, t1
	li	t1, ~(MPMC_DC__CE)
	and	t0, t0, t1
	ori	t0, t0, MPMC_DC__SI_NORMAL
	sw	t0, MPMC_DC(a0)

	/* Enable buffers bank 0 */
	lw	t0, MPMC_DC0(a0)
	li	t1, MPMC_DCx__BE
	or	t0, t0, t1
	sw	t0, MPMC_DC0(a0)

#if (CONFIG_NR_DRAM_BANKS > 1)
	/* Enable buffers bank 1 */
	lw	t0, MPMC_DC1(a0)
	li	t1, MPMC_DCx__BE
	or	t0, t0, t1
	sw	t0,MPMC_DC1(a0)
#endif /* (CONFIG_NR_DRAM_BANKS > 1) */

#endif /* TEXT_BASE == 0xBFC00000 */

	j       ra
	nop


/*
 * Register definitions for memory controller of
 * ADMtek/Infineon adm5120 SoC.
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
 */

#ifndef ADM5120_MPMC_H
#define ADM5120_MPMC_H

#define MPMC_C				0x00000000
#define MPMC_S				0x00000004
#define MPMC_CONF			0x00000008
#define MPMC_DC				0x00000020
#define MPMC_DR				0x00000024
#define MPMC_DRP			0x00000030
#define MPMC_DRAS			0x00000034
#define MPMC_DSREX			0x00000038
#define MPMC_DAPR			0x0000003C
#define MPMC_DDAL			0x00000040
#define MPMC_DWR			0x00000044
#define MPMC_DRC			0x00000048
#define MPMC_DRFC			0x0000004C
#define MPMC_DXSR			0x00000050
#define MPMC_DRRD			0x00000054
#define MPMC_DMRD			0x00000058
#define MPMC_SEW			0x00000080
#define MPMC_DC0			0x00000100
#define MPMC_DRC0			0x00000104
#define MPMC_DC1			0x00000120
#define MPMC_DRC1			0x00000124
#define MPMC_DC2			0x00000140
#define MPMC_DRC2			0x00000144
#define MPMC_DC3			0x00000160
#define MPMC_DRC3			0x00000164
#define MPMC_SC0			0x00000200
#define MPMC_SWW0			0x00000204
#define MPMC_SWO0			0x00000208
#define MPMC_SWR0			0x0000020C
#define MPMC_SWP0			0x00000210
#define MPMC_SWWR0			0x00000214
#define MPMC_SWT0			0x00000218
#define MPMC_SC1			0x00000220
#define MPMC_SWW1			0x00000224
#define MPMC_SWO1			0x00000228
#define MPMC_SWR1			0x0000022C
#define MPMC_SWP1			0x00000230
#define MPMC_SWWR1			0x00000234
#define MPMC_SWT1			0x00000238

#define MPMC_C__AM			(1<<1)
#define MPMC_C__ME			(1<<0)

#define MPMC_DC__SI			(3<<7)
#define MPMC_DC__SI_NORMAL		(0<<7)
#define MPMC_DC__SI_MODE		(1<<7)
#define MPMC_DC__SI_PALL		(2<<7)
#define MPMC_DC__SI_NOP			(3<<7)
#define MPMC_DC__DMC			(1<<1)
#define MPMC_DC__CE			(1<<0)

#define MPMC_DRP__PCP			0x0000000F

#define MPMC_DRAS__APCP			0x0000000F

#define MPMC_DSREX__SRET		0x0000000F

#define MPMC_DAPR__LACT			0x0000000F

#define MPMC_DDAL__DACT			0x0000000F

#define MPMC_DWR__WRT			0x0000000F

#define MPMC_DRC__AACP			0x0000001F

#define MPMC_DRFC__ARACP		0x0000001F

#define MPMC_DXSR__EACP			0x0000001F

#define MPMC_DRRD__ABL			0x0000000F

#define MPMC_DMRD__LACT			0x0000000F

#define MPMC_DR__RT			0x000007FF

#define MPMC_DCx__BE			(1<<19)

#define MPMC_SCx__BLS			(1<<7)
#define MPMC_SCx__MW			(3<<0)
#define MPMC_SCx__MW_8BIT		(0<<0)
#define MPMC_SCx__MW_16BIT		(1<<0)
#define MPMC_SCx__MW_32BIT		(2<<0)
#define MPMC_SCx__MW_RES		(3<<0)

#define MPMC_SWWx__WWE			0x0000000F

#define MPMC_SWOx__SWO0			0x0000000F

#define MPMC_SWRx__NMRW			0x0000000F

#define MPMC_SWPx__WPS			0x0000001F

#define MPMC_SWWRx__WWS			0x0000001F

#define MPMC_SWTx__WAITTURN		0x0000000F

#endif /* ADM5120_MPMC_H */

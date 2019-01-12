#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_MIPS32		1  /* MIPS32 CPU core	*/
#define CONFIG_ADM5120		1
#define CONFIG_BBR4		1

#define CONFIG_BOOTDELAY	2	/* autoboot after 2 seconds	*/

#define CFG_PL010_SERIAL
#define CFG_UART_PL010_CLOCK   62500000        /* 62.5MHz in AMD5120P */
#define CFG_SERIAL0            0xB2600000
#define CFG_SERIAL1            0xB2800000
#define CONFIG_CONS_INDEX      0
#define CONFIG_PL01x_PORTS	{ (void *) (CFG_SERIAL0), (void *) (CFG_SERIAL1) }
#define UART_PL010_BAUD2DIVISOR(baud)  ((CFG_UART_PL010_CLOCK+(0x8*(baud)))/(0x10*(baud))-1)


#define CONFIG_BAUDRATE		115200

/* valid baudrates */
#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

#define	CONFIG_TIMESTAMP		/* Print image info with timestamp */
#define	CONFIG_BOOTARGS		"console=ttyS0,${baudrate} root=/dev/sda1"
#define	CONFIG_BOOTCOMMAND	"bootm 0xbfd10000"
#define CONFIG_BZIP2	/* include support for bzip2 compressed images */
#define CONFIG_MISC_INIT_R	/* call misc_init_r()		*/

#define  CONFIG_COMMANDS	(CONFIG_CMD_DFL | CFG_CMD_PING | CFG_CMD_LOADS | CFG_CMD_LOADB | CFG_CMD_ELF | CFG_CMD_FLASH)

#define	CFG_LONGHELP				/* undef to save memory      */

#include <cmd_confdefs.h>

/*
 * Miscellaneous configurable options
 */

#define	CFG_PROMPT		"BBR-4MG/HG # "	/* Monitor Command Prompt    */

#define	CFG_CBSIZE		256		/* Console I/O Buffer Size   */
#define	CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)  /* Print Buffer Size */
#define	CFG_MAXARGS		16		/* max number of command args*/

#define CFG_MALLOC_LEN		2560*1024

#define CFG_BOOTPARAMS_LEN	128*1024

#define CFG_MHZ			175

#define CFG_HZ                  (CFG_MHZ * 1000000) /* FIXME causes overflow in net.c */

#define CFG_SDRAM_BASE		0x80000000     /* Cached addr */

#define	CFG_LOAD_ADDR		0x80400000     /* default load address	*/

#define CFG_MEMTEST_START	0x80200000
#define CFG_MEMTEST_END		0x80800000

#define CPU_CLOCK_RATE		175000000	/* 175 MHz clock for the MIPS core      */
#define CFG_CP0_COUNT_RATE	((CPU_CLOCK_RATE)/2)

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CFG_MAX_FLASH_SECT	(256)	/* max number of sectors on one chip */

#define CFG_FLASH_BANKS_LIST {0xbfc00000}

#define CFG_FLASH_CFI           1
#define CFG_FLASH_CFI_DRIVER    1
#define CFG_FLASH_PROTECTION	1

/* The following #defines are needed to get flash environment right */
#define	CFG_MONITOR_BASE	TEXT_BASE
#define	CFG_MONITOR_LEN		(192 << 10)

#define CFG_INIT_SP_OFFSET	0x400000

#define CFG_FLASH_BASE		0xbfc00000

/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Write */

#define CFG_ENV_IS_IN_FLASH	1

/* Address and size of Primary Environment Sector	*/
//#define CFG_ENV_ADDR		0xbfc20000
#define CFG_ENV_ADDR		0xbfc30000
#define CFG_ENV_SECT_SIZE	0x10000

#define CONFIG_FLASH_16BIT

#define CONFIG_NET_MULTI

#define CONFIG_MEMSIZE_IN_BYTES

#define CONFIG_ENV_OVERWRITE

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CFG_DCACHE_SIZE		8192
#define CFG_ICACHE_SIZE		8192
#define CFG_CACHELINE_SIZE	16


#if CFG_MONITOR_BASE < CFG_FLASH_BASE
#define CFG_RAMBOOT
#else
#undef CFG_RAMBOOT
#endif

#define SWITCH			0xB2000000
#define MPMC			0xB1000000


#endif	/* __CONFIG_H */

#ifdef CONFIG_ADM5120

#include <asm/addrspace.h>

#define ADM5120_UART1_BASE	0x12800000
#define ADM5120_UART0_BASE	0x12600000
#define ADM5120_SYSC_BASE	0x12400000
#define ADM5120_INTC_BASE	0x12200000
#define ADM5120_SCR_BASE	0x12000000
#define ADM5120_MIPS_BASE	0x11a00000
#define ADM5120_PCIIO_BASE	0x11500000
#define ADM5120_PCIIO_END	(0x115ffff0-1)
#define ADM5120_PCI_CONFIG_ADDR	0x115ffff0
#define ADM5120_PCI_CONFIG_DATA	0x115ffff8
#define ADM5120_PCIMEM_BASE	0x11400000
#define ADM5120_PCIMEM_END	0x114fffff
#define ADM5120_USB_BASE	0x11200000
#define ADM5120_MPMC_BASE	0x11000000

#define SCR_CODE 		(0x00)
#define SCR_SFTRESET	(0x04)
#define SCR_SWRESET	(0x0c)
#define SCR_GLOBAL_ST	(0x10)
#define SCR_PHY_ST		(0x14)
#define SCR_PORT_ST	(0x18)
#define SCR_MEM_CONTROL	(0x1c)
#define SCR_SW_CONF	(0x20)
#define SCR_CPUP_CONF	(0x24)
#define SCR_PORT_CONF0	(0x28)
#define SCR_PORT_CONF1	(0x2c)
#define SCR_PORT_CONF2	(0x30)
#define SCR_VLAN_GI	(0x40)
#define SCR_VLAN_GII	(0x44)
#define SCR_SEND_TRIG	(0x48)
#define SCR_SRCH_CMD	(0x4c)
#define SCR_ADDR_ST0	(0x50)
#define SCR_ADDR_ST1	(0x54)
#define SCR_MAC_WT0	(0x58)
#define SCR_MAC_WT1	(0x5c)
#define SCR_BW_CNTL0	(0x60)
#define SCR_BW_CNTL1	(0x64)
#define SCR_PHY_CNTL0	(0x68)
#define SCR_PHY_CNTL1	(0x6c)
#define SCR_FC_TH	(0x70)
#define SCR_ADJ_PORT_TH	(0x74)
#define SCR_PORT_TH	(0x78)
#define SCR_PHY_CNTL2	(0x7c)
#define SCR_PHY_CNTL3	(0x80)
#define SCR_PRI_CNTL	(0x84)
#define SCR_VRAN_PRI	(0x88)
#define SCR_TOS_EN	(0x8c)
#define SCR_TOS_map0	(0x90)
#define SCR_TOS_map1	(0x94)
#define SCR_CUSTOM_PRI1	(0x98)
#define SCR_CUSTOM_PRI2	(0x9c)
#define SCR_EMPTY_CNT	(0xa4)
#define SCR_PORT_CNT_SEL	(0xa8)
#define SCR_PORT_CNT	(0xac)
#define SCR_INT_ST	(0xb0)
#define SCR_INT_MSK	(0xb4)
#define SCR_GPIO_CONF0	(0xb8)
#define SCR_GPIO_CONF2	(0xbc)
#define SCR_WATCHDOG0	(0xc0)
#define SCR_WATCHDOG1	(0xc4)
#define SCR_SWAPIN	(0xc8)
#define SCR_SWAPOUT	(0xcc)
#define SCR_SEND_HBADDR	(0xd0)
#define SCR_SEND_LBADDR	(0xd4)
#define SCR_RECEIVE_HBADDR	(0xd8)
#define SCR_RECEIVE_LBADDR	(0xdc)
#define SCR_SEND_HWADDR	(0xe0)
#define SCR_SEND_LWADDR	(0xe4)
#define SCR_RECEIVE_HWADDR	(0xe8)
#define SCR_RECEIVE_LWADDR	(0xec)
#define SCR_TIMER_INT	(0xf0)
#define SCR_TIMER	(0xf4)
#define SCR_PORT0_LED	(0x100)
#define SCR_PORT1_LED	(0x104)
#define SCR_PORT2_LED	(0x108)
#define SCR_PORT3_LED	(0x10c)
#define SCR_PORT4_LED	(0x110)

#define SOFTWARE_RESET						0x1
#define SW_CPU_PORT_DISABLE					0x00000001
#define SW_DISABLE_PORT_MASK					0x0000003F

static inline void scr_write(int ofs, u32 value)
{
	*(volatile u32 *)(KSEG1ADDR(ADM5120_SCR_BASE + ofs)) = value;
}

static inline u32 scr_read(int ofs)
{
	return *(volatile u32 *)(KSEG1ADDR(ADM5120_SCR_BASE + ofs));
}


#endif

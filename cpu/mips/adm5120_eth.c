/*
 * adm5120_eth.c -- ADM5120 Ether net driver
 *
 * Copyright (c) 2006 SATO Masuhiro <thomas@fenix.ne.jp>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/*
 * Usage for this driver is difficult to explain, since ADM5120 Ether
 * net controler has programable HUB.
 *
 * - ADM5120 has 6 HUBs, which makes VLAN.
 * - The software send/receive packets to/from CPU port on the VLANs
 *   instead of physical Ether NET ports.
 *
 * Setting of the HUB can be overrided by environment value, "ethports", like
 *
 * ethports=0x4f,0x50
 *
 * This parameter makes.
 * VLAN0: port0, port1, port2, port3, cpu
 * VLAN1: port4, cpu
 * 
 */

#include <config.h>

#ifdef CONFIG_ADM5120

#include <common.h>
#include <malloc.h>
#include <net.h>
#include <asm/io.h>
#include <asm/addrspace.h>
#include <asm/adm5120.h>

#undef DEBUG_RX
#undef DEBUG_TX
#undef DEBUG

#define ETH_PORT_CPU	0x40
#define ETH_PORT_GIGA	0x20	/* GMII port */
#define ETH_PORT_4	0x10
#define ETH_PORT_3	0x08
#define ETH_PORT_2	0x04
#define ETH_PORT_1	0x02
#define ETH_PORT_0	0x01

#define RX_HP_BUF_COUNT  4
#define TX_HP_BUF_COUNT  4
#define RX_BUF_COUNT  4
#define TX_BUF_COUNT  4
#define BUFSIZE		1536

#define DESC_OWN_SC	0x80000000
#define DESC_RING_END	0x10000000

#define PHYS_PORTS	5
#define VLAN_PORTS	6

#define ETH_INT_MASK	0x01fdefff

#ifndef SYNC
#define	SYNC asm volatile ("sync")
#endif

struct buf_desc {
	u32 buffer1;
	u32 buffer2;
	u32 buffer1_length;
	u32 flags;
} __attribute__((packed));


static int adm5120_eth_init(struct eth_device* dev, bd_t * bd);
static void adm5120_eth_halt(struct eth_device *dev);
static int adm5120_eth_recv(struct eth_device *dev);
static int adm5120_eth_send(struct eth_device *dev, volatile void *packet, int length);
static int adm5120_eth_update(void);

static void init_buffer(void);
static int init_buf_ring(struct buf_desc *buf_desc, int count, u8 *buf, u32 own);
static void adm5120_switch_init(void);
static int tx_hp_update(void);
static int tx_update(void);
static int rx_hp_update(void);
static int rx_update(void);


/* all ports are connected to VLAN0 */
static u8 port_map[VLAN_PORTS] = {
	ETH_PORT_CPU | ETH_PORT_4 | ETH_PORT_3 | ETH_PORT_2 | ETH_PORT_1 | ETH_PORT_0,
};
static u32 port_enable;

static int irq;
static int tx_put, tx_done, tx_hp_put, tx_hp_done;
static int rx_put, rx_done, rx_suspend, rx_hp_put, rx_hp_done, rx_hp_suspend;
/* buffers for high priority packets */
static u8 tx_hp_buf[RX_HP_BUF_COUNT * BUFSIZE] __attribute__((aligned(32)));
static u8 rx_hp_buf[TX_HP_BUF_COUNT * BUFSIZE] __attribute__((aligned(32)));
/* buffers for normal priority packets */
static u8 tx_buf[RX_BUF_COUNT * BUFSIZE] __attribute__((aligned(32)));
static u8 rx_buf[TX_BUF_COUNT * BUFSIZE] __attribute__((aligned(32)));

struct buf_desc	rx_hp_desc[RX_HP_BUF_COUNT] __attribute__((aligned(32)));
struct buf_desc	tx_hp_desc[TX_HP_BUF_COUNT] __attribute__((aligned(32)));
;
struct buf_desc	rx_desc[RX_BUF_COUNT] __attribute__((aligned(32)));
;
struct buf_desc	tx_desc[TX_BUF_COUNT] __attribute__((aligned(32)));


int
adm5120_eth_initialize(bd_t *bis)
{
	char *p;
	int i;

	if ((p = getenv("ethports")) != NULL) {
		long ports;

		i = 0;
		memset(port_map, 0, sizeof(port_map));
		while (*p) {
			ports = simple_strtol(p, &p, 0x10);
			if (ports)
				port_map[i++] = (u8)ports;
			if (*p == 0)
				break;
			if (*p != ',' && *p != ':')
				break;
			p++;
		}
	}

	adm5120_switch_init();

#ifdef DEBUG
	printf("adm5120_eth port groups ");
#endif
	for (i = 0; i < VLAN_PORTS && port_map[i] != 0; i++) {
		struct eth_device *dev;
#ifdef DEBUG
		int j;
		u8 bit;

		bit = 0x01;
		printf("%d:", i);

		for (j = 0; j < PHYS_PORTS + 2; j++) {
			if (bit & port_map[i]) {
				if (j > 0)
					printf(".", j);
				printf("%d", j);
			}
			bit <<= 1;
		}
		printf(" ");
#endif

		dev = (struct eth_device *)malloc(sizeof(struct eth_device));
		if (dev == NULL) {
			printf("\nadm5120_eth_initialize(): malloc() failed\n");
			break;
		}
		memset(dev, 0, sizeof(struct eth_device));

		sprintf(dev->name, "adm%d", i);
		dev->iobase = 0;
		dev->priv   = (void *)i;
		dev->init   = adm5120_eth_init;
		dev->halt   = adm5120_eth_halt;
		dev->send   = adm5120_eth_send;
		dev->recv   = adm5120_eth_recv;

		eth_register(dev);
	}

	return 1;
}



static void
adm5120_switch_init()
{
	int i;

	scr_write(SCR_CPUP_CONF, 0x01);
	scr_write(SCR_PORT_CONF0, 0x3f);
	SYNC;
	udelay(10000);

	scr_write(SCR_VLAN_GI, 
		port_map[0] | (port_map[1] << 8) |
		(port_map[2] << 8) | (port_map[3] << 8));

	scr_write(SCR_VLAN_GII,
		port_map[4] | (port_map[5] << 8));

	SYNC;

	port_enable = 0;
	for (i = 0; i < VLAN_PORTS; i++)
		port_enable |= port_map[i];

#ifdef DEBUG
	printf("port_enable = %08x\n", port_enable);
#endif


	/* CPU port to be disabled, CRC padding, disable unknown packets */
	scr_write(SCR_CPUP_CONF, 0x01 |
		0x02 |	/* CRC padding */
		((port_enable & 0x3f) << 9)); /* disable unknown packets */
		
	/* enable ports, multicast, back pressure */
	scr_write(SCR_PORT_CONF0,
		(~port_enable & 0x3f) |
		((port_enable & 0x3f) << 16) |
		((port_enable & 0x3f) << 8));

	scr_write(SCR_PORT_CONF1, (port_enable & 0x3f) << 20);

	if (port_enable & ETH_PORT_GIGA) {
		scr_write(SCR_PORT_CONF2,
			0x01 | /* AN enable */
			0x02 |/* 100M */	
			0x08 | /* full duplex */
			0x30 /* flow control */);
	} else {
		scr_write(SCR_PORT_CONF2,
			0x02 |/* 100M */	
			0x08 | /* full duplex */
			0x30 | /* flow control */
			0x100 /* disable check TXC */ );
	}

	SYNC;

	udelay(10000);

	scr_write(SCR_PHY_CNTL2,
		(port_enable & 0x1f) |		/* auto negotiation */
		((port_enable & 0x1f) << 5) |	/* speed control */
		((port_enable & 0x1f) << 10) |	/* duplex control */
		((port_enable & 0x1f) << 15) |	/* FC rec */
		((port_enable & 0x1f) << 20) | 	/* disable reset */
		((port_enable & 0x1f) << 25) |
		0x80000000); 			/* XXX undocumented */
#if 1
	scr_write(SCR_PHY_CNTL3, 0x0001410b);
#else
	scr_write(SCR_PHY_CNTL3, 0x0000410b);
#endif
	scr_write(SCR_INT_MSK, ETH_INT_MASK);
	scr_write(SCR_INT_ST, scr_read(SCR_INT_ST));

	SYNC;
}


static int
init_buf_ring(struct buf_desc *buf_desc, int count, u8 *buf, u32 own)
{
	int i;

	memset(buf, 0, BUFSIZE * count);
	for (i = 0; i < count; i++) {
		buf_desc[i].buffer1 = own |
			((i == count - 1) ? DESC_RING_END : 0) |
			virt_to_phys(buf + (i * BUFSIZE));
		buf_desc[i].buffer2 = 0;
		buf_desc[i].buffer1_length = BUFSIZE;
		buf_desc[i].flags = 0;
	}
	dma_cache_wback_inv((unsigned long)buf, BUFSIZE * count);
	dma_cache_wback_inv((unsigned long)buf_desc, sizeof(struct buf_desc) * count);
	
	return i;
}


static void
init_buffer()
{
	tx_put = 0;
	tx_done = 0;
	tx_hp_put = 0;
	tx_hp_done = 0;
	rx_put = 0;
	rx_done = 0;
	rx_suspend = RX_BUF_COUNT;
	rx_hp_put = 0;
	rx_hp_done = 0;
	rx_hp_suspend = RX_HP_BUF_COUNT;
	init_buf_ring(tx_hp_desc, TX_HP_BUF_COUNT, tx_hp_buf, 0);
	init_buf_ring(rx_hp_desc, RX_HP_BUF_COUNT, rx_hp_buf, DESC_OWN_SC);
	init_buf_ring(tx_desc, TX_BUF_COUNT, tx_buf, 0);
	init_buf_ring(rx_desc, RX_BUF_COUNT, rx_buf, DESC_OWN_SC);

	scr_write(SCR_SEND_HBADDR, virt_to_phys(tx_hp_desc));
	scr_write(SCR_SEND_LBADDR, virt_to_phys(tx_desc));
	scr_write(SCR_RECEIVE_HBADDR, virt_to_phys(rx_hp_desc));
	scr_write(SCR_RECEIVE_LBADDR, virt_to_phys(rx_desc));
	SYNC;
}


static int
adm5120_eth_init(struct eth_device *dev, bd_t *bd)
{
#ifdef DEBUG
	printf("adm5120_eth_init(): priv=%p dev->enetaddr=%02x:%02x:%02x:%02x:%02x:%02x\n",
			dev->priv,
			dev->enetaddr[0],
			dev->enetaddr[1],
			dev->enetaddr[2],
			dev->enetaddr[3],
			dev->enetaddr[4],
			dev->enetaddr[5]
			);
#endif
	init_buffer();

	scr_write(SCR_CPUP_CONF, scr_read(SCR_CPUP_CONF) & ~0x01);

	scr_write(SCR_MAC_WT1, 
		(dev->enetaddr[5] << 24) | (dev->enetaddr[4] << 16) |
		(dev->enetaddr[3] << 8) | dev->enetaddr[2]);
	scr_write(SCR_MAC_WT0, 
		(dev->enetaddr[1] << 24) | (dev->enetaddr[0] << 16) |
		((unsigned)dev->priv << 3) | 0x2041);

	SYNC;

	while ((scr_read(SCR_MAC_WT0) & 0x2) == 0)	/* wtMAC_done */
		;

	scr_write(SCR_INT_MSK, ETH_INT_MASK);
	scr_write(SCR_INT_ST, scr_read(SCR_INT_ST));

	SYNC;


	return(1);
}



static void
adm5120_eth_halt(struct eth_device *dev)
{
	scr_write(SCR_CPUP_CONF, scr_read(SCR_CPUP_CONF) | 0x01);
	SYNC;
	udelay(10000);
}



static int
adm5120_eth_send(struct eth_device *dev, volatile void *packet, int length)
{
	u8 *bp;
	int retry;
	struct buf_desc *dp;

	retry =  100;
	while (tx_put - tx_done == TX_BUF_COUNT && --retry) {
		udelay(1000);
		adm5120_eth_update();
	}
	if (retry == 0)
		return 0;

	bp = tx_buf + BUFSIZE * (tx_put % TX_BUF_COUNT);
	memcpy(bp, packet, length);

	dp = &tx_desc[tx_put % TX_BUF_COUNT];
	dp->buffer1_length = length;
	if (length < 60)
		length = 60;
	dp->flags = (length << 16) | (1 << (unsigned)dev->priv);
	dp->buffer1 |= DESC_OWN_SC;

	dma_cache_wback_inv((unsigned long)bp, length);
	dma_cache_wback_inv((unsigned long)dp, sizeof(struct buf_desc));
	scr_write(SCR_SEND_TRIG, 1);
	SYNC;

#ifdef DEBUG_TX
	printf("adm5120_eth_tx(), tx_put=%d, desc=%p, buf=%p, len=%d\n",
		tx_put, dp, bp, length);
#endif

	tx_put++;

	return length;
}



static int
adm5120_eth_recv(struct eth_device *dev)
{
	volatile struct buf_desc *dp;
	u8 *bp;
	int len;
	unsigned int_st_clear;

	len = 0;

	if (rx_put == rx_done && rx_hp_put == rx_hp_done)
		adm5120_eth_update();

	int_st_clear = 0;
	 while (rx_hp_put > rx_hp_done) {
		bp = rx_hp_buf + BUFSIZE * (rx_hp_done % RX_BUF_COUNT);
		dp = &rx_hp_desc[rx_hp_done % RX_BUF_COUNT];
		
		len = dp->flags >> 16; 
		if (len > 0) {
			NetReceive(bp, len);
			dma_cache_inv((unsigned long)bp, len);
		}
		dp->buffer1 |= DESC_OWN_SC;
		dma_cache_wback_inv((unsigned long)dp, sizeof(struct buf_desc));
		rx_hp_done++;	
		rx_hp_suspend++;	
		int_st_clear |= 0x10;
#ifdef DEBUG_RX
		//if (len)
			printf("adm5120_eth_rx() pull %d bytes. rx_hp_done=%d\n",
			len, rx_hp_done);
#endif
	} 

	while (rx_put > rx_done) {
		bp = rx_buf + BUFSIZE * (rx_done % RX_BUF_COUNT);
		dp = &rx_desc[rx_done % RX_BUF_COUNT];
		
		len = dp->flags >> 16; 
		if (len > 0) {
			NetReceive(bp, len);
			dma_cache_inv((unsigned long)bp, len);
		}
		dp->buffer1 |= DESC_OWN_SC;
		dma_cache_wback_inv((unsigned long)dp, sizeof(struct buf_desc));
		rx_done++;	
		rx_suspend++;	
		int_st_clear |= 0x20;
#ifdef DEBUG_RX
		//if (len)
			printf("adm5120_eth_rx() pull %d bytes. rx_done=%d\n",
			len, rx_done);
#endif
	}

	scr_write(SCR_INT_ST, int_st_clear);

	return 0;
}



static int rx_update()
{
	int n;
	n = 0;

	while (rx_put < rx_suspend) {
		volatile struct buf_desc *dp;
		dp = &rx_desc[rx_put % RX_BUF_COUNT];
		dma_cache_inv((unsigned long)dp, sizeof(struct buf_desc));
		if ((dp->buffer1 & DESC_OWN_SC) != 0)
			break;
		rx_put++;
		n++;
#ifdef DEBUG_RX
		printf("rx_update() rx_put=%0d, rx_suspend=%d\n",
			rx_put, rx_suspend);
#endif
	}

	return n;
}



static int rx_hp_update()
{
	int n;
	n = 0;

	while (rx_hp_put < rx_hp_suspend) {
		volatile struct buf_desc *dp;
		dp = &rx_hp_desc[rx_put % RX_BUF_COUNT];
		dma_cache_inv((unsigned long)dp, sizeof(struct buf_desc));
		if ((dp->buffer1 & DESC_OWN_SC) != 0)
			break;
		rx_hp_put++;
		n++;
	}

	return n;
}

static int tx_update()
{
	int n;

	n = 0;

	while (tx_put > tx_done) {
		volatile struct buf_desc *dp;

		dp = &tx_desc[rx_put % RX_BUF_COUNT];
		dma_cache_inv((unsigned long)dp, sizeof(struct buf_desc));
		if ((dp->buffer1 & DESC_OWN_SC) != 0)
			break;
		tx_done++;
		n++;
#ifdef DEBUG_TX
		printf("tx_update() tx_put=%0d, tx_done=%d\n",
			tx_put, tx_done);
#endif
	}
	if (tx_put > tx_done) {
		scr_write(SCR_SEND_TRIG, 1);
		SYNC;
	}

	return n;
}


static int tx_hp_update()
{
	int n;

	n = 0;

	while (tx_hp_put > tx_hp_done) {
		volatile struct buf_desc *dp;

		dp = &tx_hp_desc[rx_put % RX_BUF_COUNT];
		dma_cache_inv((unsigned long)dp, sizeof(struct buf_desc));
		if ((dp->buffer1 & DESC_OWN_SC) != 0)
			break;
		tx_hp_done++;
		n++;
#ifdef DEBUG_TX
		printf("tx_hp_update() tx_hp_put=%0d, tx_hp_done=%d\n",
			tx_hp_put, tx_hp_done);
#endif
	}

	return n;
}



static int adm5120_eth_update()
{
	int n = 0;
	unsigned int_st = scr_read(SCR_INT_ST);

	if (int_st & 0x04)
		n += rx_hp_update();
	if (int_st & 0x08)
		n += rx_update();
	if (int_st & 0x01)
		n += tx_hp_update();
	if (int_st & 0x02)
		n += tx_update();

	scr_write(SCR_INT_ST, int_st & 0xf);
	
	return n;
}

#endif

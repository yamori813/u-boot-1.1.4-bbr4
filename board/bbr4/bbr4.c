#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>

#define BBR4_ETHADDR_AREA	0xbfd00016


long int initdram(int board_type)
{
	return 0x800000;
}


int checkboard (void)
{
	return 0;
}


int misc_init_r (void)
{
	DECLARE_GLOBAL_DATA_PTR;

	if (getenv ("ethaddr") == NULL) {
		unsigned char mac[6];
		char str[64];

		memcpy(mac, BBR4_ETHADDR_AREA, 6);

		sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		setenv("ethaddr", str);
		memcpy (gd->bd->bi_enetaddr, mac, 6);
		mac[5]++;
		sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		setenv("eth1addr", str);

		return 0;
	}
}


/*
mdio-tool allow for direct access to mdio registers in a network phy.

Routines are based on mii-tool: http://freecode.com/projects/mii-tool

mdio-tool comes with ABSOLUTELY NO WARRANTY; Use with care!

Copyright (C) 2013 Pieter Voorthuijsen

mdio-tool is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

mdio-tool is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with mdio-tool.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/sockios.h>

#ifndef __GLIBC__
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#endif
#include "mii.h"

#define MAX_ETH		8		/* Maximum # of interfaces */

static int skfd = -1;		/* AF_INET socket for ioctl() calls. */
static struct ifreq ifr;

/*--------------------------------------------------------------------*/

static int mdio_read(int skfd, int location)
{
	struct mii_data *mii = (struct mii_data *)&ifr.ifr_data;
	mii->reg_num = location;
	if (ioctl(skfd, SIOCGMIIREG, &ifr) < 0) {
		fprintf(stderr, "SIOCGMIIREG on %s failed: %s\n", ifr.ifr_name,
			strerror(errno));
		return -1;
	}
	return mii->val_out;
}

static void mdio_write(int skfd, int location, int value)
{
	struct mii_data *mii = (struct mii_data *)&ifr.ifr_data;
	mii->reg_num = location;
	mii->val_in = value;
	if (ioctl(skfd, SIOCSMIIREG, &ifr) < 0) {
		fprintf(stderr, "SIOCSMIIREG on %s failed: %s\n", ifr.ifr_name,
			strerror(errno));
	}
}

static int mdio_read_extended_reg(int skfd, int mmd, int location)
{
	mdio_write(skfd, MII_REGCR, mmd);
	mdio_write(skfd, MII_ADDAR, location);
	mdio_write(skfd, MII_REGCR, mmd | MII_ADDAR_DATA_NOINC);
	return mdio_read(skfd, MII_ADDAR);
}

static void mdio_write_extended_reg(int skfd, int mmd, int location, int value)
{
	mdio_write(skfd, MII_REGCR, mmd);
	mdio_write(skfd, MII_ADDAR, location);
	mdio_write(skfd, MII_REGCR, mmd | MII_ADDAR_DATA_NOINC);
	mdio_write(skfd, MII_ADDAR, value);
}

int main(int argc, char **argv)
{
	int addr, dev, val, mmd;
	struct mii_data *mii = (struct mii_data *)&ifr.ifr_data;

	if(argc < 2) {
		printf("Usage:\nmdio-tool [r/w/er/ew] <dev> [mmd] <reg> [val]\n"
			"\tOperation:\n"
			"\t  r: read register\n"
			"\t  w: write register\n"
			"\t  er: read register from extended reg space\n"
			"\t  ew: write register from extended reg space\n"
			"\tdev: as in 'eth0'\n"
			"\tmmd: for 'er' and 'ew' operations, specify the MDIO's Manageable Device\n"
			"\treg: the register's address to read from/write to\n"
			"\tval: the value to write in the register\n");
		return 0;
	}

	/* Open a basic socket. */
	if ((skfd = socket(AF_INET, SOCK_DGRAM,0)) < 0) {
		perror("socket");
		return -1;
	}

	/* Get the vitals from the interface. */
	strncpy(ifr.ifr_name, argv[2], IFNAMSIZ);
	if (ioctl(skfd, SIOCGMIIPHY, &ifr) < 0) {
		if (errno != ENODEV)
			fprintf(stderr, "SIOCGMIIPHY on '%s' failed: %s\n",
					argv[2], strerror(errno));
		return -1;
	}

	if(argv[1][0] == 'e') {
		if(argv[1][1] == 'r') {
			mmd = strtol(argv[3], NULL, 16);
			addr = strtol(argv[4], NULL, 16);
			printf("0x%.4x\n", mdio_read_extended_reg(skfd, mmd, addr));
		}
		else if(argv[1][1] == 'w') {
			mmd = strtol(argv[3], NULL, 16);
			addr = strtol(argv[4], NULL, 16);
			val = strtol(argv[5], NULL, 16);
			mdio_write_extended_reg(skfd, mmd, addr, val);
		}
		else {
			printf("Fout!\n");
		}
	}
	else if(argv[1][0] == 'r') {
		addr = strtol(argv[3], NULL, 16);
		printf("0x%.4x\n", mdio_read(skfd, addr));
	}
	else if(argv[1][0] == 'w') {
		addr = strtol(argv[3], NULL, 16);
		val = strtol(argv[4], NULL, 16);
		mdio_write(skfd, addr, val);
	}
	else {
		printf("Fout!\n");
	}

	close(skfd);
}

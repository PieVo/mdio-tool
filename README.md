mdio-tool
=========
This is tool to read and write MII registers from ethernet physiscals under linux.
It has been tested with Realtek and Marvell PHY's connected via PCIe.

Syntax:
	mdio-tool [r/w] [devname] [addr] <value>

Building this tool requires kernel sources (includes). Make sure to set the path in the Makefile


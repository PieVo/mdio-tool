mdio-tool
=========
This is tool to read and write MII registers from ethernet physicals under linux.
It has been tested with Realtek and Marvell PHY's connected via PCIe and should work
with all drivers implementing the mdio ioctls.

Syntax:
	mdio-tool [r/w] [devname] [addr] <value>



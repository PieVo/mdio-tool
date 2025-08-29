mdio-tool
=========
This is tool to read and write MII registers from ethernet physicals under linux.
It has been tested with Realtek and Marvell PHY's connected via PCIe and should work
with all drivers implementing the mdio ioctls.

mdio-tool comes with ABSOLUTELY NO WARRANTY; Use with care!

Syntax:
	mdio-tool [r/w/er/ew] <dev> [mmd] <reg> [val]

        Operation:
          r: read register
          w: write register
          er: read register from extended reg space
          ew: write register from extended reg space
        dev: as in 'eth0'
        mmd: for 'er' and 'ew' operations, specify the MDIO's Manageable Device
        reg: the register's address to read from/write to
        val: the value to write in the register

	./mdio-tool w eth0 0x10 0x0
	./mdio-tool r eth0 0x0
        ./mdio-tool er eth0 0x1F 0x0460
        ./mdio-tool ew eth0 0x1F 0x0460 0x02





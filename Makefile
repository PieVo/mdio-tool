CROSS_COMPILE=arm-linux-gnueabihf

all:
	$(CROSS_COMPILE)-gcc mdio-tool.c -o mdio-tool
clean:
	rm mdio-tool

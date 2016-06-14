CROSS_COMPILE ?= arm-linux-gnueabihf-
CC	?= $(CROSS_COMPILE)gcc

all:
	$(CC) mdio-tool.c -o mdio-tool
clean:
	rm mdio-tool

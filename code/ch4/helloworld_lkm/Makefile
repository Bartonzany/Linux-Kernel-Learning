# ch4/helloworld_lkm/Makefile
# Simplest kernel module Makefile

PWD   := $(shell pwd)
KDIR  := /mnt/workspace/github-learning/linux-5.15/out/arm64
obj-m += helloworld_lkm.o

all:
	make -C $(KDIR) M=$(PWD) modules
install:
	make -C $(KDIR) M=$(PWD) modules_install
clean:
	make -C $(KDIR) M=$(PWD) clean
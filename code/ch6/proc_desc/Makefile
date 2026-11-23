KDIR ?= /home/linxi/DevKit/Projects/github_learning/linux-5.15/out/x86_64

obj-m += proc_desc.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

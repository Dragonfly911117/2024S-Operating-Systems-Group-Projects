obj-m += jiffies.o
obj-m += seconds.o
obj-m += task_info.o


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

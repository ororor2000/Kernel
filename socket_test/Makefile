obj-m += kernel_nl.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc user_nl.c -o user_nl -g
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm user_nl

client:
	gcc user_nl.c -o user_nl -g
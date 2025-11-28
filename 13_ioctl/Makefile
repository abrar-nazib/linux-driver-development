obj-m += ioctl.o

all: testapp
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

testapp: test.c
	gcc $< -o $@
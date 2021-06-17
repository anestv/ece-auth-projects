
CC = gcc

LDLIBS = -lrt
#RASPBERRY_CFLAGS = -march=armv6zk -mcpu=arm1176jzf-s -mfpu=vfp -marm

SRC_C := $(wildcard *.c)

.PHONY: all clean raspberry-run

all: embedded_final_hw
#	gcc embedded_final_hw.c -lrt -o emb_hw

embedded_final_hw: $(SRC_C) common.h

raspberry: CC = arm-linux-gnueabi-gcc-9
raspberry: CFLAGS = -march=armv6zk -mcpu=arm1176jzf-s -mfpu=vfp -marm
# Those variables are set only for that rule. For pi zero
raspberry: $(SRC_C) common.h
	$(CC) $(CFLAGS) $(SRC_C) $(LDLIBS) -o $@
#	arm-linux-gnueabi-gcc-9 $(RASPBERRY_CFLAGS) $(SRC_C) $(LDLIBS) -o raspberry

raspberry-run: raspberry
	scp raspberry pi@192.168.7.2:/home/pi/embedded
	ssh -t pi@192.168.7.2 timeout -k2 30s embedded/raspberry
# -t to put it in terminal mode, and show live output.
# timeout: send sigterm after 30s and sigkill after 32s

clean:
	rm -f embedded_final_hw raspberry
# -f so that it doesn't throw error when files don't exist


# A simple makefile for building program composed of C/asm source files.
# Builds for ARM target using cross-development toolchain. 
# Refer to CS107E guide to Make for more information on makefiles

APPLICATION = apps/clock
MODULES = timer.o gpio.o abort.o

CFLAGS = -Og -g -Wall -std=c99 -ffreestanding
LDFLAGS = -nostdlib -T memmap
LDLIBS =

all : $(APPLICATION).bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o $(MODULES) start.o cstart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@
	
%.o: %.s
	arm-none-eabi-as $< -o $@

%.list: %.o
	arm-none-eabi-objdump --no-show-raw-insn -d $< > $@

install: $(APPLICATION).bin
	rpi-install.py $<

# 'make test' builds test application and uploads it to Pi
test: tests/test_clock.bin
	rpi-install.py $<

clean:
	rm -f *.o *.bin *.elf *.list *~
	rm -f apps/*.o apps/*.bin apps/*.elf apps/*.list apps/*~
	rm -f tests/*.o tests/*.bin tests/*.elf tests/*.list tests/*~

.PHONY: all clean install test

.PRECIOUS: %.elf %.o

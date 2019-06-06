COMPILER_BASE = /mnt/d/Documents/Programming/gcc/arm-eabi-8.3
COMPILER_BIN = $(COMPILER_BASE)/arm-eabi/bin
COMPILER_INC = $(COMPILER_BASE)/arm-eabi/include
COMPILER_LIB = $(COMPILER_BASE)/arm-eabi/lib

ARMGNU = $(COMPILER_BIN)

GCC = $(COMPILER_BASE)/bin/arm-eabi-gcc

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

PROJECT_NAME = test

SRCS = blinker04.c
OBJS = vectors.o $(PROJECT_NAME).o

gcc : $(PROJECT_NAME).hex kernel.img

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s

vectors.o : vectors.s
	$(ARMGNU)/as vectors.s -o vectors.o

$(PROJECT_NAME).o : $(SRCS)
	$(GCC) $(COPS) -c $(SRCS) -o $(PROJECT_NAME).o

$(PROJECT_NAME).elf : memmap $(OBJS) 
	$(ARMGNU)/ld.bfd $(OBJS) -T memmap -o $(PROJECT_NAME).elf
	$(ARMGNU)/objdump -D $(PROJECT_NAME).elf > $(PROJECT_NAME).list

$(PROJECT_NAME).bin : $(PROJECT_NAME).elf
	$(ARMGNU)/objcopy $(PROJECT_NAME).elf -O binary $(PROJECT_NAME).bin

$(PROJECT_NAME).hex : $(PROJECT_NAME).elf
	$(ARMGNU)/objcopy $(PROJECT_NAME).elf -O ihex $(PROJECT_NAME).hex

kernel.img : $(PROJECT_NAME).bin
	cp $(PROJECT_NAME).bin kernel.img

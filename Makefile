COMPILER_BASE = /mnt/d/Documents/Programming/gcc/arm-eabi-8.3
COMPILER_BIN = $(COMPILER_BASE)/arm-eabi/bin
COMPILER_INC = $(COMPILER_BASE)/arm-eabi/include
COMPILER_LIB = $(COMPILER_BASE)/arm-eabi/lib

ARMGNU = $(COMPILER_BIN)

GCC = $(COMPILER_BASE)/bin/arm-eabi-gcc

AOPS = --warn --fatal-warnings -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard

PROJECT_NAME = blinker04
LINK_SCRIPT = linker_script

ASMS = vectors.s
SRCS = BCM2836_HW.c PWM.c DMA.c WS2812B_LED_Drv.c
OBJS = $(patsubst %.s, %.o, $(ASMS)) $(patsubst %.c, %.o, $(SRCS)) $(PROJECT_NAME).o

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

$(patsubst %.s, %.o, $(ASMS)): %.o : %.s
	$(ARMGNU)/as $(AOPS) $^ -o $@

$(patsubst %.c, %.o, $(SRCS)): %.o : %.c %.h
	$(GCC) $(COPS) -c $< -o $@
	$(ARMGNU)/objdump -D $@ > $*.list

$(PROJECT_NAME).o : $(PROJECT_NAME).c
	$(GCC) $(COPS) -c $^ -o $@

$(PROJECT_NAME).elf : $(LINK_SCRIPT) $(OBJS) 
	$(ARMGNU)/ld.bfd $(OBJS) -T $(LINK_SCRIPT) -o $(PROJECT_NAME).elf
	$(ARMGNU)/objdump -D $(PROJECT_NAME).elf > $(PROJECT_NAME).list

$(PROJECT_NAME).bin : $(PROJECT_NAME).elf
	$(ARMGNU)/objcopy $(PROJECT_NAME).elf -O binary $(PROJECT_NAME).bin

$(PROJECT_NAME).hex : $(PROJECT_NAME).elf
	$(ARMGNU)/objcopy $(PROJECT_NAME).elf -O ihex $(PROJECT_NAME).hex

kernel.img : $(PROJECT_NAME).bin
	cp $(PROJECT_NAME).bin kernel.img

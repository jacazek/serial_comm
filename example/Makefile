PROGRAM = main
MCU_TARGET     = atmega2560
#MCU_TARGET	= atmega2561
#MCU_TARGET     = atmega328p

DEPENDS = serialcomm


OPTIMIZE = -O2

CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

# Override is only needed by avr-lib build system.
override INCLUDE       = ../include/
override LIBS          = ../dist
override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET)
override LDFLAGS       = -Wl,-Map,$(PROGRAM).map

all: $(PROGRAM).elf lst text eeprom
force_all: clean all
.PHONY: force_all
clean:
	rm -rf *.o *.hex *.bin *.srec *.elf *.map *.lst
$(PROGRAM).elf: $(PROGRAM).c
	#$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(INCLUDE) -o $@ $^ -L$(LIBS) -l$(DEPENDS)

lst:  $(PROGRAM).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@



# Rules for building the .text rom images
text: hex bin srec

hex:  $(PROGRAM).hex
bin:  $(PROGRAM).bin
srec: $(PROGRAM).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@



# Rules for building the .eeprom rom images
eeprom: ehex ebin esrec

ehex:  $(PROGRAM)_eeprom.hex
ebin:  $(PROGRAM)_eeprom.bin
esrec: $(PROGRAM)_eeprom.srec

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@ \
	|| { echo empty $@ not generated; exit 0; }

%_eeprom.srec: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@ \
	|| { echo empty $@ not generated; exit 0; }

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@ \
	|| { echo empty $@ not generated; exit 0; }



# Every thing below here is used by avr-libc's build system and can be ignored
# by the casual user.
FIG2DEV                 = fig2dev
EXTRA_CLEAN_FILES       = *.hex *.bin *.srec

dox: eps png pdf

eps: $(PROGRAM).eps
png: $(PROGRAM).png
pdf: $(PROGRAM).pdf

%.eps: %.fig
	$(FIG2DEV) -L eps $< $@

%.pdf: %.fig
	$(FIG2DEV) -L pdf $< $@

%.png: %.fig
	$(FIG2DEV) -L png $< $@


# program the system
program:
	avrdude -p m2560 -P /dev/ttyACM0 -b 115200 -c stk500v2 -U flash:w:main.hex
	#sudo avrdude -p m2561 -P /dev/ttyACM0 -b 19200 -c stk500v1 -U flash:w:$(BUILDPATH)main.hex



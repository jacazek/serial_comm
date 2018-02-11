PROGRAM = serialcomm
MCU_TARGET     = atmega2560
#MCU_TARGET	= atmega2561
#MCU_TARGET     = atmega328p

TESTS = serialcomm_test
MOCKS = 

INCLUDEPATH = ./include/
SRCPATH = ./libc/
DEPENDS = 
BUILDPATH = ./build/
OBJ_PATH = $(BUILDPATH)/
TEST_BUILDPATH = ./test/build/
DISTPATH = ./dist/


OPTIMIZE = -O2

AR = avr-ar
CC = avr-gcc
CC_TEST = gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

MAKE_DIR = mkdir -p

# Override is only needed by avr-lib build system.
override ARFLAGS       = rcs
override CFLAGS        = -I$(INCLUDEPATH) -g -Wall -c $(OPTIMIZE) -mmcu=$(MCU_TARGET)
override LDFLAGS       = -Wl,-Map,$(BUILDPATH)$(PROGRAM).map

all: PATHS $(PROGRAM).a
$(PROGRAM).a: $(PROGRAM).o
	#$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS)
	$(AR) $(ARFLAGS) $(DISTPATH)lib$@ $(BUILDPATH)*.o

$(PROGRAM).o: $(SRCPATH)
	$(CC) $(CFLAGS) $(LDFLAGS) $^*.c
	mv *.o $(BUILDPATH)

PATHS:
	$(MAKE_DIR) $(DISTPATH) $(BUILDPATH) $(TEST_BUILDPATH)

clean:
	rm -rf $(DISTPATH) $(BUILDPATH)

test: clean $(TESTS)

$(TESTS): %_test: $(SRCPATH)%.c $(TEST_BUILDPATH)
	gcc -D TEST -o $(TEST_BUILDPATH)$@.o ./test/$@.c $(TEST_DEPENDS) $< ../Unity/src/unity.c
	$(TEST_BUILDPATH)$@.o

$(TEST_BUILDPATH): PATHS

lst:  $(BUILDPATH)$(PROGRAM).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@



# Rules for building the .text rom images
text: hex bin srec

hex:  $(BUILDPATH)$(PROGRAM).hex
bin:  $(BUILDPATH)$(PROGRAM).bin
srec: $(BUILDPATH)$(PROGRAM).srec

%.hex: %.a
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.a
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.a
	$(OBJCOPY) -j .text -j .data -O binary $< $@




# Every thing below here is used by avr-libc's build system and can be ignored
# by the casual user.
FIG2DEV                 = fig2dev
EXTRA_CLEAN_FILES       = *.hex *.bin *.srec

dox: eps png pdf

eps: $(BUILDPATH)$(PROGRAM).eps
png: $(BUILDPATH)$(PROGRAM).png
pdf: $(BUILDPATH)$(PROGRAM).pdf

%.eps: %.fig
	$(FIG2DEV) -L eps $< $@

%.pdf: %.fig
	$(FIG2DEV) -L pdf $< $@

%.png: %.fig
	$(FIG2DEV) -L png $< $@


# program the system
program:
#	sudo avrdude -p m2560 -P /dev/ttyACM0 -b 115200 -c stk500v2 -F -u -U flash:w:main.hex
	#sudo avrdude -C/root/.arduino15/packages/MegaCore/hardware/avr/2.0.0/avrdude.conf -p atmega2561 -P /dev/ttyACM0 -b 19200 -c stk500v1 -F -u -U flash:w:main.hex
	sudo avrdude -p m2561 -P /dev/ttyACM0 -b 19200 -c stk500v1 -F -u -U flash:w:$(BUILDPATH)main.hex



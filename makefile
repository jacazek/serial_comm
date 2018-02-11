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

all: PATHS $(PROGRAM).a tests
force_all: clean all
.PHONY: force_all
$(PROGRAM).a: $(PROGRAM).o
	#$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS)
	$(AR) $(ARFLAGS) $(DISTPATH)lib$@ $(BUILDPATH)*.o

$(PROGRAM).o: $(SRCPATH)
	$(CC) $(CFLAGS) $(LDFLAGS) $^*.c
	mv *.o $(BUILDPATH)

PATHS:
	$(MAKE_DIR) $(DISTPATH) $(BUILDPATH) $(TEST_BUILDPATH)

tests:
	cd test && $(MAKE) 

example:
	cd example && $(MAKE)

clean:
	rm -rf $(DISTPATH) $(BUILDPATH)
	cd test && $(MAKE) clean

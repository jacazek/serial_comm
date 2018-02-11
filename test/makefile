TESTS = configureSerial_test transmitSerialAsync_test
INCLUDEPATH = ../include/
SRCPATH = ../libc/
BUILDPATH = ./build/
MOCKS = 
TEST_DEPENDS = 
LIBS = ../dist
LIBRARY = serialcomm

MAKE_DIR = mkdir -p

all: $(BUILDPATH) $(TESTS)
force_all: clean all
.PHONY: force_all
$(TESTS): %_test: $(SRCPATH)%.c
	#gcc -D TEST -D __AVR_ATmega2560__ -o $(BUILDPATH)$@.o $@.c $< ./Unity/unity.c -I/usr/lib/avr/include/ -I$(INCLUDEPATH)
	gcc -D TEST -o $(BUILDPATH)$@.o $@.c $< ./Unity/unity.c -Imocks/ -I$(INCLUDEPATH)
	$(BUILDPATH)$@.o

$(BUILDPATH):
	$(MAKE_DIR) $@

clean:
	rm -rf $(BUILDPATH)
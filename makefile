PROJECT = serialcomm
MCU_TARGET     = atmega2560


DIST = ./dist/
DIST_LIB = $(DIST)lib/
DIST_INCLUDE = $(DIST)include/
EXAMPLE_LIB = ./example/lib/
EXAMPLE_INCLUDE = ./example/include/
BUILD = ./build/
TEST = ./test/
MOCKS = ./test/mocks/
DIRECTORIES = $(DIST) $(BUILD) $(TEST) $(MOCKS) $(DIST_LIB) $(DIST_INCLUDE)

INCLUDE = ./include/
LIBC = ./libc/
SOURCE_FILES = $(filter-out $(wildcard $(LIBC)*_test.c), $(wildcard $(LIBC)*.c))
TEST_FILES = $(patsubst $(LIBC)%,%,$(patsubst %_test.c,%_test,$(wildcard $(LIBC)*_test.c)))

OPTIMIZE = -Os

REMOVE = rm -rf
MAKE_DIR = mkdir -p
CHANGE_DIR = cd

AR = avr-ar
CC = avr-gcc
GCC = gcc
CFLAGS = -g -Wall $(OPTIMIZE) -ffunction-sections -mmcu=$(MCU_TARGET)
LDFLAGS = 
ARFLAGS = rsc
OBJCOPY = avr-objcopy


all: $(DIRECTORIES) $(PROJECT).a


$(PROJECT).a: $(PROJECT).binary
	$(AR) $(ARFLAGS) -o $(DIST_LIB)lib$@ $(BUILD)*.o
	cp $(INCLUDE)* $(DIST_INCLUDE)


$(PROJECT).binary: $(SOURCE_FILES)
	$(CC) $(CFLAGS) -c $(LDFLAGS) $^ -I$(INCLUDE)
	mv *.o $(BUILD)



$(DIRECTORIES):
	$(MAKE_DIR) $@

tests: $(TEST_FILES)

$(TEST_FILES): %_test: $(LIBC)%.c
	$(GCC) -m64 -D TEST -o $(TEST)$@.o $(LIBC)$@.c $< ./Unity/unity.c -I$(MOCKS) -I$(INCLUDE)
	$(TEST)$@.o

example: example/main.c main.hex

$(BUILD)main.elf: example/main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o main.elf $< -I$(DIST_INCLUDE) -I$(EXAMPLE_INCLUDE) -L$(DIST_LIB) -L$(EXAMPLE_LIB) -lserialcomm -lring_buffer
	mv *.elf $(BUILD)

main.hex: $(BUILD)main.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $(DIST)$@

clean:
	$(REMOVE) $(DIST) $(BUILD)
	$(REMOVE) $(TEST)*.o
#include <serialcomm.h>
#include "./ring_buffer.h"
#include <avr/io.h>
#include <stdlib.h>

#define F_CPU 16000000	// 16 MHz oscillator.
#define BaudRate 9600
#define MYUBRR ((F_CPU / 16 / BaudRate ) - 1)

static char buffer[8];
static char flag = 0;


void transmitComplete() {
		transmitSerialAsync("Hello!\n", &transmitComplete);
}

void configure() {
	struct SerialSettings settings;
	settings.baudrate = MYUBRR;
	settings.TX = 1;
	configureSerial(settings);
}

int main() {
	RingBuffer ringBuffer = createRingBuffer(10, buffer);
	configure();
	transmitComplete();
	while(1) {
	}
}
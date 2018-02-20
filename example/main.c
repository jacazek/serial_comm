#include <serialcomm.h>
#include <ring_buffer.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000	// 16 MHz oscillator.
#define BaudRate 9600
#define MYUBRR ((F_CPU / 16 / BaudRate ) - 1)

void transmitComplete() {

}

void dataReceieved(uint8_t value) {
	if (value == '\n' || value == '\r') {
		flushTransmitter();
	} else {
		uint8_t toSend[1] = { value };
		transmitSerialAsync(toSend, 1);
	}
}

void configure() {
	SerialTransmitterSettings transmitterSettings;
	transmitterSettings.bufferSize = 10;
	SerialSettings settings;
	settings.baudrate = MYUBRR;
	configureSerial(settings);
	startTransmitter(transmitterSettings);

}

int main() {
	configure();
	sei();
	startReceiver(dataReceieved);
	while(1) {
		
	}
}
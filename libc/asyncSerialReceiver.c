#include <serialcomm.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <ring_buffer.h>

dataCallback tempCallback;
static uint8_t started = 0;

int8_t startReceiver(dataCallback clientCallback) {
	int8_t retval = -1;
	if (!started) {
		UCSR0B |= (1<<RXCIE0);
		UCSR0A |= (1<<RXC0);
		tempCallback = clientCallback;
		retval = 0;
		started = 1;
	}
	return retval;
}

int8_t stopReceiver() {
	int8_t retval = -1;
	if (started) {
		UCSR0A &= ~(1<<RXC0);
		UCSR0B &= ~(1<<RXCIE0);
		retval = 0;
		started = 0;
	}
	return retval;
}

ISR(USART0_RX_vect) {
	uint8_t value = UDR0;
	UCSR0A &= ~(1<<RXC0);
	tempCallback(value);
}
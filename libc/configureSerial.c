#include <avr/io.h>
#include <serialcomm.h>

void configureSerial(struct SerialSettings settings) {
	// enable global interrupts

	/*Set baud rate */ 
	UBRR0H = (byte)(settings.baudrate>>8);
	UBRR0L = (byte) settings.baudrate;
	/* Enable receiver and transmitter   */
	UCSR0B = (settings.RX<<RXEN0)|(settings.TX<<TXEN0);
	/* Frame format: 8data, No parity, 1stop bit */ 
	UCSR0C = (1<<UCSZ01|1<<UCSZ00);
}
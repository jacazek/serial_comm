#include <serialcomm.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

callback transmitCompleteCallback;
byte *transmission;
int transmissionLength;
int currentPosition;

void stageNextByteForTransmission() {
	UDR0 = transmission[currentPosition];
	currentPosition ++;
}

void enableEmptyRegisterInterruptHandler() {
	UCSR0B |= (1<<UDRIE0);
}

void disableEmptyRegisterInterruptHandler() {
	UCSR0B &= ~(1<<UDRIE0);
}

void enableTransmitCompleteInterruptHandler() {
	UCSR0B |= (1<<TXCIE0);
}

void disableTransmitCompleteInterruptHandler() {
	UCSR0B &= ~(1<<TXCIE0);
}

void transmitSerialAsync(byte bytes[], callback transmitCompleteCallback) {
	// if transmission in progress... don't do anything yet.  we will handle this later
	transmission = bytes;
	transmissionLength = strlen(bytes);
	currentPosition = 0;
	stageNextByteForTransmission();
	enableEmptyRegisterInterruptHandler();
	enableTransmitCompleteInterruptHandler();

	sei();
}

/**
	Interrupt service routine for UART Transmit Data Register Empty
*/
ISR(USART0_UDRE_vect) {
	if (currentPosition < transmissionLength) {
		stageNextByteForTransmission();
	} else {
		disableEmptyRegisterInterruptHandler();
	}
}

/**
	Interrupt service routine for UART Transmit Complete 
*/
ISR(USART0_TX_vect) {
	(*transmitCompleteCallback)();
	disableTransmitCompleteInterruptHandler();
}
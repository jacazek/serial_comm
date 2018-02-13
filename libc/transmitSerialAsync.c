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

void enableGlobalInterrupts() {
	sei();
}

void disableGlobalInterrupts() {
	cli();
}

short isTransmitComplete() {
	return UCSR0A & (1<<TXC0);
}

short isDataRegisterEmpty() {
	return (UCSR0A & (1<<UDRE0));
}

void transmitSerialAsync(byte bytes[], callback callback) {
	// if transmission in progress... don't do anything yet.  we will handle this later
	transmitCompleteCallback = callback;
	transmission = bytes;
	transmissionLength = strlen(bytes);
	currentPosition = 0;
	disableGlobalInterrupts();
	stageNextByteForTransmission();
	enableEmptyRegisterInterruptHandler();
	enableTransmitCompleteInterruptHandler();
	enableGlobalInterrupts();
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
	// There is a possibility that when a byte is shifted out (transmit complete)
	// that the next byte to be transmitted is not yet staged (data register empty)
	// This means that the transmit complete interrupt will be triggered and so will the data register empty
	// Since the data register empty interrupt runs first, it will stage a new byte and flag the register as full
	// we should not callback to client unless the data register is empty
	if (isDataRegisterEmpty()) {
		(*transmitCompleteCallback)();
	}
}
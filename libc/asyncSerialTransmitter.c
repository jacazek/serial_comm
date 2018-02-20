#include <serialcomm.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <ring_buffer.h>
#include <string.h>
#include <stdlib.h>

RingBuffer* ringBuffer;
uint8_t started = 0;


int8_t startTransmitter(SerialTransmitterSettings settings) {
	int8_t retval = -1;
	if (!started) {
		ringBuffer = createRingBuffer(settings.bufferSize);
		UCSR0B |= (1<<TXEN0);
		started = 1;
		retval = 0;
	}
	return retval;
}

void stopTransmitter() {
	if (started) {
		free(ringBuffer);
		UCSR0B &= ~(1<<TXEN0);
		started = 0;
	}
}


int8_t transmitSerialAsync(uint8_t *value, uint8_t size) {
	int8_t retval = -1;
	if (started) {
		uint8_t i = 0;
		for (i = 0; i < size; i++) {
			writeRingBuffer(ringBuffer, value[i]);
		}
		retval = i;
	}
	return retval;
}

void disableEmptyRegisterInterruptHandler() {
	UCSR0B &= ~(1<<UDRIE0);
}

void enableEmptyRegisterInterruptHandler() {
	UCSR0B |= (1<<UDRIE0);
}

void stageNextByteForTransmission() {
	char value = 0;
	if (readRingBuffer(ringBuffer, &value) == 0) {
		UDR0 = value;
	} else {
		disableEmptyRegisterInterruptHandler();
	}
}


/**

 */
int8_t flushTransmitter() {
	int8_t retval = -1;
	if (started) {
		if (ringBuffer->filled > 0) {
			retval = 0;
			stageNextByteForTransmission();
			enableEmptyRegisterInterruptHandler();
		}
	}
	return retval;
}

/**
	Interrupt service routine for UART Transmit Data Register Empty
*/
ISR(USART0_UDRE_vect) {
	if (ringBuffer->filled > 0) {
		stageNextByteForTransmission();
	} else {
		disableEmptyRegisterInterruptHandler();
	}
}


// void enableTransmitCompleteInterruptHandler() {
// 	UCSR0B |= (1<<TXCIE0);
// }

// void disableTransmitCompleteInterruptHandler() {
// 	UCSR0B &= ~(1<<TXCIE0);
// }

// short isTransmitComplete() {
// 	return UCSR0A & (1<<TXC0);
// }

// short isDataRegisterEmpty() {
// 	return (UCSR0A & (1<<UDRE0));
// }


/**
	Interrupt service routine for UART Transmit Complete 
*/
// ISR(USART0_TX_vect) {
// 	// There is a possibility that when a byte is shifted out (transmit complete)
// 	// that the next byte to be transmitted is not yet staged (data register empty)
// 	// This means that the transmit complete interrupt will be triggered and so will the data register empty
// 	// Since the data register empty interrupt runs first, it will stage a new byte and flag the register as full
// 	// we should not callback to client unless the data register is empty
// 	if (isDataRegisterEmpty()) {
// 		//transmitCompleteCallback();
// 	}
// }
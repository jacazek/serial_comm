#ifndef _MOCK_IO_H_
#define _MOCK_IO_H_

#include <common_types.h>
#define DEFINE_REGISTER(name) extern volatile byte name;
// Expose "registers" so code and tests can read/write to it
// Bit values taken from datasheet for atmega2560/2561
// This may not work well for atmega8 and atmega328
DEFINE_REGISTER(UBRR0H)
DEFINE_REGISTER(UBRR0L)
DEFINE_REGISTER(UCSR0A)
#define RXC0 7
#define TXC0 6
#define UDRE0 5
DEFINE_REGISTER(UCSR0B)
#define TXCIE0 6
#define UDRIE0 5
#define RXEN0 4 
#define TXEN0 3
#define UCSZ02 2
DEFINE_REGISTER(UCSR0C)
#define UMSEL01 7
#define UMSEL00 6
#define UPMN1 5
#define UPMN0 4
#define USBS0 3
#define UCSZ01 2
#define UCSZ00 1
DEFINE_REGISTER(UDR0)

#endif
#ifndef _MOCK_INTERRUPT_H_
#define _MOCK_INTERRUPT_H_
// Macro to create ISR function signature
#define ISR(vector) void vector##_function()
// Macro to declare an ISR
#define DECLARE_ISR(vector) extern void vector##_function();

/**
	Declare interrupt service routines we would like to test
	Use the same vector names as devined in the AVR io.h header
*/
DECLARE_ISR(USART0_UDRE_vect)
DECLARE_ISR(USART0_TX_vect)
extern void sei();

#endif 
#include "../Unity/unity.h"
#include <serialcomm.h>
#include <common_types.h>
// We mock the AVR library
#include <avr/io.h> 
#include <avr/interrupt.h>



// Setup our mocked data
volatile byte UCSR0A = 0;
volatile byte UCSR0B = 0;
volatile byte UCSR0C = 0;
volatile byte UDR0 = 0;
short seiCallCount = 0;
short callbackCount = 0;

void setUp() {
	UCSR0A = 1;
	UCSR0B = 0;
	UCSR0C = 0;
	UDR0 = 0;
	seiCallCount = 0;
	callbackCount = 0;
}

void cli() {
	
}

void sei() {
	seiCallCount++;
}

void doNothing() {

}

void incrementCallbackCount() {
	callbackCount++;
}

void flagDataRegisterFull() {
	UCSR0A &= ~(1<<UDRE0);
}

void flagDataRegisterEmpty() {
	UCSR0A |= (1<<UDRE0);
}

void flagTransmitComple() {
	UCSR0A |= (1<<TXC0);
}

void flagTransmitInProgress() {
	UCSR0A &= ~(1<<TXC0);
}

void GivenACharacter_WhenTransmitAsync_CharacterStagedForTransmission() {
	TEST_ASSERT_EQUAL_UINT8(0, UDR0);
	char expectedValue = 'A';
	transmitSerialAsync("A", &doNothing);
	TEST_ASSERT_EQUAL_UINT8(expectedValue, UDR0);
}

void GivenMultipleCharacters_WhenTransmitAsync_MultipleCharactersStagedForTransmission() {
	char characters[] = "ABC";
	short i = 0;
	transmitSerialAsync(characters, &doNothing);

	for (i = 0; i < 3; i++) {
		TEST_ASSERT_EQUAL_UINT8(characters[i], UDR0);
		USART0_UDRE_vect_function();
	}
}

void GivenTransmissionByte_WhenTransmitAsync_DataEmptyInterruptEnabled() {
	TEST_ASSERT_BIT_LOW(UDRIE0, UCSR0B);
	transmitSerialAsync("A", &doNothing);
	TEST_ASSERT_BIT_HIGH(UDRIE0, UCSR0B);
}

void GivenTransmissionByte_WhenTransmitAsync_TransmitCompleteInterruptEnabled() {
	TEST_ASSERT_BIT_LOW(TXCIE0, UCSR0B);
	transmitSerialAsync("A", &doNothing);
	TEST_ASSERT_BIT_HIGH(TXCIE0, UCSR0B);
}

void GivenTransmissionByte_WhenTransmitAsync_GlobalInterruptsEnabled() {
	TEST_ASSERT_EQUAL_UINT16(0, seiCallCount);
	transmitSerialAsync("A", &doNothing);
	TEST_ASSERT_EQUAL_UINT16(1, seiCallCount);
}

void GivenATransmission_WhenTransmitAsync_CallbackInvokedWhenTransmissionComplete() {
	TEST_ASSERT_EQUAL_UINT16(0, callbackCount);
	flagTransmitComple();
	flagDataRegisterEmpty();
	transmitSerialAsync("A", &incrementCallbackCount);
	USART0_TX_vect_function();
	TEST_ASSERT_EQUAL_UINT16(1, callbackCount);
}

void GivenATransmission_WhenTransmitAsync_CallbackNotInvoked_WhenTransmissionComplete_ButRegisterNotEmpty() {
	TEST_ASSERT_EQUAL_UINT16(0, callbackCount);
	flagTransmitComple();
	flagDataRegisterFull();
	transmitSerialAsync("A", &incrementCallbackCount);
	USART0_TX_vect_function();
	TEST_ASSERT_EQUAL_UINT16(0, callbackCount);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(GivenACharacter_WhenTransmitAsync_CharacterStagedForTransmission);
	RUN_TEST(GivenMultipleCharacters_WhenTransmitAsync_MultipleCharactersStagedForTransmission);
	RUN_TEST(GivenTransmissionByte_WhenTransmitAsync_DataEmptyInterruptEnabled);
	RUN_TEST(GivenTransmissionByte_WhenTransmitAsync_TransmitCompleteInterruptEnabled);
	RUN_TEST(GivenTransmissionByte_WhenTransmitAsync_GlobalInterruptsEnabled);
	RUN_TEST(GivenATransmission_WhenTransmitAsync_CallbackInvokedWhenTransmissionComplete);
	RUN_TEST(GivenATransmission_WhenTransmitAsync_CallbackNotInvoked_WhenTransmissionComplete_ButRegisterNotEmpty);
	return UNITY_END();
}
#include "Unity/unity.h"
#include <serialcomm.h>
#include <common_types.h>
// We mock the AVR library
#include <avr/io.h> 
#include <avr/interrupt.h>



// Setup our mocked data
volatile byte UBRR0H = 0;
volatile byte UBRR0L = 0;
volatile byte UCSR0B = 0;
volatile byte UCSR0C = 0;
volatile byte UDR0 = 0;
short seiCallCount = 0;

void setUp() {
	UBRR0H = 0;
	UBRR0L = 0;
	UCSR0B = 0;
	UCSR0C = 0;
	UDR0 = 0;
	seiCallCount = 0;
}

void cli() {
	
}

void sei() {
	seiCallCount++;
}

void doNothing() {

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

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(GivenACharacter_WhenTransmitAsync_CharacterStagedForTransmission);
	RUN_TEST(GivenMultipleCharacters_WhenTransmitAsync_MultipleCharactersStagedForTransmission);
	RUN_TEST(GivenTransmissionByte_WhenTransmitAsync_DataEmptyInterruptEnabled);
	RUN_TEST(GivenTransmissionByte_WhenTransmitAsync_TransmitCompleteInterruptEnabled);
	RUN_TEST(GivenTransmissionByte_WhenTransmitAsync_GlobalInterruptsEnabled);
	return UNITY_END();
}
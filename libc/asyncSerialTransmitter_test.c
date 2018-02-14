#include "../Unity/unity.h"
#include <serialcomm.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../test/mocks/io_mocks.c"

void setUp() {
	UDR0 = 0;
	UCSR0B = 0;
}

void tearDown() {
	stopTransmitter();
}

void WhenTransmitterStarted_ThenTransmitterEnabled() {
	SerialTransmitterSettings settings;
	settings.bufferSize = 2;
	startTransmitter(settings);
	TEST_ASSERT_BIT_HIGH(TXEN0, UCSR0B);
}

void GivenTransmissionIsStarted_WhenTransmitSerialAsync_ThenCharactersBuffered() {
	SerialTransmitterSettings settings;
	settings.bufferSize = 2;
	startTransmitter(settings);
	char expected[] = "ABC";
	TEST_ASSERT_EQUAL_UINT8(3, transmitSerialAsync(expected, 3));
}

void GivenTransmitterIsStarted_WhenStopTransmitter_ThenTransmitterDisabled() {
	SerialTransmitterSettings settings;
	settings.bufferSize = 2;
	startTransmitter(settings);
	TEST_ASSERT_BIT_HIGH(TXEN0, UCSR0B);
	stopTransmitter();
	TEST_ASSERT_BIT_LOW(TXEN0, UCSR0B);
}

void GivenTransmitterDisabled_WhenFlushTransmitter_FlushFails() {
	SerialTransmitterSettings settings;
	settings.bufferSize = 2;
	startTransmitter(settings);
	char expected[] = "ABC";
	transmitSerialAsync(expected, 3);
	stopTransmitter();
	TEST_ASSERT_EQUAL_UINT8(-1, flushTransmitter());
}

void GivenBufferedTransmission_WhenFlushTransmitter_CharacterStagedForTransmission() {
	SerialTransmitterSettings settings;
	settings.bufferSize = 2;
	startTransmitter(settings);
	char expected[] = "ABC";
	transmitSerialAsync(expected, 3);
	TEST_ASSERT_EQUAL_UINT8(0, UDR0);
	TEST_ASSERT_EQUAL_INT8(0, flushTransmitter());
	TEST_ASSERT_EQUAL_UINT8('B', UDR0);
}

void GivenBufferedTransmission_WhenCharacterFlushed_NextCharacterStagedForTransmission() {
	char expected[] = "ABC";
	uint8_t bufferSize = 3;
	SerialTransmitterSettings settings;
	settings.bufferSize = bufferSize;
	startTransmitter(settings);
	transmitSerialAsync(expected, bufferSize);
	uint8_t i;
	for (i = 0; i < bufferSize; i++) {
		USART0_UDRE_vect_function();
		TEST_ASSERT_EQUAL_UINT8(expected[i], UDR0);
	}
}

void GivenBufferedTransmission_WhenFlushTransmitter_DataEmptyInterruptEnabled() {
	SerialTransmitterSettings settings;
	settings.bufferSize = 2;
	startTransmitter(settings);
	transmitSerialAsync("A", 1);
	TEST_ASSERT_BIT_LOW(UDRIE0, UCSR0B);
	flushTransmitter();
	TEST_ASSERT_BIT_HIGH(UDRIE0, UCSR0B);
}

void GivenBufferedTransmission_WhenLastCharacterFlushed_DataEmptyInterruptDisabled() {
	SerialTransmitterSettings settings;
	settings.bufferSize = 2;
	startTransmitter(settings);
	transmitSerialAsync("A", 1);
	flushTransmitter();
	TEST_ASSERT_BIT_HIGH(UDRIE0, UCSR0B);
	USART0_UDRE_vect_function();
	TEST_ASSERT_BIT_LOW(UDRIE0, UCSR0B);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(WhenTransmitterStarted_ThenTransmitterEnabled);
	RUN_TEST(GivenTransmissionIsStarted_WhenTransmitSerialAsync_ThenCharactersBuffered);
	RUN_TEST(GivenTransmitterIsStarted_WhenStopTransmitter_ThenTransmitterDisabled);
	RUN_TEST(GivenTransmitterDisabled_WhenFlushTransmitter_FlushFails);
	RUN_TEST(GivenBufferedTransmission_WhenFlushTransmitter_CharacterStagedForTransmission);
	RUN_TEST(GivenBufferedTransmission_WhenCharacterFlushed_NextCharacterStagedForTransmission);
	RUN_TEST(GivenBufferedTransmission_WhenFlushTransmitter_DataEmptyInterruptEnabled);
	RUN_TEST(GivenBufferedTransmission_WhenLastCharacterFlushed_DataEmptyInterruptDisabled);
	return UNITY_END();
}
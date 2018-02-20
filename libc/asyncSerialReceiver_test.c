#include "../Unity/unity.h"
#include <serialcomm.h>
#include <common_types.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../test/mocks/io_mocks.c"

// Setup our mocked data
char value = 0;
void setUp() {
	UCSR0B = 0;
	UCSR0C = 0;
	UDR0 = 0;
}

void tearDown() {
	stopReceiver();
	value = 0;
}

void doNothing(uint8_t value) {

}

void receiveHandler(uint8_t valueReceived) {
	value = valueReceived;
}

void GivenReceiverIsStopped_WhenStartReceiver_ThenSerialReceiveInerruptEnabled() {
	TEST_ASSERT_EQUAL_INT8(0, startReceiver(doNothing));
	TEST_ASSERT_BIT_HIGH(RXCIE0, UCSR0B);
}


void GivenReceiverIsStopped_WhenStartReceiver_ThenSerialReceiveEnabled() {
	TEST_ASSERT_EQUAL_INT8(0, startReceiver(doNothing));
	TEST_ASSERT_BIT_HIGH(RXEN0, UCSR0B);
}


void GivenReceiverIsStarted_WhenStartReceiver_ThenSerialReceiveInterruptEnabled() {
	startReceiver(doNothing);
	TEST_ASSERT_EQUAL_INT8(-1, startReceiver(doNothing));
	TEST_ASSERT_BIT_HIGH(RXCIE0, UCSR0B);
}

void GivenReceiverIsRunning_WhenStopReceiver_ThenSerialReceiveInerruptDisabled() {
	startReceiver(doNothing);
	TEST_ASSERT_BIT_HIGH(RXCIE0, UCSR0B);
	stopReceiver();
	TEST_ASSERT_BIT_LOW(RXCIE0, UCSR0B);
}

void GivenReceiverIsRunning_WhenStopReceiver_ThenSerialReceiveDisabled() {
	startReceiver(doNothing);
	TEST_ASSERT_BIT_HIGH(RXEN0, UCSR0B);
	stopReceiver();
	TEST_ASSERT_BIT_LOW(RXEN0, UCSR0B);
}

void GivenReceiverIsStopped_WhenStopReceiver_ThenSerialReceiveInerruptDisabled() {
	startReceiver(doNothing);
	TEST_ASSERT_BIT_HIGH(RXCIE0, UCSR0B);
	stopReceiver();
	TEST_ASSERT_BIT_LOW(RXCIE0, UCSR0B);
	stopReceiver();
	TEST_ASSERT_BIT_LOW(RXCIE0, UCSR0B);
}

void GivenACallback_AndReceiverStarted_WhenFrameReceived_ThenCallbackInvoked() {
	char sampleData = 23;
	UDR0 = sampleData;
	startReceiver(receiveHandler);
	USART0_RX_vect_function();
	TEST_ASSERT_EQUAL_UINT8(sampleData, value);
}

int main (void) {
	UNITY_BEGIN();
	RUN_TEST(GivenReceiverIsStopped_WhenStartReceiver_ThenSerialReceiveInerruptEnabled);
	RUN_TEST(GivenReceiverIsStopped_WhenStartReceiver_ThenSerialReceiveEnabled);
	RUN_TEST(GivenReceiverIsStarted_WhenStartReceiver_ThenSerialReceiveInterruptEnabled);
	RUN_TEST(GivenReceiverIsRunning_WhenStopReceiver_ThenSerialReceiveInerruptDisabled);
	RUN_TEST(GivenReceiverIsRunning_WhenStopReceiver_ThenSerialReceiveDisabled);
	RUN_TEST(GivenReceiverIsStopped_WhenStopReceiver_ThenSerialReceiveInerruptDisabled);
	RUN_TEST(GivenACallback_AndReceiverStarted_WhenFrameReceived_ThenCallbackInvoked);
	return UNITY_END();
}
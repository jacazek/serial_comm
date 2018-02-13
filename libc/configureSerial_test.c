#include "../Unity/unity.h"
#include <serialcomm.h>
#include <common_types.h>
#include <avr/io.h> // this is mocked

// Setup our mocked data
volatile byte UBRR0H = 0;
volatile byte UBRR0L = 0;
volatile byte UCSR0B = 0;
volatile byte UCSR0C = 0;
volatile byte UDR0 = 0;

void setUp() {
	UBRR0H = 0;
	UBRR0L = 0;
	UCSR0B = 0;
	UCSR0C = 0;
}

void GivenSettingsWithHighBaudRate_WhenConfigureSerial_ThenBaudRateBytesSet() {
	byte expectedHighByte = 0;
	byte expectedLowByte = 104;
	struct SerialSettings settings;
	settings.baudrate = 16000000 / 16 / 9600;

	configureSerial(settings);

	TEST_ASSERT_EQUAL_UINT8_MESSAGE(expectedHighByte, UBRR0H, "Baud rate high byte set");
	TEST_ASSERT_EQUAL_UINT8_MESSAGE(expectedLowByte, UBRR0L, "Baud rate low byte not set");
}

void GivenSettingsWithLowBaudRate_WhenConfigureSerial_ThenBaudRateBytesSet() {
	byte expectedHighByte = 13;
	byte expectedLowByte = 5;
	struct SerialSettings settings;
	settings.baudrate = 16000000 / 16 / 300;

	configureSerial(settings);

	TEST_ASSERT_EQUAL_UINT8_MESSAGE(expectedHighByte, UBRR0H, "Baud rate high byte not set");
	TEST_ASSERT_EQUAL_UINT8_MESSAGE(expectedLowByte, UBRR0L, "Baud rate low byte not set");
}

void GivenSettingsDisableTransmit_WhenConfigureSerial_TransmitDisabled() {
	struct SerialSettings settings;
	settings.TX = 0;

	configureSerial(settings);

	TEST_ASSERT_BIT_LOW(TXEN0, UCSR0B);
}

void GivenSettingsEnableTransmit_WhenConfigureSerial_TransmitEnabled() {
	struct SerialSettings settings;
	settings.TX = 1;

	configureSerial(settings);

	TEST_ASSERT_BIT_HIGH(TXEN0, UCSR0B);
}

void GivenSettingsDisableReceive_WhenConfigureSerial_ReceiveDisabled() {
	struct SerialSettings settings;
	settings.RX = 0;

	configureSerial(settings);

	TEST_ASSERT_BIT_LOW(RXEN0, UCSR0B);
}

void GivenSettingsEnableReceive_WhenConfigureSerial_ReceiveEnabled() {
	struct SerialSettings settings;
	settings.RX = 1;

	configureSerial(settings);

	TEST_ASSERT_BIT_HIGH(RXEN0, UCSR0B);
}

void GivenSettings_WhenConfigureSerial_DataFrameSetTo8Bits() {
	/* Frame format: 8data, No parity, 1stop bit */
	struct SerialSettings settings;

	configureSerial(settings);

	TEST_ASSERT_BIT_LOW(UCSZ02, UCSR0B);
	TEST_ASSERT_BIT_HIGH(UCSZ01, UCSR0C);
	TEST_ASSERT_BIT_HIGH(UCSZ00, UCSR0C);
}

void GivenSettings_WhenConfigured_NoParityBit() {
	struct SerialSettings settings;

	configureSerial(settings);

	TEST_ASSERT_BIT_LOW(UPMN1, UCSR0C);
	TEST_ASSERT_BIT_LOW(UPMN0, UCSR0C);
}

void GivenSettings_WhenConfigured_AsynchronousModeEnabled() {
	struct SerialSettings settings;

	configureSerial(settings);

	TEST_ASSERT_BIT_LOW(UMSEL01, UCSR0C);
	TEST_ASSERT_BIT_LOW(UMSEL00, UCSR0C);
}


void GivenSettings_WhenConfigured_OneStopBitEnabled() {
	struct SerialSettings settings;

	configureSerial(settings);

	TEST_ASSERT_BIT_LOW(USBS0, UCSR0C);
}


int main(void) {
	UNITY_BEGIN();
	RUN_TEST(GivenSettingsWithHighBaudRate_WhenConfigureSerial_ThenBaudRateBytesSet);
	RUN_TEST(GivenSettingsWithLowBaudRate_WhenConfigureSerial_ThenBaudRateBytesSet);
	RUN_TEST(GivenSettingsDisableTransmit_WhenConfigureSerial_TransmitDisabled);
	RUN_TEST(GivenSettingsEnableTransmit_WhenConfigureSerial_TransmitEnabled);
	RUN_TEST(GivenSettingsEnableReceive_WhenConfigureSerial_ReceiveEnabled);
	RUN_TEST(GivenSettingsDisableReceive_WhenConfigureSerial_ReceiveDisabled);
	RUN_TEST(GivenSettings_WhenConfigureSerial_DataFrameSetTo8Bits);
	RUN_TEST(GivenSettings_WhenConfigured_NoParityBit);
	RUN_TEST(GivenSettings_WhenConfigured_AsynchronousModeEnabled);
	RUN_TEST(GivenSettings_WhenConfigured_OneStopBitEnabled);
	return UNITY_END();
}
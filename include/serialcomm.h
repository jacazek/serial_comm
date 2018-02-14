#ifndef _SERIALCOMM_H_
#define _SERIALCOMM_H_
#include <common_types.h>
#include <ring_buffer.h>

typedef struct SerialSettings {
	unsigned short RX:1;
	unsigned short reserved:6;
	short baudrate;
} SerialSettings;

typedef struct SerialTransmitterSettings {
	uint8_t bufferSize;
} SerialTransmitterSettings;

extern void configureSerial(SerialSettings settings);

extern int8_t transmitSerialAsync(uint8_t *value, uint8_t size);
extern int8_t startTransmitter(SerialTransmitterSettings settings);
extern int8_t flushTransmitter();
extern int8_t stopTransmitter();

extern int8_t startReceiver(dataCallback clientCallback);
extern int8_t stopReceiver();
#endif
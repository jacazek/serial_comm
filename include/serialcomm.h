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


/**
	Configure settings for serial communications
	@param {SerialSettings} settings The settings for serial communication
*/
extern void configureSerial(SerialSettings settings);

/**
	Transmit a series of bytes
	@param {uint8_t*} value A pointer to values to be transmitted
	@param {uint8_t} count The number of values to be transmitted
	@return The number of bytes buffered successfully, -1 if not
*/
extern int8_t transmitSerialAsync(uint8_t *value, uint8_t count);
/**
	Start the transmitter
	@param {SerialTransmitterSettings} settings Settings needed for serial transmission
	@return 0 if transmitter started, -1 if not
*/
extern int8_t startTransmitter(SerialTransmitterSettings settings);
/**
	Flush the transmitter of all data waiting to be transmitted
	@return 0 if successful, -1 if not
*/
extern int8_t flushTransmitter();
/**
	Stop the transmitter
*/
extern void stopTransmitter();

/**
	Start the receiver
	@param {dataCallback} clientCallback The callback to be invoked when a byte is received
	@return 0 if successful, -1 if not
*/
extern int8_t startReceiver(dataCallback clientCallback);
/**
	Stop the receiver
*/
extern void stopReceiver();
#endif
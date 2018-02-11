#ifndef _SERIALCOMM_H_
#define _SERIALCOMM_H_
#include <common_types.h>

struct SerialSettings {
	unsigned short RX:1;
	unsigned short TX:1;
	short baudrate;
};

extern void configureSerial(struct SerialSettings settings);
extern void transmitSerialAsync(byte bytes[], callback transmitCompleteCallback);
#endif
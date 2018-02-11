#ifndef _SERIALCOMM_H_
#define _SERIALCOMM_H_
#include <common_types.h>

struct SerialSettings {
	unsigned short RX:1;
	unsigned short TX:1;
	short baudrate;
};

extern byte serialCheckRxComplete(void);
extern byte serialCheckTxReady(void);
extern byte serialRead(void);
extern void serialWrite(byte DataOut);
extern void serialWriteLine(byte DataOut[]);
extern void serialWriteInt(int Dataout);
extern void delayLong();
extern void establishContact();
extern void configureSerial(struct SerialSettings settings);
extern void transmitSerialAsync(byte bytes[], callback transmitCompleteCallback);
#endif
#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_
#include <stdint.h>



#define OVERWRITE_EN 0

typedef struct RingBuffer {
	uint8_t settings;
	uint8_t capacity;
	uint8_t filled;
	uint8_t head;
	char *buffer;
} RingBuffer;

/**
	Read a value from the provided ring buffer
	@param {RingBuffer} ringBuffer The buffer from which to read the value
	@param (char*) value Pointer to location to place output value
	@return 0 if successful, -1 if failed
*/
extern int8_t readRingBuffer(RingBuffer *ringBuffer, char *value);
/**
	Write a value to the provided ring buffer
	@param {RingBuffer} ringBuffer The buffer to which the value will be written
	@param (char) value The value to write
	@return 0 if successful, -1 if failed
*/
extern int8_t writeRingBuffer(RingBuffer *ringBuffer, char value);
/**
	Create a ring buffer
	@param {uint8_t} capacity The capacity of the provided buffer
	@param {char*} buffer The buffer to back the ring buffer
	@return A pointer to a new ring buffer if successful
*/
extern struct RingBuffer createRingBuffer(uint8_t capacity, char *buffer);
#endif
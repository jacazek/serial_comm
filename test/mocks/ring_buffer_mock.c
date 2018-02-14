#include <ring_buffer.h>

// int8_t readRingBuffer(RingBuffer *ringBuffer, char *value) {
// 	return 0;
// };
// int8_t writeRingBuffer(RingBuffer *ringBuffer, char value) {
// 	return 0;
// };
// struct RingBuffer createRingBuffer(uint8_t capacity, char *buffer) {
// 	RingBuffer temp;
// 	return temp;
// };

#define MOCK_CREATE_RING_BUFFER RingBuffer createRingBuffer(uint8_t capacity, char	*buffer)

#define MOCK_WRITE_RING_BUFFER int8_t writeRingBuffer(RingBuffer *ringBuffer, char value)

#define MOCK_READ_RING_BUFFER int8_t readRingBuffer(RingBuffer *ringBuffer, char *value)
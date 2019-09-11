#ifndef RINGBUF_H_INCLUDED
#define RINGBUF_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>

typedef struct ringbuf {
	uint8_t *buffer;
	uint32_t mask;

	uint32_t rptr;
	uint32_t wptr;
	void (*signal)(struct ringbuf* pr);
}ringbuf_t;

typedef struct ringbuf ringbuf_t;

/* Initialization and finalization */
void ringbuf_init(struct ringbuf *rb, uint8_t *buffer, size_t bufsize);

/* Writing to ring buffer */
size_t ringbuf_write_available(struct ringbuf *rb);
bool ringbuf_write_byte(struct ringbuf *rb, uint8_t data);
bool ringbuf_write_buffer(struct ringbuf *rb, size_t bufsiz,  uint8_t *data);
size_t ringbuf_write_buffer_partial(struct ringbuf *rb, size_t bufsiz, const uint8_t *data);

/* Reading from ring buffer */
size_t ringbuf_read_available(struct ringbuf *rb);
bool ringbuf_read_byte(struct ringbuf *rb, uint8_t *data);
bool ringbuf_read_buffer(struct ringbuf *rb, size_t bufsiz, uint8_t *data);

#endif // RINGBUF_H_INCLUDED

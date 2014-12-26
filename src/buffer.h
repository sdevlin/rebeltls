#ifndef REBELTLS_BUFFER_H
#define REBELTLS_BUFFER_H

#include "inttypes.h"

typedef struct {
  uint8 *buf;
  uint len;
  uint cap;
  uint rdpos;
  uint wrpos;
} buffer;

extern void buffer_init(buffer *b, uint8 *buf, uint len);
extern void buffer_alloc(buffer *b, uint len);
extern buffer *buffer_new(uint len);

extern uint buffer_readavail(buffer *b);
extern uint buffer_writeavail(buffer *b);

extern void buffer_peek(buffer *b, uint8 *buf, uint len);
extern void buffer_read(buffer *b, uint8 *buf, uint len);
extern void buffer_write(buffer *b, const uint8 *buf, uint len);

#endif

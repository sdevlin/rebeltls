#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "log.h"
#include "inttypes.h"

void buffer_init(buffer *b, uint8 *buf, uint len)
{
  b->buf = buf;
  b->len = 0;
  b->cap = len;
  b->rdpos = 0;
  b->wrpos = 0;
}

void buffer_alloc(buffer *b, uint len)
{
  uint8 *buf;

  buf = malloc(len);
  buffer_init(b, buf, len);
}

buffer *buffer_new(uint len)
{
  buffer *b;

  b = malloc(sizeof *b);
  buffer_alloc(b, len);
  return b;
}

uint buffer_readavail(buffer *b)
{
  return b->len;
}

uint buffer_writeavail(buffer *b)
{
  return b->cap - b->len;
}

void buffer_peek(buffer *b, uint8 *buf, uint len)
{
  uint endlen;

  log_assert(len <= buffer_readavail(b), "not enough read data");

  endlen = b->cap - b->rdpos;
  if (endlen >= len) {
    memcpy(buf, b->buf + b->rdpos, len);
  } else {
    memcpy(buf, b->buf + b->rdpos, endlen);
    memcpy(buf + endlen, b->buf, len - endlen);
  }
}

void buffer_read(buffer *b, uint8 *buf, uint len)
{
  buffer_peek(b, buf, len);
  b->len -= len;
  b->rdpos += len;
  b->rdpos %= b->cap;
}

void buffer_write(buffer *b, const uint8 *buf, uint len)
{
  uint endlen;

  log_assert(len <= buffer_writeavail(b), "not enough write space");

  endlen = b->cap - b->wrpos;
  if (endlen >= len) {
    memcpy(b->buf + b->wrpos, buf, len);
  } else {
    memcpy(b->buf + b->wrpos, buf, endlen);
    memcpy(b->buf, buf + endlen, len - endlen);
  }

  b->len += len;
  b->wrpos += len;
  b->wrpos %= b->cap;
}

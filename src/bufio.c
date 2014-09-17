#include <string.h>

#include "io.h"

static uint min(uint a, uint b)
{
  return a < b ? a : b;
}

static int bufio_tryread(io_src *src, byte *buf, uint buflen)
{
  byte *srcbuf;
  uint srclen;
  uint n;

  srcbuf = src->buf.buf + src->buf.pos;
  srclen = src->buf.len - src->buf.pos;
  n = min(buflen, srclen);
  memcpy(buf, srcbuf, n);
  src->buf.pos += n;
  return n;
}

static int bufio_trywrite(io_src *src, const byte *buf, uint buflen)
{
  byte *srcbuf;
  uint srclen;
  uint n;

  srcbuf = src->buf.buf + src->buf.pos;
  srclen = src->buf.len - src->buf.pos;
  n = min(buflen, srclen);
  memcpy(srcbuf, buf, n);
  src->buf.pos += n;
  return n;
}

static int bufio_read(io_src *src, byte *buf, uint buflen)
{
  return bufio_tryread(src, buf, buflen);
}

static int bufio_write(io_src *src, const byte *buf, uint buflen)
{
  return bufio_trywrite(src, buf, buflen);
}

const struct io_defn bufio_defn = {
  .tryread = &bufio_tryread,
  .trywrite = &bufio_trywrite,
  .read = &bufio_read,
  .write = &bufio_write
};

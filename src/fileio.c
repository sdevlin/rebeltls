#include <unistd.h>

#include "io.h"

static int fileio_tryread(io_src *src, byte *buf, uint buflen)
{
  int fd;

  fd = *(int *)src;
  return read(fd, buf, buflen);
}

static int fileio_trywrite(io_src *src, const byte *buf, uint buflen)
{
  int fd;

  fd = *(int *)src;
  return write(fd, buf, buflen);
}

static int fileio_read(io_src *src, byte *buf, uint buflen)
{
  return fileio_tryread(src, buf, buflen);
}

static int fileio_write(io_src *src, const byte *buf, uint buflen)
{
  return fileio_trywrite(src, buf, buflen);
}

const struct io_defn fileio_defn = {
  .tryread = &fileio_tryread,
  .trywrite = &fileio_trywrite,
  .read = &fileio_read,
  .write = &fileio_write
};

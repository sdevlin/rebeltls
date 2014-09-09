#include <unistd.h>

#include "types.h"

void io_read(int fd, byte *buf, uint buflen)
{
  uint n;

  while (buflen > 0) {
    n = read(fd, buf, buflen);
    buf += n;
    buflen -= n;
  }
}

void io_write(int fd, const byte *buf, uint buflen)
{
  uint n;

  while (buflen > 0) {
    n = write(fd, buf, buflen);
    buf += n;
    buflen -= n;
  }
}

/* probably rethink, won't work with socket */
void io_seek(int fd, uint len)
{
  lseek(fd, len, SEEK_CUR);
}

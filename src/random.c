#include <fcntl.h>
#include <unistd.h>

#include "random.h"

static int fd = -1;

static const int max_readlen = 1 << 20;

void random_bytes(uint8 *buf, uint buflen)
{
  uint n;

  if (fd == -1) {
    fd = open("/dev/urandom", O_RDONLY);
    while (fd == -1) {
      sleep(1);
      fd = open("/dev/urandom", O_RDONLY);
    }
  }

  if (buflen > 0) {
    for (; ; ) {
      n = buflen < max_readlen ? buflen : max_readlen;
      n = read(fd, buf, n);
      buf += n;
      buflen -= n;
      if (buflen == 0) {
        return;
      }
    }
  }
}

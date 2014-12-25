#include <string.h>

#include "bytes.h"
#include "inttypes.h"

void bytes_xor(byte *x, const byte *y, const byte *z, uint len)
{
  uint i;

  for (i = 0; i < len; i += 1) {
    x[i] = y[i] ^ z[i];
  }
}

void bytes_zero(byte *buf, uint buflen)
{
  memset(buf, 0, buflen);
}

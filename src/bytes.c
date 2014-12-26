#include <string.h>

#include "bytes.h"
#include "inttypes.h"

void bytes_xor(uint8 *x, const uint8 *y, const uint8 *z, uint len)
{
  uint i;

  for (i = 0; i < len; i += 1) {
    x[i] = y[i] ^ z[i];
  }
}

void bytes_zero(uint8 *buf, uint buflen)
{
  memset(buf, 0, buflen);
}

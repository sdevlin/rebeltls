#include <string.h>

#include "bytes.h"
#include "inttypes.h"

void bytes_xor(u8 *x, const u8 *y, const u8 *z, uint len)
{
  uint i;

  for (i = 0; i < len; i += 1) {
    x[i] = y[i] ^ z[i];
  }
}

void bytes_zero(u8 *buf, uint buflen)
{
  memset(buf, 0, buflen);
}

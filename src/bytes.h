#ifndef REBELTLS_BYTES_H
#define REBELTLS_BYTES_H

#include "types.h"

extern void bytes_xor(byte *x, const byte *y, const byte *z, uint len);

extern void bytes_zero(byte *buf, uint buflen);

#endif

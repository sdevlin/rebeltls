#ifndef REBELTLS_BYTES_H
#define REBELTLS_BYTES_H

#include "inttypes.h"

extern void bytes_xor(u8 *x, const u8 *y, const u8 *z, uint len);

extern void bytes_zero(u8 *buf, uint buflen);

#endif

#ifndef REBELTLS_BYTES_H
#define REBELTLS_BYTES_H

#include "inttypes.h"

extern void bytes_xor(uint8 *x, const uint8 *y, const uint8 *z, uint len);

extern void bytes_zero(uint8 *buf, uint buflen);

#endif

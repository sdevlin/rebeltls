#ifndef TLS_BYTES_H
#define TLS_BYTES_H

#include <stdarg.h>

#include "types.h"

extern void bytes_xor(byte *x, const byte *y, const byte *z, uint len);

extern void bytes_zero(byte *buf, uint buflen);

extern byte *bytes_vpack(byte *buf, const char *fmt, va_list argp);
extern byte *bytes_pack(byte *buf, const char *fmt, ...);

extern const byte *bytes_vunpack(const byte *buf, const char *fmt,
                                 va_list argp);
extern const byte *bytes_unpack(const byte *buf, const char *fmt, ...);

#endif

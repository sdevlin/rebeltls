#ifndef TLS_BINDATA_H
#define TLS_BINDATA_H

#include <stdarg.h>

#include "types.h"

extern byte *bindata_vpack(byte *buf, const char *fmt, va_list argp);
extern byte *bindata_pack(byte *buf, const char *fmt, ...);

extern const byte *bindata_vunpack(const byte *buf, const char *fmt,
                                   va_list argp);
extern const byte *bindata_unpack(const byte *buf, const char *fmt, ...);

#endif

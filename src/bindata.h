#ifndef REBELTLS_BINDATA_H
#define REBELTLS_BINDATA_H

#include <stdarg.h>

#include "inttypes.h"

extern uint bindata_vpack(byte *buf, const char *fmt, va_list argp);
extern uint bindata_pack(byte *buf, const char *fmt, ...);

extern uint bindata_vunpack(const byte *buf, const char *fmt,
                                   va_list argp);
extern uint bindata_unpack(const byte *buf, const char *fmt, ...);

#endif

#ifndef REBELTLS_BINDATA_H
#define REBELTLS_BINDATA_H

#include <stdarg.h>

#include "inttypes.h"

extern uint bindata_vpack(u8 *buf, const char *fmt, va_list argp);
extern uint bindata_pack(u8 *buf, const char *fmt, ...);

extern uint bindata_vunpack(const u8 *buf, const char *fmt,
                            va_list argp);
extern uint bindata_unpack(const u8 *buf, const char *fmt, ...);

#endif

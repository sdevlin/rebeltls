#ifndef REBELTLS_BINDATA_H
#define REBELTLS_BINDATA_H

#include <stdarg.h>

#include "inttypes.h"

extern uint bindata_vpack(uint8 *buf, const char *fmt, va_list argp);
extern uint bindata_pack(uint8 *buf, const char *fmt, ...);

extern uint bindata_vunpack(const uint8 *buf, const char *fmt,
                            va_list argp);
extern uint bindata_unpack(const uint8 *buf, const char *fmt, ...);

#endif

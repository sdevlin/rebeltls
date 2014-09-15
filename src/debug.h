#ifndef TLS_DEBUG_H
#define TLS_DEBUG_H

#include <stdarg.h>

extern void debug_info(const char *fmt, ...);
extern void debug_vinfo(const char *fmt, va_list argp);

extern void debug_warn(const char *fmt, ...);
extern void debug_vwarn(const char *fmt, va_list argp);

extern void debug_error(const char *fmt, ...);
extern void debug_verror(const char *fmt, va_list argp);

extern void debug_assert(int expr, const char *fmt, ...);
extern void debug_vassert(int expr, const char *fmt, va_list argp);

#endif

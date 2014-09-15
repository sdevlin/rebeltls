#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "debug.h"

#define COLOR_INFO "0"
#define COLOR_WARN "3"
#define COLOR_ERROR "1"

static void print(const char *prefix, const char *color,
                  const char *fmt, va_list argp)
{
  int tty;
  tty = isatty(STDOUT_FILENO);
  if (tty) {
    fprintf(stderr, "\x1b[1;%sm", color);
  }
  fprintf(stderr, "%s: ", prefix);
  vfprintf(stderr, fmt, argp);
  if (tty) {
    fprintf(stderr, "\x1b[0m");
  }
  fprintf(stderr, "\n");
}

void debug_info(const char *fmt, ...)
{
  va_list argp;

  va_start(argp, fmt);
  debug_vinfo(fmt, argp);
  va_end(argp);
}

void debug_vinfo(const char *fmt, va_list argp)
{
  print("INFO", COLOR_INFO, fmt, argp);
}

void debug_warn(const char *fmt, ...)
{
  va_list argp;

  va_start(argp, fmt);
  debug_vwarn(fmt, argp);
  va_end(argp);
}

void debug_vwarn(const char *fmt, va_list argp)
{
  print("WARN", COLOR_WARN, fmt, argp);
}

void debug_error(const char *fmt, ...)
{
  va_list argp;

  va_start(argp, fmt);
  debug_verror(fmt, argp);
}

void debug_verror(const char *fmt, va_list argp)
{
  debug_vassert(0, fmt, argp);
}

void debug_assert(int expr, const char *fmt, ...)
{
  va_list argp;

  va_start(argp, fmt);
  debug_vassert(expr, fmt, argp);
  va_end(argp);
}

void debug_vassert(int expr, const char *fmt, va_list argp)
{
  if (!expr) {
    print("ERROR", COLOR_ERROR, fmt, argp);
    exit(1);
  }
}

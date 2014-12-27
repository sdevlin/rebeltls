#ifndef REBELTLS_BDLANG_H
#define REBELTLS_BDLANG_H

/*
  bdlang documentation:

    endianness:
      < - big-endian
      > - little-endian
      ! - network-endian (same as big-)
      = - native-endian (almost certainly little-)

    integer types:
      b - i8
      B - u8
      h - i16
      H - u16
      T - u24
      l - i32
      L - u32
      q - i64
      Q - u64

    command types:
      register - individually specified locations
      array    - fixed-size buffer
      vector   - variable-size buffer (like pascal string)
*/

#include "inttypes.h"

enum {
  ENDIAN_BIG,
  ENDIAN_LITTLE,
  ENDIAN_NATIVE
};

enum {
  TYPE_REGISTER,
  TYPE_ARRAY,
  TYPE_VECTOR
};

struct bdcmd {
  int cmdtype;
  int inttype;
  union {
    u64 val;
    struct {
      u32 max;
      u32 min;
    } rng;
  } coef;
};

struct bdprog {
  const char *src;
  int endian;
  uint ncmds;
  struct bdcmd *cmds;
  struct bdprog *next;
};

extern const struct bdprog *bdlang_compile(const char *src);

#endif

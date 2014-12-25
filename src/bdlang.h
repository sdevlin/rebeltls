#ifndef REBELTLS_BDLANG_H
#define REBELTLS_BDLANG_H

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
    uint64 val;
    struct {
      uint32 max;
      uint32 min;
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

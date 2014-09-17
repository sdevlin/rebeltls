#ifndef TLS_VECTOR_H
#define TLS_VECTOR_H

#include "types.h"

struct vector {
  byte *data;
  uint32 cap;
  uint32 len;
};

#endif

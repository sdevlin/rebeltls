#ifndef TLS_VECTOR_H
#define TLS_VECTOR_H

#include "types.h"

struct vector {
  const byte *buf;
  uint32 len;
};

#endif

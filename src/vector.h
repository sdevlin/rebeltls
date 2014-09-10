#ifndef TLS_VECTOR_H
#define TLS_VECTOR_H

#include "types.h"

typedef struct {
  byte *data;
  uint32 len;
  uint itemsize;
} vector;

#endif

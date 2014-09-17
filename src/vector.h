#ifndef TLS_VECTOR_H
#define TLS_VECTOR_H

#include "types.h"

struct vector {
  byte *data;
  uint32 cap;
  uint32 len;
};

extern void vector_init(struct vector *vec, void *data, uint32 len);
extern struct vector vector_new(void *data, uint32 len);

#endif

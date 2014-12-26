#ifndef REBELTLS_VECTOR_H
#define REBELTLS_VECTOR_H

#include "inttypes.h"

struct vector {
  uint8 *data;
  uint32 cap;
  uint32 len;
};

extern void vector_init(struct vector *vec, void *data, uint32 len);
extern struct vector vector_new(void *data, uint32 len);

#endif

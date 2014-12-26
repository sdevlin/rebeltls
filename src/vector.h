#ifndef REBELTLS_VECTOR_H
#define REBELTLS_VECTOR_H

#include "inttypes.h"

struct vector {
  u8 *data;
  u32 cap;
  u32 len;
};

extern void vector_init(struct vector *vec, void *data, u32 len);
extern struct vector vector_new(void *data, u32 len);

#endif

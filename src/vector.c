#include "vector.h"

extern void vector_init(struct vector *vec, void *data, u32 len)
{
  vec->data = data;
  vec->len = len;
}

extern struct vector vector_new(void *data, u32 len)
{
  struct vector vec;

  vector_init(&vec, data, len);
  return vec;
}

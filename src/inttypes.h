#ifndef REBELTLS_INTTYPES_H
#define REBELTLS_INTTYPES_H

#include <stddef.h>
#include <stdint.h>

typedef unsigned uint;
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef uint32_t uint24;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef struct {
  uint64 hi;
  uint64 lo;
} uint128;

extern uint24 uint24_clamp(uint24 x);

extern void uint128_zero(uint128 *x);
extern void uint128_xor(uint128 *x, const uint128 *y, const uint128 *z);
extern uint uint128_bit(uint128 *x, uint k);

#endif

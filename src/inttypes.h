#ifndef REBELTLS_INTTYPES_H
#define REBELTLS_INTTYPES_H

#include <stddef.h>
#include <stdint.h>

typedef unsigned uint;
typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef uint32_t u24;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef struct {
  u64 hi;
  u64 lo;
} u128;

extern u24 u24_clamp(u24 x);

extern void u128_zero(u128 *x);
extern void u128_xor(u128 *x, const u128 *y, const u128 *z);
extern uint u128_bit(u128 *x, uint k);

#endif

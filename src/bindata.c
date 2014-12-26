#include <stdarg.h>
#include <stdlib.h>

#include "bdlang.h"
#include "log.h"
#include "inttypes.h"
#include "vector.h"

static uint pack_i8(u8 *buf, i8 n)
{
  buf[0] = n;
  return sizeof n;
}

static uint pack_u8(u8 *buf, u8 n)
{
  buf[0] = n;
  return sizeof n;
}

static uint nat_pack_i16(u8 *buf, i16 n)
{
  *(i16 *)buf = n;
  return sizeof n;
}

static uint le_pack_i16(u8 *buf, i16 n)
{
  buf[0] = 0xff & n;
  buf[1] = 0xff & (n >> 8);
  return sizeof n;
}

static uint be_pack_i16(u8 *buf, i16 n)
{
  buf[1] = 0xff & n;
  buf[0] = 0xff & (n >> 8);
  return sizeof n;
}

static uint nat_pack_u16(u8 *buf, u16 n)
{
  *(u16 *)buf = n;
  return sizeof n;
}

static uint le_pack_u16(u8 *buf, u16 n)
{
  buf[0] = 0xff & n;
  buf[1] = 0xff & (n >> 8);
  return sizeof n;
}

static uint be_pack_u16(u8 *buf, u16 n)
{
  buf[1] = 0xff & n;
  buf[0] = 0xff & (n >> 8);
  return sizeof n;
}

__attribute__ ((noreturn))
static uint nat_pack_u24(__attribute__ ((unused)) u8 *buf,
                         __attribute__ ((unused)) u32 n)
{
  log_abort("bindata: native pack u24");
}

static uint le_pack_u24(u8 *buf, u32 n)
{
  buf[0] = 0xff & n;
  buf[1] = 0xff & (n >> 8);
  buf[2] = 0xff & (n >> 16);
  return 3;
}

static uint be_pack_u24(u8 *buf, u32 n)
{
  buf[2] = 0xff & n;
  buf[1] = 0xff & (n >> 8);
  buf[0] = 0xff & (n >> 16);
  return 3;
}

static uint nat_pack_i32(u8 *buf, i32 n)
{
  *(i32 *)buf = n;
  return sizeof n;
}

static uint le_pack_i32(u8 *buf, i32 n)
{
  buf[0] = 0xff & n;
  buf[1] = 0xff & (n >> 8);
  buf[2] = 0xff & (n >> 16);
  buf[3] = 0xff & (n >> 24);
  return sizeof n;
}

static uint be_pack_i32(u8 *buf, i32 n)
{
  buf[3] = 0xff & n;
  buf[2] = 0xff & (n >> 8);
  buf[1] = 0xff & (n >> 16);
  buf[0] = 0xff & (n >> 24);
  return sizeof n;
}

static uint nat_pack_u32(u8 *buf, u32 n)
{
  *(u32 *)buf = n;
  return sizeof n;
}

static uint le_pack_u32(u8 *buf, u32 n)
{
  buf[0] = 0xff & n;
  buf[1] = 0xff & (n >> 8);
  buf[2] = 0xff & (n >> 16);
  buf[3] = 0xff & (n >> 24);
  return sizeof n;
}

static uint be_pack_u32(u8 *buf, u32 n)
{
  buf[3] = 0xff & n;
  buf[2] = 0xff & (n >> 8);
  buf[1] = 0xff & (n >> 16);
  buf[0] = 0xff & (n >> 24);
  return sizeof n;
}

static uint nat_pack_i64(u8 *buf, i64 n)
{
  *(i64 *)buf = n;
  return sizeof n;
}

static uint le_pack_i64(u8 *buf, i64 n)
{
  buf[0] = 0xff & n;
  buf[1] = 0xff & (n >> 8);
  buf[2] = 0xff & (n >> 16);
  buf[3] = 0xff & (n >> 24);
  buf[4] = 0xff & (n >> 32);
  buf[5] = 0xff & (n >> 40);
  buf[6] = 0xff & (n >> 48);
  buf[7] = 0xff & (n >> 56);
  return sizeof n;
}

static uint be_pack_i64(u8 *buf, i64 n)
{
  buf[7] = 0xff & n;
  buf[6] = 0xff & (n >> 8);
  buf[5] = 0xff & (n >> 16);
  buf[4] = 0xff & (n >> 24);
  buf[3] = 0xff & (n >> 32);
  buf[2] = 0xff & (n >> 40);
  buf[1] = 0xff & (n >> 48);
  buf[0] = 0xff & (n >> 56);
  return sizeof n;
}

static uint nat_pack_u64(u8 *buf, u64 n)
{
  *(u64 *)buf = n;
  return sizeof n;
}

static uint le_pack_u64(u8 *buf, u64 n)
{
  buf[0] = 0xff & n;
  buf[1] = 0xff & (n >> 8);
  buf[2] = 0xff & (n >> 16);
  buf[3] = 0xff & (n >> 24);
  buf[4] = 0xff & (n >> 32);
  buf[5] = 0xff & (n >> 40);
  buf[6] = 0xff & (n >> 48);
  buf[7] = 0xff & (n >> 56);
  return sizeof n;
}

static uint be_pack_u64(u8 *buf, u64 n)
{
  buf[7] = 0xff & n;
  buf[6] = 0xff & (n >> 8);
  buf[5] = 0xff & (n >> 16);
  buf[4] = 0xff & (n >> 24);
  buf[3] = 0xff & (n >> 32);
  buf[2] = 0xff & (n >> 40);
  buf[1] = 0xff & (n >> 48);
  buf[0] = 0xff & (n >> 56);
  return sizeof n;
}

struct pack_fns {
  uint (*pack_i8)(u8 *, i8);
  uint (*pack_u8)(u8 *, u8);
  uint (*pack_i16)(u8 *, i16);
  uint (*pack_u16)(u8 *, u16);
  uint (*pack_u24)(u8 *, u32);
  uint (*pack_i32)(u8 *, i32);
  uint (*pack_u32)(u8 *, u32);
  uint (*pack_i64)(u8 *, i64);
  uint (*pack_u64)(u8 *, u64);
};

static struct pack_fns nat_pack_fns = {
  .pack_i8 = &pack_i8,
  .pack_u8 = &pack_u8,
  .pack_i16 = &nat_pack_i16,
  .pack_u16 = &nat_pack_u16,
  .pack_u24 = &nat_pack_u24,
  .pack_i32 = &nat_pack_i32,
  .pack_u32 = &nat_pack_u32,
  .pack_i64 = &nat_pack_i64,
  .pack_u64 = &nat_pack_u64
};

static struct pack_fns le_pack_fns = {
  .pack_i8 = &pack_i8,
  .pack_u8 = &pack_u8,
  .pack_i16 = &le_pack_i16,
  .pack_u16 = &le_pack_u16,
  .pack_u24 = &le_pack_u24,
  .pack_i32 = &le_pack_i32,
  .pack_u32 = &le_pack_u32,
  .pack_i64 = &le_pack_i64,
  .pack_u64 = &le_pack_u64
};

static struct pack_fns be_pack_fns = {
  .pack_i8 = &pack_i8,
  .pack_u8 = &pack_u8,
  .pack_i16 = &be_pack_i16,
  .pack_u16 = &be_pack_u16,
  .pack_u24 = &be_pack_u24,
  .pack_i32 = &be_pack_i32,
  .pack_u32 = &be_pack_u32,
  .pack_i64 = &be_pack_i64,
  .pack_u64 = &be_pack_u64
};

#define PACK_SWITCH                             \
  do {                                          \
    switch (inttype) {                          \
      PACK_CASE('b', i8, int);                  \
      PACK_CASE('B', u8, unsigned);             \
      PACK_CASE('h', i16, int);                 \
      PACK_CASE('H', u16, unsigned);            \
      PACK_CASE('T', u24, unsigned long);       \
      PACK_CASE('l', i32, long);                \
      PACK_CASE('L', u32, unsigned long);       \
      PACK_CASE('q', i64, long long);           \
      PACK_CASE('Q', u64, unsigned long long);  \
    default:                                    \
      exit(1);                                  \
    }                                           \
  } while (0)

#define PACK_CASE(c, ptype, atype)                                      \
  case c:                                                               \
  for (i = 0, n = 0; i < count; i += 1) {                               \
                                         n += fns->pack_##ptype(buf + n, va_arg(argp, atype)); \
                                         }                              \
  break;

    static uint pack_register(u8 *buf, int inttype, u64 count,
                              struct pack_fns *fns, va_list argp)
    {
      uint i, n;
      PACK_SWITCH;
      return n;
    }

#undef PACK_CASE

#define PACK_CASE(c, ptype, atype)              \
  case c:                                       \
  {                                             \
    ptype *p = va_arg(argp, ptype *);           \
    for (i = 0, n = 0; i < len; i += 1) {       \
      n += fns->pack_##ptype(buf + n, p[i]);    \
    }                                           \
    break;                                      \
  }

static uint pack_array(u8 *buf, int inttype, u64 len,
                       struct pack_fns *fns, va_list argp)
{
  uint i, n;
  PACK_SWITCH;
  return n;
}

#undef PACK_CASE

#define PACK_CASE(c, ptype, atype)              \
  case c:                                       \
  {                                             \
    ptype *p = (ptype *)vec->data;              \
    len = vec->len / (sizeof *p);               \
    for (i = 0; i < len; i += 1) {              \
      n += fns->pack_##ptype(buf + n, p[i]);    \
    }                                           \
    break;                                      \
  }

static uint pack_vector(u8 *buf, int inttype, u32 min, u32 max,
                        struct pack_fns *fns, va_list argp)
{
  uint i, n;
  uint len;
  struct vector *vec;
  vec = va_arg(argp, struct vector *);
  log_assert(vec->len >= min && vec->len <= max,
             "vector length %u, expected <%u..%u>",
             vec->len, min, max);
  /* TODO handle variable size vectors */
  n = fns->pack_u8(buf, vec->len);
  PACK_SWITCH;
  return n;
}

uint bindata_vpack(u8 *buf, const char *fmt, va_list argp)
{
  const struct bdprog *prog;
  struct bdcmd *cmd;
  struct pack_fns *fns;
  uint i, n;

  prog = bdlang_compile(fmt);

  switch (prog->endian) {
  case ENDIAN_NATIVE:
    fns = &nat_pack_fns;
    break;
  case ENDIAN_LITTLE:
    fns = &le_pack_fns;
    break;
  case ENDIAN_BIG:
    fns = &be_pack_fns;
    break;
  default:
    exit(1);
  }

  for (i = 0, n = 0; i < prog->ncmds; i += 1) {
    cmd = prog->cmds + i;

    switch (cmd->cmdtype) {
    case TYPE_REGISTER:
      n += pack_register(buf + n, cmd->inttype, cmd->coef.val, fns, argp);
      break;
    case TYPE_ARRAY:
      n += pack_array(buf + n, cmd->inttype, cmd->coef.val, fns, argp);
      break;
    case TYPE_VECTOR:
      n += pack_vector(buf + n, cmd->inttype,
                       cmd->coef.rng.min, cmd->coef.rng.max,
                       fns, argp);
      break;
    default:
      exit(1);
    }
  }

  return n;
}

uint bindata_pack(u8 *buf, const char *fmt, ...)
{
  uint n;
  va_list argp;
  va_start(argp, fmt);
  n = bindata_vpack(buf, fmt, argp);
  va_end(argp);
  return n;
}

static uint unpack_i8(const u8 *buf, i8 *p)
{
  *p = buf[0];
  return sizeof *p;
}

static uint unpack_u8(const u8 *buf, u8 *p)
{
  *p = buf[0];
  return sizeof *p;
}

static uint nat_unpack_i16(const u8 *buf, i16 *p)
{
  *p = *(i16 *)buf;
  return sizeof *p;
}

static uint le_unpack_i16(const u8 *buf, i16 *p)
{
  *p = ((buf[0]) |
        (buf[1] << 8));
  return sizeof *p;
}

static uint be_unpack_i16(const u8 *buf, i16 *p)
{
  *p = (((i16) buf[1]) |
        ((i16) buf[0] << 8));
  return sizeof *p;
}

static uint nat_unpack_u16(const u8 *buf, u16 *p)
{
  *p = *(u16 *)buf;
  return sizeof *p;
}

static uint le_unpack_u16(const u8 *buf, u16 *p)
{
  *p = ((buf[0]) |
        (buf[1] << 8));
  return sizeof *p;
}

static uint be_unpack_u16(const u8 *buf, u16 *p)
{
  *p = ((buf[1]) |
        (buf[0] << 8));
  return sizeof *p;
}

__attribute__ ((noreturn))
static uint nat_unpack_u24(__attribute__ ((unused)) const u8 *buf,
                           __attribute__ ((unused)) u24 *p)
{
  log_abort("not implemented");
}

static uint le_unpack_u24(const u8 *buf, u24 *p)
{
  *p = ((buf[0]) |
        (buf[1] << 8) |
        (buf[2] << 16));
  return 3;
}

static uint be_unpack_u24(const u8 *buf, u24 *p)
{
  *p = ((buf[2]) |
        (buf[1] << 8) |
        (buf[0] << 16));
  return 3;
}

static uint nat_unpack_i32(const u8 *buf, i32 *p)
{
  *p = *(i32 *)buf;
  return sizeof *p;
}

static uint le_unpack_i32(const u8 *buf, i32 *p)
{
  *p = (((long) buf[0]) |
        ((long) buf[1] << 8) |
        ((long) buf[2] << 16) |
        ((long) buf[3] << 24));
  return sizeof *p;
}

static uint be_unpack_i32(const u8 *buf, i32 *p)
{
  *p = (((long) buf[3]) |
        ((long) buf[2] << 8) |
        ((long) buf[1] << 16) |
        ((long) buf[0] << 24));
  return sizeof *p;
}

static uint nat_unpack_u32(const u8 *buf, u32 *p)
{
  *p = *(u32 *)buf;
  return sizeof *p;
}

static uint le_unpack_u32(const u8 *buf, u32 *p)
{
  *p = (((unsigned long) buf[0]) |
        ((unsigned long) buf[1] << 8) |
        ((unsigned long) buf[2] << 16) |
        ((unsigned long) buf[3] << 24));
  return sizeof *p;
}

static uint be_unpack_u32(const u8 *buf, u32 *p)
{
  *p = (((unsigned long) buf[3]) |
        ((unsigned long) buf[2] << 8) |
        ((unsigned long) buf[1] << 16) |
        ((unsigned long) buf[0] << 24));
  return sizeof *p;
}

static uint nat_unpack_i64(const u8 *buf, i64 *p)
{
  *p = *(i64 *)buf;
  return sizeof *p;
}

static uint le_unpack_i64(const u8 *buf, i64 *p)
{
  *p = (((long long) buf[0]) |
        ((long long) buf[1] << 8) |
        ((long long) buf[2] << 16) |
        ((long long) buf[3] << 24) |
        ((long long) buf[4] << 32) |
        ((long long) buf[5] << 40) |
        ((long long) buf[6] << 48) |
        ((long long) buf[7] << 56));
  return sizeof *p;
}

static uint be_unpack_i64(const u8 *buf, i64 *p)
{
  *p = (((long long) buf[7]) |
        ((long long) buf[6] << 8) |
        ((long long) buf[5] << 16) |
        ((long long) buf[4] << 24) |
        ((long long) buf[3] << 32) |
        ((long long) buf[2] << 40) |
        ((long long) buf[1] << 48) |
        ((long long) buf[0] << 56));
  return sizeof *p;
}

static uint nat_unpack_u64(const u8 *buf, u64 *p)
{
  *p = *(u64 *)buf;
  return sizeof *p;
}

static uint le_unpack_u64(const u8 *buf, u64 *p)
{
  *p = (((unsigned long long) buf[0]) |
        ((unsigned long long) buf[1] << 8) |
        ((unsigned long long) buf[2] << 16) |
        ((unsigned long long) buf[3] << 24) |
        ((unsigned long long) buf[4] << 32) |
        ((unsigned long long) buf[5] << 40) |
        ((unsigned long long) buf[6] << 48) |
        ((unsigned long long) buf[7] << 56));
  return sizeof *p;
}

static uint be_unpack_u64(const u8 *buf, u64 *p)
{
  *p = (((unsigned long long) buf[7]) |
        ((unsigned long long) buf[6] << 8) |
        ((unsigned long long) buf[5] << 16) |
        ((unsigned long long) buf[4] << 24) |
        ((unsigned long long) buf[3] << 32) |
        ((unsigned long long) buf[2] << 40) |
        ((unsigned long long) buf[1] << 48) |
        ((unsigned long long) buf[0] << 56));
  return sizeof *p;
}

struct unpack_fns {
  uint (*unpack_i8)(const u8 *, i8 *);
  uint (*unpack_u8)(const u8 *, u8 *);
  uint (*unpack_i16)(const u8 *, i16 *);
  uint (*unpack_u16)(const u8 *, u16 *);
  uint (*unpack_u24)(const u8 *, u24 *);
  uint (*unpack_i32)(const u8 *, i32 *);
  uint (*unpack_u32)(const u8 *, u32 *);
  uint (*unpack_i64)(const u8 *, i64 *);
  uint (*unpack_u64)(const u8 *, u64 *);
};

static struct unpack_fns nat_unpack_fns = {
  .unpack_i8 = unpack_i8,
  .unpack_u8 = unpack_u8,
  .unpack_i16 = nat_unpack_i16,
  .unpack_u16 = nat_unpack_u16,
  .unpack_u24 = nat_unpack_u24,
  .unpack_i32 = nat_unpack_i32,
  .unpack_u32 = nat_unpack_u32,
  .unpack_i64 = nat_unpack_i64,
  .unpack_u64 = nat_unpack_u64
};

static struct unpack_fns le_unpack_fns = {
  .unpack_i8 = unpack_i8,
  .unpack_u8 = unpack_u8,
  .unpack_i16 = le_unpack_i16,
  .unpack_u16 = le_unpack_u16,
  .unpack_u24 = le_unpack_u24,
  .unpack_i32 = le_unpack_i32,
  .unpack_u32 = le_unpack_u32,
  .unpack_i64 = le_unpack_i64,
  .unpack_u64 = le_unpack_u64
};

static struct unpack_fns be_unpack_fns = {
  .unpack_i8 = unpack_i8,
  .unpack_u8 = unpack_u8,
  .unpack_i16 = be_unpack_i16,
  .unpack_u16 = be_unpack_u16,
  .unpack_u24 = be_unpack_u24,
  .unpack_i32 = be_unpack_i32,
  .unpack_u32 = be_unpack_u32,
  .unpack_i64 = be_unpack_i64,
  .unpack_u64 = be_unpack_u64
};

#define UNPACK_SWITCH                           \
  do {                                          \
    switch (inttype) {                          \
      UNPACK_CASE('b', i8);                     \
      UNPACK_CASE('B', u8);                     \
      UNPACK_CASE('h', i16);                    \
      UNPACK_CASE('H', u16);                    \
      UNPACK_CASE('T', u24);                    \
      UNPACK_CASE('l', i32);                    \
      UNPACK_CASE('L', u32);                    \
      UNPACK_CASE('q', i64);                    \
      UNPACK_CASE('Q', u64);                    \
    default:                                    \
      exit(1);                                  \
    }                                           \
  } while (0)

#define UNPACK_CASE(c, type)                                            \
  case c:                                                               \
  for (i = 0, n = 0; i < count; i += 1) {                               \
                                         n += fns->unpack_##type(buf + n, va_arg(argp, type *)); \
                                         }                              \
  break;

    static uint unpack_register(const u8 *buf, int inttype, u64 count,
                                struct unpack_fns *fns, va_list argp)
    {
      uint i, n;
      UNPACK_SWITCH;
      return n;
    }

#undef UNPACK_CASE

#define UNPACK_CASE(c, type)                    \
  case c:                                       \
  {                                             \
    type *p = va_arg(argp, type *);             \
    for (i = 0, n = 0; i < len; i += 1) {       \
      n += fns->unpack_##type(buf + n, p + i);  \
    }                                           \
    break;                                      \
  }

static uint unpack_array(const u8 *buf, int inttype, u64 len,
                         struct unpack_fns *fns, va_list argp)
{
  uint i, n;
  UNPACK_SWITCH;
  return n;
}

#undef UNPACK_CASE

#define UNPACK_CASE(c, type)                    \
  case c:                                       \
  {                                             \
    type *p = (type *)vec->data;                \
    len = vec->len / (sizeof *p);               \
    for (i = 0; i < len; i += 1) {              \
      n += fns->unpack_##type(buf + n, p + i);  \
    }                                           \
    break;                                      \
  }

static uint unpack_vector(const u8 *buf, int inttype, u32 min, u32 max,
                          struct unpack_fns *fns, va_list argp)
{
  uint i, n;
  uint len;
  struct vector *vec;
  vec = va_arg(argp, struct vector *);
  log_assert(vec->len >= min && vec->len <= max,
             "vector length %u, expected <%u..%u>",
             vec->len, min, max);
  /* TODO handle variable size vectors */
  vec->len = 0;
  n = fns->unpack_u8(buf, (u8 *)&vec->len);
  UNPACK_SWITCH;
  return n;
}

uint bindata_vunpack(const u8 *buf, const char *fmt, va_list argp)
{
  const struct bdprog *prog;
  struct bdcmd *cmd;
  struct unpack_fns *fns;
  uint i, n;

  prog = bdlang_compile(fmt);

  switch (prog->endian) {
  case ENDIAN_NATIVE:
    fns = &nat_unpack_fns;
    break;
  case ENDIAN_LITTLE:
    fns = &le_unpack_fns;
    break;
  case ENDIAN_BIG:
    fns = &be_unpack_fns;
    break;
  default:
    exit(1);
  }

  for (i = 0, n = 0; i < prog->ncmds; i += 1) {
    cmd = prog->cmds + i;

    switch (cmd->cmdtype) {
    case TYPE_REGISTER:
      n += unpack_register(buf + n, cmd->inttype, cmd->coef.val, fns, argp);
      break;
    case TYPE_ARRAY:
      n += unpack_array(buf + n, cmd->inttype, cmd->coef.val, fns, argp);
      break;
    case TYPE_VECTOR:
      n += unpack_vector(buf + n, cmd->inttype,
                         cmd->coef.rng.min, cmd->coef.rng.max,
                         fns, argp);
      break;
    default:
      exit(1);
    }
  }

  return n;
}

uint bindata_unpack(const u8 *buf, const char *fmt, ...)
{
  uint n;
  va_list argp;
  va_start(argp, fmt);
  n = bindata_vunpack(buf, fmt, argp);
  va_end(argp);
  return n;
}

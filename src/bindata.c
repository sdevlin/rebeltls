#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>

#include "types.h"
#include "vector.h"

struct cmd {
  int mod;
  int act;
};

static const char *eatspace(const char *fmt)
{
  while (*fmt == ' ') {
    fmt += 1;
  }

  return fmt;
}

static struct cmd getcmd(const char **fmtp)
{
  struct cmd cmd = { 1, 0 };
  const char *fmt;

  fmt = eatspace(*fmtp);

  if (isdigit(*fmt)) {
    cmd.mod = 0;
    while (isdigit(*fmt)) {
      cmd.mod *= 10;
      cmd.mod += (*fmt) - '0';
      fmt += 1;
    }
  }

  fmt = eatspace(fmt);

  cmd.act = *fmt;

  *fmtp = fmt + 1;

  return cmd;
}

static byte *pack_int8(byte *buf, int8 n)
{
  buf[0] = n;
  return buf + sizeof n;
}

static byte *pack_uint8(byte *buf, uint8 n)
{
  buf[0] = n;
  return buf + sizeof n;
}

static byte *nat_pack_int16(byte *buf, int16 n)
{
  *(int16 *)buf = n;
  return buf + sizeof n;
}

static byte *le_pack_int16(byte *buf, int16 n)
{
  buf[0] = 0xff & n;
  buf[1] = 0xff & (n >> 8);
  return buf + sizeof n;
}

static byte *be_pack_int16(byte *buf, int16 n)
{
  buf[1] = 0xff & n;
  buf[0] = 0xff & (n >> 8);
  return buf + sizeof n;
}

static byte *nat_pack_uint16(byte *buf, uint16 n)
{
  *(uint16 *)buf = n;
  return buf + sizeof n;
}

static byte *le_pack_uint16(byte *buf, uint16 n)
{
  buf[0] = 0xff & n;
  buf[1] = 0xff & (n >> 8);
  return buf + sizeof n;
}

static byte *be_pack_uint16(byte *buf, uint16 n)
{
  buf[1] = 0xff & n;
  buf[0] = 0xff & (n >> 8);
  return buf + sizeof n;
}

static byte *nat_pack_int32(byte *buf, int32 n)
{
  *(int32 *)buf = n;
  return buf + sizeof n;
}

static byte *le_pack_int32(byte *buf, int32 n)
{
  buf[0] = 0xff & n;
  buf[1] = 0xff & (n >> 8);
  buf[2] = 0xff & (n >> 16);
  buf[3] = 0xff & (n >> 24);
  return buf + sizeof n;
}

static byte *be_pack_int32(byte *buf, int32 n)
{
  buf[3] = 0xff & n;
  buf[2] = 0xff & (n >> 8);
  buf[1] = 0xff & (n >> 16);
  buf[0] = 0xff & (n >> 24);
  return buf + sizeof n;
}

static byte *nat_pack_uint32(byte *buf, uint32 n)
{
  *(uint32 *)buf = n;
  return buf + sizeof n;
}

static byte *le_pack_uint32(byte *buf, uint32 n)
{
  buf[0] = 0xff & n;
  buf[1] = 0xff & (n >> 8);
  buf[2] = 0xff & (n >> 16);
  buf[3] = 0xff & (n >> 24);
  return buf + sizeof n;
}

static byte *be_pack_uint32(byte *buf, uint32 n)
{
  buf[3] = 0xff & n;
  buf[2] = 0xff & (n >> 8);
  buf[1] = 0xff & (n >> 16);
  buf[0] = 0xff & (n >> 24);
  return buf + sizeof n;
}

static byte *nat_pack_int64(byte *buf, int64 n)
{
  *(int64 *)buf = n;
  return buf + sizeof n;
}

static byte *le_pack_int64(byte *buf, int64 n)
{
  buf[0] = 0xff & n;
  buf[1] = 0xff & (n >> 8);
  buf[2] = 0xff & (n >> 16);
  buf[3] = 0xff & (n >> 24);
  buf[4] = 0xff & (n >> 32);
  buf[5] = 0xff & (n >> 40);
  buf[6] = 0xff & (n >> 48);
  buf[7] = 0xff & (n >> 56);
  return buf + sizeof n;
}

static byte *be_pack_int64(byte *buf, int64 n)
{
  buf[7] = 0xff & n;
  buf[6] = 0xff & (n >> 8);
  buf[5] = 0xff & (n >> 16);
  buf[4] = 0xff & (n >> 24);
  buf[3] = 0xff & (n >> 32);
  buf[2] = 0xff & (n >> 40);
  buf[1] = 0xff & (n >> 48);
  buf[0] = 0xff & (n >> 56);
  return buf + sizeof n;
}

static byte *nat_pack_uint64(byte *buf, uint64 n)
{
  *(uint64 *)buf = n;
  return buf + sizeof n;
}

static byte *le_pack_uint64(byte *buf, uint64 n)
{
  buf[0] = 0xff & n;
  buf[1] = 0xff & (n >> 8);
  buf[2] = 0xff & (n >> 16);
  buf[3] = 0xff & (n >> 24);
  buf[4] = 0xff & (n >> 32);
  buf[5] = 0xff & (n >> 40);
  buf[6] = 0xff & (n >> 48);
  buf[7] = 0xff & (n >> 56);
  return buf + sizeof n;
}

static byte *be_pack_uint64(byte *buf, uint64 n)
{
  buf[7] = 0xff & n;
  buf[6] = 0xff & (n >> 8);
  buf[5] = 0xff & (n >> 16);
  buf[4] = 0xff & (n >> 24);
  buf[3] = 0xff & (n >> 32);
  buf[2] = 0xff & (n >> 40);
  buf[1] = 0xff & (n >> 48);
  buf[0] = 0xff & (n >> 56);
  return buf + sizeof n;
}

struct pack_fns {
  byte *(*pack_int8)(byte *, int8);
  byte *(*pack_uint8)(byte *, uint8);
  byte *(*pack_int16)(byte *, int16);
  byte *(*pack_uint16)(byte *, uint16);
  byte *(*pack_int32)(byte *, int32);
  byte *(*pack_uint32)(byte *, uint32);
  byte *(*pack_int64)(byte *, int64);
  byte *(*pack_uint64)(byte *, uint64);
};

static struct pack_fns nat_pack_fns = {
  .pack_int8 = &pack_int8,
  .pack_uint8 = &pack_uint8,
  .pack_int16 = &nat_pack_int16,
  .pack_uint16 = &nat_pack_uint16,
  .pack_int32 = &nat_pack_int32,
  .pack_uint32 = &nat_pack_uint32,
  .pack_int64 = &nat_pack_int64,
  .pack_uint64 = &nat_pack_uint64
};

static struct pack_fns le_pack_fns = {
  .pack_int8 = &pack_int8,
  .pack_uint8 = &pack_uint8,
  .pack_int16 = &le_pack_int16,
  .pack_uint16 = &le_pack_uint16,
  .pack_int32 = &le_pack_int32,
  .pack_uint32 = &le_pack_uint32,
  .pack_int64 = &le_pack_int64,
  .pack_uint64 = &le_pack_uint64
};

static struct pack_fns be_pack_fns = {
  .pack_int8 = &pack_int8,
  .pack_uint8 = &pack_uint8,
  .pack_int16 = &be_pack_int16,
  .pack_uint16 = &be_pack_uint16,
  .pack_int32 = &be_pack_int32,
  .pack_uint32 = &be_pack_uint32,
  .pack_int64 = &be_pack_int64,
  .pack_uint64 = &be_pack_uint64
};

#define PACKN(ptype, atype)                            \
  for (i = 0; i < cmd.mod; i += 1)                     \
    buf = fns->pack_##ptype(buf, va_arg(argp, atype))

byte *bindata_vpack(byte *buf, const char *fmt, va_list argp)
{
  struct cmd cmd;
  struct pack_fns *fns = &le_pack_fns;
  int i;

  for (; ; ) {
    cmd = getcmd(&fmt);

    switch (cmd.act) {

    case '=':
      fns = &nat_pack_fns;
      break;

    case '<':
      fns = &le_pack_fns;
      break;

    case '>':
    case '!':
      fns = &be_pack_fns;
      break;

    case 'b':
      PACKN(int8, int);
      break;

    case 'B':
      PACKN(uint8, unsigned);
      break;

    case 'h':
      PACKN(int16, int);
      break;

    case 'H':
      PACKN(uint16, unsigned);
      break;

    case 'l':
      PACKN(int32, long);
      break;

    case 'L':
      PACKN(uint32, unsigned long);
      break;

    case 'q':
      PACKN(int64, long long);
      break;

    case 'Q':
      PACKN(uint64, unsigned long long);
      break;

    case '\0':
      return buf;

    default:
      exit(1);
    }
  }
}

byte *bindata_pack(byte *buf, const char *fmt, ...)
{
  va_list argp;
  va_start(argp, fmt);
  buf = bindata_vpack(buf, fmt, argp);
  va_end(argp);
  return buf;
}

static const byte *unpack_int8(const byte *buf, int8 *p)
{
  *p = buf[0];
  return buf + sizeof *p;
}

static const byte *unpack_uint8(const byte *buf, uint8 *p)
{
  *p = buf[0];
  return buf + sizeof *p;
}

static const byte *nat_unpack_int16(const byte *buf, int16 *p)
{
  *p = *(int16 *)buf;
  return buf + sizeof *p;
}

static const byte *le_unpack_int16(const byte *buf, int16 *p)
{
  *p = ((buf[0]) |
        (buf[1] << 8));
  return buf + sizeof *p;
}

static const byte *be_unpack_int16(const byte *buf, int16 *p)
{
  *p = (((int16) buf[1]) |
        ((int16) buf[0] << 8));
  return buf + sizeof *p;
}

static const byte *nat_unpack_uint16(const byte *buf, uint16 *p)
{
  *p = *(uint16 *)buf;
  return buf + sizeof *p;
}

static const byte *le_unpack_uint16(const byte *buf, uint16 *p)
{
  *p = ((buf[0]) |
        (buf[1] << 8));
  return buf + sizeof *p;
}

static const byte *be_unpack_uint16(const byte *buf, uint16 *p)
{
  *p = ((buf[1]) |
        (buf[0] << 8));
  return buf + sizeof *p;
}

static const byte *nat_unpack_int32(const byte *buf, int32 *p)
{
  *p = *(int32 *)buf;
  return buf + sizeof *p;
}

static const byte *le_unpack_int32(const byte *buf, int32 *p)
{
  *p = (((long) buf[0]) |
        ((long) buf[1] << 8) |
        ((long) buf[2] << 16) |
        ((long) buf[3] << 24));
  return buf + sizeof *p;
}

static const byte *be_unpack_int32(const byte *buf, int32 *p)
{
  *p = (((long) buf[3]) |
        ((long) buf[2] << 8) |
        ((long) buf[1] << 16) |
        ((long) buf[0] << 24));
  return buf + sizeof *p;
}

static const byte *nat_unpack_uint32(const byte *buf, uint32 *p)
{
  *p = *(uint32 *)buf;
  return buf + sizeof *p;
}

static const byte *le_unpack_uint32(const byte *buf, uint32 *p)
{
  *p = (((unsigned long) buf[0]) |
        ((unsigned long) buf[1] << 8) |
        ((unsigned long) buf[2] << 16) |
        ((unsigned long) buf[3] << 24));
  return buf + sizeof *p;
}

static const byte *be_unpack_uint32(const byte *buf, uint32 *p)
{
  *p = (((unsigned long) buf[3]) |
        ((unsigned long) buf[2] << 8) |
        ((unsigned long) buf[1] << 16) |
        ((unsigned long) buf[0] << 24));
  return buf + sizeof *p;
}

static const byte *nat_unpack_int64(const byte *buf, int64 *p)
{
  *p = *(int64 *)buf;
  return buf + sizeof *p;
}

static const byte *le_unpack_int64(const byte *buf, int64 *p)
{
  *p = (((long long) buf[0]) |
        ((long long) buf[1] << 8) |
        ((long long) buf[2] << 16) |
        ((long long) buf[3] << 24) |
        ((long long) buf[4] << 32) |
        ((long long) buf[5] << 40) |
        ((long long) buf[6] << 48) |
        ((long long) buf[7] << 56));
  return buf + sizeof *p;
}

static const byte *be_unpack_int64(const byte *buf, int64 *p)
{
  *p = (((long long) buf[7]) |
        ((long long) buf[6] << 8) |
        ((long long) buf[5] << 16) |
        ((long long) buf[4] << 24) |
        ((long long) buf[3] << 32) |
        ((long long) buf[2] << 40) |
        ((long long) buf[1] << 48) |
        ((long long) buf[0] << 56));
  return buf + sizeof *p;
}

static const byte *nat_unpack_uint64(const byte *buf, uint64 *p)
{
  *p = *(uint64 *)buf;
  return buf + sizeof *p;
}

static const byte *le_unpack_uint64(const byte *buf, uint64 *p)
{
  *p = (((unsigned long long) buf[0]) |
        ((unsigned long long) buf[1] << 8) |
        ((unsigned long long) buf[2] << 16) |
        ((unsigned long long) buf[3] << 24) |
        ((unsigned long long) buf[4] << 32) |
        ((unsigned long long) buf[5] << 40) |
        ((unsigned long long) buf[6] << 48) |
        ((unsigned long long) buf[7] << 56));
  return buf + sizeof *p;
}

static const byte *be_unpack_uint64(const byte *buf, uint64 *p)
{
  *p = (((unsigned long long) buf[7]) |
        ((unsigned long long) buf[6] << 8) |
        ((unsigned long long) buf[5] << 16) |
        ((unsigned long long) buf[4] << 24) |
        ((unsigned long long) buf[3] << 32) |
        ((unsigned long long) buf[2] << 40) |
        ((unsigned long long) buf[1] << 48) |
        ((unsigned long long) buf[0] << 56));
  return buf + sizeof *p;
}

struct unpack_fns {
  const byte *(*unpack_int8)(const byte *, int8 *);
  const byte *(*unpack_uint8)(const byte *, uint8 *);
  const byte *(*unpack_int16)(const byte *, int16 *);
  const byte *(*unpack_uint16)(const byte *, uint16 *);
  const byte *(*unpack_int32)(const byte *, int32 *);
  const byte *(*unpack_uint32)(const byte *, uint32 *);
  const byte *(*unpack_int64)(const byte *, int64 *);
  const byte *(*unpack_uint64)(const byte *, uint64 *);
};

static struct unpack_fns nat_unpack_fns = {
  .unpack_int8 = unpack_int8,
  .unpack_uint8 = unpack_uint8,
  .unpack_int16 = nat_unpack_int16,
  .unpack_uint16 = nat_unpack_uint16,
  .unpack_int32 = nat_unpack_int32,
  .unpack_uint32 = nat_unpack_uint32,
  .unpack_int64 = nat_unpack_int64,
  .unpack_uint64 = nat_unpack_uint64
};

static struct unpack_fns le_unpack_fns = {
  .unpack_int8 = unpack_int8,
  .unpack_uint8 = unpack_uint8,
  .unpack_int16 = le_unpack_int16,
  .unpack_uint16 = le_unpack_uint16,
  .unpack_int32 = le_unpack_int32,
  .unpack_uint32 = le_unpack_uint32,
  .unpack_int64 = le_unpack_int64,
  .unpack_uint64 = le_unpack_uint64
};

static struct unpack_fns be_unpack_fns = {
  .unpack_int8 = unpack_int8,
  .unpack_uint8 = unpack_uint8,
  .unpack_int16 = be_unpack_int16,
  .unpack_uint16 = be_unpack_uint16,
  .unpack_int32 = be_unpack_int32,
  .unpack_uint32 = be_unpack_uint32,
  .unpack_int64 = be_unpack_int64,
  .unpack_uint64 = be_unpack_uint64
};

const byte *bindata_vunpack(const byte *buf, const char *fmt, va_list argp)
{
  struct unpack_fns *fns = &nat_unpack_fns;

  for (; ; fmt += 1) {
    switch (*fmt) {

    case '=':
      fns = &nat_unpack_fns;
      break;

    case '<':
      fns = &le_unpack_fns;
      break;

    case '>':
    case '!':
      fns = &be_unpack_fns;
      break;

    case ' ':
      /* nop */
      break;

    case 'b':
      buf = fns->unpack_int8(buf, va_arg(argp, int8 *));
      break;

    case 'B':
      buf = fns->unpack_uint8(buf, va_arg(argp, uint8 *));
      break;

    case 'h':
      buf = fns->unpack_int16(buf, va_arg(argp, int16 *));
      break;

    case 'H':
      buf = fns->unpack_uint16(buf, va_arg(argp, uint16 *));
      break;

    case 'l':
      buf = fns->unpack_int32(buf, va_arg(argp, int32 *));
      break;

    case 'L':
      buf = fns->unpack_uint32(buf, va_arg(argp, uint32 *));
      break;

    case 'q':
      buf = fns->unpack_int64(buf, va_arg(argp, int64 *));
      break;

    case 'Q':
      buf = fns->unpack_uint64(buf, va_arg(argp, uint64 *));
      break;

    case 'v':

      break;

    case 'V':

      break;

    case '\0':
      return buf;

    default:
      exit(1);
    }
  }
}

const byte *bindata_unpack(const byte *buf, const char *fmt, ...)
{
  va_list argp;
  va_start(argp, fmt);
  buf = bindata_vunpack(buf, fmt, argp);
  va_end(argp);
  return buf;
}

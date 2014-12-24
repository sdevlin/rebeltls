#ifndef MD_DEFN_H
#define MD_DEFN_H

#define DEFINE_MD(name)                                             \
  void name##_init(name##_ctx *ctx)                                 \
  {                                                                 \
    ctx->mlen = 0;                                                  \
    memcpy(ctx->h, name##_initstate, sizeof ctx->h);                \
  }                                                                 \
                                                                    \
  name##_ctx *name##_new(void)                                      \
  {                                                                 \
    name##_ctx *ctx;                                                \
    ctx = malloc(sizeof *ctx);                                      \
    name##_init(ctx);                                               \
    return ctx;                                                     \
  }                                                                 \
                                                                    \
  void name##_reset(name##_ctx *ctx)                                \
  {                                                                 \
    name##_init(ctx);                                               \
  }                                                                 \
                                                                    \
  /* TODO maybe move this to a global utility package */            \
  static uint min(uint a, uint b)                                   \
  {                                                                 \
    return a < b ? a : b;                                           \
  }                                                                 \
                                                                    \
  __attribute__ ((unused)) static const name##_ctx dummy;           \
  static const byte pad[sizeof dummy.buf] = { 0x80 };               \
  static const uint buflen = sizeof dummy.buf;                      \
  static const uint mlenoffset = buflen - (2 * sizeof dummy.h[0]);  \
                                                                    \
  void name##_update(name##_ctx *ctx, const byte *m, uint mlen)     \
  {                                                                 \
    uint pos;                                                       \
    uint take;                                                      \
                                                                    \
    while (mlen > 0) {                                              \
      pos = ctx->mlen % buflen;                                     \
      take = min(buflen - pos, mlen);                               \
                                                                    \
      memcpy(ctx->buf + pos, m, take);                              \
      ctx->mlen += take;                                            \
                                                                    \
      m += take;                                                    \
      mlen -= take;                                                 \
                                                                    \
      if (ctx->mlen % buflen == 0) {                                \
        name##_compress(ctx);                                       \
      }                                                             \
    }                                                               \
  }                                                                 \
                                                                    \
  void name##_final(name##_ctx *ctx, byte *h)                       \
  {                                                                 \
    uint pos;                                                       \
    uint diff;                                                      \
                                                                    \
    pos = ctx->mlen % buflen;                                       \
                                                                    \
    if (pos >= mlenoffset) {                                        \
      diff = buflen - pos;                                          \
      memcpy(ctx->buf + pos, pad, diff);                            \
      name##_compress(ctx);                                         \
      memcpy(ctx->buf, pad + diff, mlenoffset);                     \
    } else {                                                        \
      memcpy(ctx->buf + pos, pad, mlenoffset - pos);                \
    }                                                               \
                                                                    \
    packmlen(ctx);                                                  \
    name##_compress(ctx);                                           \
    packh(ctx, h);                                                  \
  }                                                                 \
                                                                    \
  void name##_digest(const byte *m, uint mlen, byte *h)             \
  {                                                                 \
    name##_ctx ctx;                                                 \
                                                                    \
    name##_init(&ctx);                                              \
    name##_update(&ctx, m, mlen);                                   \
    name##_final(&ctx, h);                                          \
  }

#endif

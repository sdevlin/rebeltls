#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "bytes.h"
#include "md.h"
#include "sha384.h"
#include "sha512.h"
#include "types.h"

static const uint64 h[] = {
  0xcbbb9d5dc1059ed8, 0x629a292a367cd507,
  0x9159015a3070dd17, 0x152fecd8f70e5939,
  0x67332667ffc00b31, 0x8eb44a8768581511,
  0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4
};

void sha384_init(sha384_ctx *ctx)
{
  ctx->mlen = 0;
  memcpy(ctx->h, h, sizeof ctx->h);
}

sha384_ctx *sha384_new(void)
{
  sha384_ctx *ctx;
  ctx = malloc(sizeof *ctx);
  sha384_init(ctx);
  return ctx;
}

void sha384_reset(sha384_ctx *ctx)
{
  sha384_init(ctx);
}

void sha384_compress(sha384_ctx *ctx)
{
  sha512_compress(ctx);
}

static byte *md_buffer(md_ctx *ctx)
{
  return ((sha384_ctx *)ctx)->buf;
}

static void md_compress(md_ctx *ctx)
{
  sha384_compress((sha384_ctx *)ctx);
}

static void md_packmlen(md_ctx *ctx)
{
  bytes_pack(md_buffer(ctx) + 112, "> QQ", 0, ctx->mlen << 3);
}

static const md_defn defn = {
  .buflen = 128,
  .buffer = &md_buffer,
  .compress = &md_compress,
  .mlenoffset = 112,
  .packmlen = &md_packmlen
};

void sha384_update(sha384_ctx *ctx, const byte *m, uint mlen)
{
  md_update(&defn, (md_ctx *)ctx, m, mlen);
}

void sha384_final(sha384_ctx *ctx, byte *h)
{
  md_final(&defn, (md_ctx *)ctx);
  bytes_pack(h, "> 6Q",
             ctx->h[0], ctx->h[1], ctx->h[2],
             ctx->h[3], ctx->h[4], ctx->h[5]);
}

void sha384_digest(const byte *m, uint mlen, byte *h)
{
  sha384_ctx ctx;

  sha384_init(&ctx);
  sha384_update(&ctx, m, mlen);
  sha384_final(&ctx, h);
}
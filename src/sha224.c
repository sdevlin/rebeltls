#include <stdlib.h>
#include <string.h>

#include "bindata.h"
#include "md.h"
#include "sha224.h"
#include "sha256.h"
#include "types.h"

static const uint32 h[] = {
  0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
  0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
};

void sha224_init(sha224_ctx *ctx)
{
  ctx->mlen = 0;
  memcpy(ctx->h, h, sizeof ctx->h);
}

sha224_ctx *sha224_new(void)
{
  sha224_ctx *ctx;
  ctx = malloc(sizeof *ctx);
  sha224_init(ctx);
  return ctx;
}

void sha224_reset(sha224_ctx *ctx)
{
  sha224_init(ctx);
}

void sha224_compress(sha224_ctx *ctx)
{
  sha256_compress(ctx);
}

static byte *md_buffer(md_ctx *ctx)
{
  return ((sha224_ctx *)ctx)->buf;
}

static void md_compress(md_ctx *ctx)
{
  sha224_compress((sha224_ctx *)ctx);
}

static void md_packmlen(md_ctx *ctx)
{
  bindata_pack(md_buffer(ctx) + 56, "> Q", ctx->mlen << 3);
}

static const md_defn defn = {
  .buflen = 64,
  .buffer = &md_buffer,
  .compress = &md_compress,
  .mlenoffset = 56,
  .packmlen = &md_packmlen
};

void sha224_update(sha224_ctx *ctx, const byte *m, uint mlen)
{
  md_update(&defn, (md_ctx *)ctx, m, mlen);
}

void sha224_final(sha224_ctx *ctx, byte *h)
{
  md_final(&defn, (md_ctx *)ctx);
  bindata_pack(h, "> 7L",
               ctx->h[0], ctx->h[1], ctx->h[2], ctx->h[3],
               ctx->h[4], ctx->h[5], ctx->h[6]);
}

void sha224_digest(const byte *m, uint mlen, byte *h)
{
  sha224_ctx ctx;

  sha224_init(&ctx);
  sha224_update(&ctx, m, mlen);
  sha224_final(&ctx, h);
}

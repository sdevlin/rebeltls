#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "bytes.h"
#include "md.h"
#include "sha512.h"
#include "types.h"

void sha512_init(sha512_ctx *ctx)
{
  ctx->mlen = 0;
}

sha512_ctx *sha512_new(void)
{
  sha512_ctx *ctx;
  ctx = malloc(sizeof *ctx);
  sha512_init(ctx);
  return ctx;
}

void sha512_compress(sha512_ctx *ctx)
{
  ctx->mlen = 0;
}

static byte *md_buffer(md_ctx *ctx)
{
  return ((sha512_ctx *)ctx)->buf;
}

static void md_compress(md_ctx *ctx)
{
  sha512_compress((sha512_ctx *)ctx);
}

static void md_packmlen(md_ctx *ctx)
{
  bytes_pack(md_buffer(ctx) + 112, "> 2Q", 0, ctx->mlen << 3);
}

static const md_defn defn = {
  .buflen = 128,
  .buffer = &md_buffer,
  .compress = &md_compress,
  .mlenoffset = 112,
  .packmlen = &md_packmlen
};

void sha512_update(sha512_ctx *ctx, const byte *m, uint mlen)
{
  md_update(&defn, (md_ctx *)ctx, m, mlen);
}

void sha512_final(sha512_ctx *ctx, byte *h)
{
  md_final(&defn, (md_ctx *)ctx);
  bytes_pack(h, "> 8L",
             ctx->h[0], ctx->h[1], ctx->h[2], ctx->h[3],
             ctx->h[4], ctx->h[5], ctx->h[6], ctx->h[7]);
}

void sha512_digest(const byte *m, uint mlen, byte *h)
{
  sha512_ctx ctx;

  sha512_init(&ctx);
  sha512_update(&ctx, m, mlen);
  sha512_final(&ctx, h);
}

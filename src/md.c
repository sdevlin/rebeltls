#include <string.h>

#include "bytes.h"
#include "md.h"
#include "types.h"

static uint min(uint a, uint b)
{
  return a < b ? a : b;
}

void md_update(const md_defn *defn, md_ctx *ctx, const byte *m, uint mlen)
{
  uint offset, take;

  while (mlen > 0) {
    offset = ctx->mlen % defn->buflen;
    take = min(defn->buflen - offset, mlen);

    memcpy(defn->buffer(ctx) + offset, m, take);
    ctx->mlen += take;

    m += take;
    mlen -= take;

    if (ctx->mlen % defn->buflen == 0) {
      defn->compress(ctx);
    }
  }
}

void md_final(const md_defn *defn, md_ctx *ctx, byte *h)
{
  byte *buf;
  uint buflen;
  uint i;
  uint offset;

  buf = defn->buffer(ctx);
  buflen = defn->buflen;
  offset = buflen - defn->mlensize;

  i = ctx->mlen % buflen;
  buf[i] = 0x80;
  i = (i + 1) % buflen;

  for (; i != offset; i = (i + 1) % buflen) {
    if (i == 0) {
      defn->compress(ctx);
    }

    buf[i] = 0;
  }

  /* maybe just call a packmlen function */
  bytes_pack(buf + offset, "> Q", ctx->mlen << 3);

  defn->compress(ctx);

  /* maybe just call a packhash function */
  bytes_pack(h, "> 8L",
             ctx->h[0], ctx->h[1], ctx->h[2], ctx->h[3],
             ctx->h[4], ctx->h[5], ctx->h[6], ctx->h[7]);
}

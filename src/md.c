#include <string.h>

#include "bytes.h"
#include "md.h"
#include "inttypes.h"

static uint min(uint a, uint b)
{
  return a < b ? a : b;
}

void md_update(const md_defn *defn, md_ctx *ctx, const u8 *m, uint mlen)
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

void md_final(const md_defn *defn, md_ctx *ctx)
{
  u8 *buf;
  uint buflen;
  uint i;

  buf = defn->buffer(ctx);
  buflen = defn->buflen;

  i = ctx->mlen % buflen;
  buf[i] = 0x80;
  i = (i + 1) % buflen;

  for (; i != defn->mlenoffset; i = (i + 1) % buflen) {
    if (i == 0) {
      defn->compress(ctx);
    }

    buf[i] = 0;
  }

  defn->packmlen(ctx);
  defn->compress(ctx);
}

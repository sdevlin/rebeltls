static uint min(uint a, uint b)
{
  return a < b ? a : b;
}

void MD_NAME##_update(MD_NAME##_ctx *ctx, const byte *m, uint mlen)
{
  uint offset, take;
  const uint buflen = sizeof ctx->buf;

  while (mlen > 0) {
    offset = ctx->mlen % buflen;
    take = min(buflen - offset, mlen);

    memcpy(ctx->buffer + offset, m, take);
    ctx->mlen += take;

    m += take;
    mlen -= take;

    if (ctx->mlen % buflen == 0) {
      MD_NAME##_compress(ctx);
    }
  }
}

static const MD_NAME##_ctx dummy;
static const byte pad[sizeof dummy.buf] = { 0x80 };
static const uint buflen = sizeof dummy.buf;
static const uint mlenoffset = buflen - (2 * sizeof dummy.h[0]);

void MD_NAME##_final(MD_NAME##_ctx *ctx)
{
  uint pos;
  uint diff;

  pos = ctx->mlen % buflen;

  if (pos >= mlenoffset) {
    diff = buflen - pos;
    memcpy(ctx->buf + pos, pad, diff);
    MD_NAME##_compress(ctx);
    memcpy(ctx->buf, pad + diff, mlenoffset);
  } else {
    memcpy(ctx->buf + pos, pad, mlenoffset - pos);
  }

  bindata_pack(ctx->buf + mlenoffset, MD_MLENPACKFMT, ctx->mlen << 3);
  MD_NAME##_compress(ctx);
}

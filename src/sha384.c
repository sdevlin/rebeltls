#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "bindata.h"
#include "md_defn.h"
#include "sha384.h"
#include "sha512.h"
#include "inttypes.h"

const u64 sha384_initstate[] = {
  0xcbbb9d5dc1059ed8, 0x629a292a367cd507,
  0x9159015a3070dd17, 0x152fecd8f70e5939,
  0x67332667ffc00b31, 0x8eb44a8768581511,
  0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4
};

void sha384_compress(sha384_ctx *ctx)
{
  sha512_compress(ctx);
}

static void packmlen(sha384_ctx *ctx)
{
  bindata_pack(ctx->buf + 112, "> QQ", 0, ctx->mlen << 3);
}

static void packh(sha384_ctx *ctx, u8 *h)
{
  bindata_pack(h, "> Q[6]", ctx->h);
}

DEFINE_MD(sha384)

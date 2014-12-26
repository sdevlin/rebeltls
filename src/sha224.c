#include <stdlib.h>
#include <string.h>

#include "bindata.h"
#include "md_defn.h"
#include "sha224.h"
#include "sha256.h"
#include "inttypes.h"

const u32 sha224_initstate[] = {
  0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
  0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
};

void sha224_compress(sha224_ctx *ctx)
{
  sha256_compress(ctx);
}

static void packmlen(sha224_ctx *ctx)
{
  bindata_pack(ctx->buf + 56, "> Q", ctx->mlen << 3);
}

static void packh(sha224_ctx *ctx, u8 *h)
{
  bindata_pack(h, "> L[7]", ctx->h);
}

DEFINE_MD(sha224)

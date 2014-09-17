#include <string.h>

#include "bindata.h"
#include "io.h"
#include "record.h"
#include "types.h"

void record_init(struct record *rec, uint8 content_type,
                 byte *fragment, uint16 length)
{
  /* TODO validate content type */
  rec->content_type = content_type;
  rec->protocol_version.major = 3;
  rec->protocol_version.minor = 3;
  /* TODO validate length */
  rec->length = length;
  /* TODO validate fragment not null */
  memcpy(rec->fragment, fragment, length);
}

int record_read(io *io, struct record *rec)
{
  byte buf[5];

  io_read(io, buf, sizeof buf);

  bindata_unpack(buf, "! B BB H",
                 &rec->content_type,
                 &rec->protocol_version.major,
                 &rec->protocol_version.minor,
                 &rec->length);

  io_read(io, rec->fragment, rec->length);

  return 0;
}

void record_write(io *io, const struct record *rec)
{
  byte buf[5];

  bindata_pack(buf, "! B BB H",
               rec->content_type,
               rec->protocol_version.major,
               rec->protocol_version.minor,
               rec->length);

  io_write(io, buf, sizeof buf);
  io_write(io, rec->fragment, rec->length);
}

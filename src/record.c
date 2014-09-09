#include "bytes.h"
#include "io.h"
#include "record.h"
#include "types.h"

int record_read(int fd, struct record *rec)
{
  byte buf[5];

  io_read(fd, buf, sizeof buf);

  bytes_unpack(buf, "! B BB H",
               &rec->content_type,
               &rec->protocol_version.major,
               &rec->protocol_version.minor,
               &rec->length);

  if (rec->length > RECORD_MAX_LEN) {
    io_seek(fd, rec->length);
    return 1;
  }

  io_read(fd, rec->fragment, rec->length);

  return 0;
}

void record_write(int fd, const struct record *rec)
{
  byte buf[5];

  bytes_pack(buf, "! B BB H",
             rec->content_type,
             rec->protocol_version.major,
             rec->protocol_version.minor,
             rec->length);

  io_write(fd, buf, sizeof buf);
  io_write(fd, rec->fragment, rec->length);
}

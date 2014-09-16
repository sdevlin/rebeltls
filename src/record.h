#ifndef TLS_RECORD_H
#define TLS_RECORD_H

#include "io.h"
#include "types.h"

#define RECORD_MAX_LEN 18432

enum {
  CT_CHANGE_CIPHER_SPEC = 20,
  CT_ALERT = 21,
  CT_HANDSHAKE = 22,
  CT_APPLICATION_DATA = 23
};

struct record {
  uint8 content_type;
  struct {
    uint8 major;
    uint8 minor;
  } protocol_version;
  uint16 length;
  byte fragment[RECORD_MAX_LEN];
};

extern void record_init(struct record *rec,
                        uint8 content_type,
                        byte *fragment,
                        uint16 length);

extern int record_read(io *io, struct record *rec);
extern void record_write(io *io, const struct record *rec);

#endif

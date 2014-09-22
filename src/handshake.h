#ifndef REBELTLS_HANDSHAKE_H
#define REBELTLS_HANDSHAKE_H

enum {
  HT_HELLO_REQUEST = 0,
  HT_CLIENT_HELLO = 1,
  HT_SERVER_HELLO = 2,
  HT_CERTIFICATE = 11,
  HT_SERVER_KEY_EXCHANGE = 12,
  HT_CERTIFICATE_REQUEST = 13,
  HT_SERVER_HELLO_DONE = 14,
  HT_CERTIFICATE_VERIFY = 15,
  HT_CLIENT_KEY_EXCHANGE = 16,
  HT_FINISHED = 20
};

struct handshake {
  uint8 msg_type;
  uint24 length;
  union {
    struct { } client_hello;
    struct { } server_hello;
    struct { } certificate;
    struct { } server_key_exchange;
    struct { } certificate_request;
    struct { } server_hello_done;
    struct { } certificate_verify;
    struct { } client_key_exchange;
    struct { } finished;
  };
};

#endif

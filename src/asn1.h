#ifndef REBELTLS_ASN1_H
#define REBELTLS_ASN1_H

typedef enum {
  TAG_INT      = 0x02,
  TAG_BITSTR   = 0x03,
  TAG_OCTSTR   = 0x04,
  TAG_NULL     = 0x05,
  TAG_OID      = 0x06,
  TAG_SEQ      = 0x10,
  TAG_SET      = 0x11,
  TAG_PRINTSTR = 0x13,
  TAG_T61STR   = 0x14,
  TAG_IA5STR   = 0x16,
  TAG_UTCTIME  = 0x17
} asn1_tag;

typedef struct {
  asn1_tag tag;
} asn1_node;

extern const asn1_node *asn1_parse(const char *desc);

#endif

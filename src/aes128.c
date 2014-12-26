#include <string.h>

#include "bindata.h"
#include "inttypes.h"
#include "log.h"
#include "bytes.h"
#include "aes128.h"

/* the layout of this combined table assumes little-endianness */
static const uint8 enc_T[] = {
  0xa5, 0x63, 0x63, 0xc6, 0xa5, 0x63, 0x63, 0x00,
  0x84, 0x7c, 0x7c, 0xf8, 0x84, 0x7c, 0x7c, 0x00,
  0x99, 0x77, 0x77, 0xee, 0x99, 0x77, 0x77, 0x00,
  0x8d, 0x7b, 0x7b, 0xf6, 0x8d, 0x7b, 0x7b, 0x00,
  0x0d, 0xf2, 0xf2, 0xff, 0x0d, 0xf2, 0xf2, 0x00,
  0xbd, 0x6b, 0x6b, 0xd6, 0xbd, 0x6b, 0x6b, 0x00,
  0xb1, 0x6f, 0x6f, 0xde, 0xb1, 0x6f, 0x6f, 0x00,
  0x54, 0xc5, 0xc5, 0x91, 0x54, 0xc5, 0xc5, 0x00,
  0x50, 0x30, 0x30, 0x60, 0x50, 0x30, 0x30, 0x00,
  0x03, 0x01, 0x01, 0x02, 0x03, 0x01, 0x01, 0x00,
  0xa9, 0x67, 0x67, 0xce, 0xa9, 0x67, 0x67, 0x00,
  0x7d, 0x2b, 0x2b, 0x56, 0x7d, 0x2b, 0x2b, 0x00,
  0x19, 0xfe, 0xfe, 0xe7, 0x19, 0xfe, 0xfe, 0x00,
  0x62, 0xd7, 0xd7, 0xb5, 0x62, 0xd7, 0xd7, 0x00,
  0xe6, 0xab, 0xab, 0x4d, 0xe6, 0xab, 0xab, 0x00,
  0x9a, 0x76, 0x76, 0xec, 0x9a, 0x76, 0x76, 0x00,
  0x45, 0xca, 0xca, 0x8f, 0x45, 0xca, 0xca, 0x00,
  0x9d, 0x82, 0x82, 0x1f, 0x9d, 0x82, 0x82, 0x00,
  0x40, 0xc9, 0xc9, 0x89, 0x40, 0xc9, 0xc9, 0x00,
  0x87, 0x7d, 0x7d, 0xfa, 0x87, 0x7d, 0x7d, 0x00,
  0x15, 0xfa, 0xfa, 0xef, 0x15, 0xfa, 0xfa, 0x00,
  0xeb, 0x59, 0x59, 0xb2, 0xeb, 0x59, 0x59, 0x00,
  0xc9, 0x47, 0x47, 0x8e, 0xc9, 0x47, 0x47, 0x00,
  0x0b, 0xf0, 0xf0, 0xfb, 0x0b, 0xf0, 0xf0, 0x00,
  0xec, 0xad, 0xad, 0x41, 0xec, 0xad, 0xad, 0x00,
  0x67, 0xd4, 0xd4, 0xb3, 0x67, 0xd4, 0xd4, 0x00,
  0xfd, 0xa2, 0xa2, 0x5f, 0xfd, 0xa2, 0xa2, 0x00,
  0xea, 0xaf, 0xaf, 0x45, 0xea, 0xaf, 0xaf, 0x00,
  0xbf, 0x9c, 0x9c, 0x23, 0xbf, 0x9c, 0x9c, 0x00,
  0xf7, 0xa4, 0xa4, 0x53, 0xf7, 0xa4, 0xa4, 0x00,
  0x96, 0x72, 0x72, 0xe4, 0x96, 0x72, 0x72, 0x00,
  0x5b, 0xc0, 0xc0, 0x9b, 0x5b, 0xc0, 0xc0, 0x00,
  0xc2, 0xb7, 0xb7, 0x75, 0xc2, 0xb7, 0xb7, 0x00,
  0x1c, 0xfd, 0xfd, 0xe1, 0x1c, 0xfd, 0xfd, 0x00,
  0xae, 0x93, 0x93, 0x3d, 0xae, 0x93, 0x93, 0x00,
  0x6a, 0x26, 0x26, 0x4c, 0x6a, 0x26, 0x26, 0x00,
  0x5a, 0x36, 0x36, 0x6c, 0x5a, 0x36, 0x36, 0x00,
  0x41, 0x3f, 0x3f, 0x7e, 0x41, 0x3f, 0x3f, 0x00,
  0x02, 0xf7, 0xf7, 0xf5, 0x02, 0xf7, 0xf7, 0x00,
  0x4f, 0xcc, 0xcc, 0x83, 0x4f, 0xcc, 0xcc, 0x00,
  0x5c, 0x34, 0x34, 0x68, 0x5c, 0x34, 0x34, 0x00,
  0xf4, 0xa5, 0xa5, 0x51, 0xf4, 0xa5, 0xa5, 0x00,
  0x34, 0xe5, 0xe5, 0xd1, 0x34, 0xe5, 0xe5, 0x00,
  0x08, 0xf1, 0xf1, 0xf9, 0x08, 0xf1, 0xf1, 0x00,
  0x93, 0x71, 0x71, 0xe2, 0x93, 0x71, 0x71, 0x00,
  0x73, 0xd8, 0xd8, 0xab, 0x73, 0xd8, 0xd8, 0x00,
  0x53, 0x31, 0x31, 0x62, 0x53, 0x31, 0x31, 0x00,
  0x3f, 0x15, 0x15, 0x2a, 0x3f, 0x15, 0x15, 0x00,
  0x0c, 0x04, 0x04, 0x08, 0x0c, 0x04, 0x04, 0x00,
  0x52, 0xc7, 0xc7, 0x95, 0x52, 0xc7, 0xc7, 0x00,
  0x65, 0x23, 0x23, 0x46, 0x65, 0x23, 0x23, 0x00,
  0x5e, 0xc3, 0xc3, 0x9d, 0x5e, 0xc3, 0xc3, 0x00,
  0x28, 0x18, 0x18, 0x30, 0x28, 0x18, 0x18, 0x00,
  0xa1, 0x96, 0x96, 0x37, 0xa1, 0x96, 0x96, 0x00,
  0x0f, 0x05, 0x05, 0x0a, 0x0f, 0x05, 0x05, 0x00,
  0xb5, 0x9a, 0x9a, 0x2f, 0xb5, 0x9a, 0x9a, 0x00,
  0x09, 0x07, 0x07, 0x0e, 0x09, 0x07, 0x07, 0x00,
  0x36, 0x12, 0x12, 0x24, 0x36, 0x12, 0x12, 0x00,
  0x9b, 0x80, 0x80, 0x1b, 0x9b, 0x80, 0x80, 0x00,
  0x3d, 0xe2, 0xe2, 0xdf, 0x3d, 0xe2, 0xe2, 0x00,
  0x26, 0xeb, 0xeb, 0xcd, 0x26, 0xeb, 0xeb, 0x00,
  0x69, 0x27, 0x27, 0x4e, 0x69, 0x27, 0x27, 0x00,
  0xcd, 0xb2, 0xb2, 0x7f, 0xcd, 0xb2, 0xb2, 0x00,
  0x9f, 0x75, 0x75, 0xea, 0x9f, 0x75, 0x75, 0x00,
  0x1b, 0x09, 0x09, 0x12, 0x1b, 0x09, 0x09, 0x00,
  0x9e, 0x83, 0x83, 0x1d, 0x9e, 0x83, 0x83, 0x00,
  0x74, 0x2c, 0x2c, 0x58, 0x74, 0x2c, 0x2c, 0x00,
  0x2e, 0x1a, 0x1a, 0x34, 0x2e, 0x1a, 0x1a, 0x00,
  0x2d, 0x1b, 0x1b, 0x36, 0x2d, 0x1b, 0x1b, 0x00,
  0xb2, 0x6e, 0x6e, 0xdc, 0xb2, 0x6e, 0x6e, 0x00,
  0xee, 0x5a, 0x5a, 0xb4, 0xee, 0x5a, 0x5a, 0x00,
  0xfb, 0xa0, 0xa0, 0x5b, 0xfb, 0xa0, 0xa0, 0x00,
  0xf6, 0x52, 0x52, 0xa4, 0xf6, 0x52, 0x52, 0x00,
  0x4d, 0x3b, 0x3b, 0x76, 0x4d, 0x3b, 0x3b, 0x00,
  0x61, 0xd6, 0xd6, 0xb7, 0x61, 0xd6, 0xd6, 0x00,
  0xce, 0xb3, 0xb3, 0x7d, 0xce, 0xb3, 0xb3, 0x00,
  0x7b, 0x29, 0x29, 0x52, 0x7b, 0x29, 0x29, 0x00,
  0x3e, 0xe3, 0xe3, 0xdd, 0x3e, 0xe3, 0xe3, 0x00,
  0x71, 0x2f, 0x2f, 0x5e, 0x71, 0x2f, 0x2f, 0x00,
  0x97, 0x84, 0x84, 0x13, 0x97, 0x84, 0x84, 0x00,
  0xf5, 0x53, 0x53, 0xa6, 0xf5, 0x53, 0x53, 0x00,
  0x68, 0xd1, 0xd1, 0xb9, 0x68, 0xd1, 0xd1, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x2c, 0xed, 0xed, 0xc1, 0x2c, 0xed, 0xed, 0x00,
  0x60, 0x20, 0x20, 0x40, 0x60, 0x20, 0x20, 0x00,
  0x1f, 0xfc, 0xfc, 0xe3, 0x1f, 0xfc, 0xfc, 0x00,
  0xc8, 0xb1, 0xb1, 0x79, 0xc8, 0xb1, 0xb1, 0x00,
  0xed, 0x5b, 0x5b, 0xb6, 0xed, 0x5b, 0x5b, 0x00,
  0xbe, 0x6a, 0x6a, 0xd4, 0xbe, 0x6a, 0x6a, 0x00,
  0x46, 0xcb, 0xcb, 0x8d, 0x46, 0xcb, 0xcb, 0x00,
  0xd9, 0xbe, 0xbe, 0x67, 0xd9, 0xbe, 0xbe, 0x00,
  0x4b, 0x39, 0x39, 0x72, 0x4b, 0x39, 0x39, 0x00,
  0xde, 0x4a, 0x4a, 0x94, 0xde, 0x4a, 0x4a, 0x00,
  0xd4, 0x4c, 0x4c, 0x98, 0xd4, 0x4c, 0x4c, 0x00,
  0xe8, 0x58, 0x58, 0xb0, 0xe8, 0x58, 0x58, 0x00,
  0x4a, 0xcf, 0xcf, 0x85, 0x4a, 0xcf, 0xcf, 0x00,
  0x6b, 0xd0, 0xd0, 0xbb, 0x6b, 0xd0, 0xd0, 0x00,
  0x2a, 0xef, 0xef, 0xc5, 0x2a, 0xef, 0xef, 0x00,
  0xe5, 0xaa, 0xaa, 0x4f, 0xe5, 0xaa, 0xaa, 0x00,
  0x16, 0xfb, 0xfb, 0xed, 0x16, 0xfb, 0xfb, 0x00,
  0xc5, 0x43, 0x43, 0x86, 0xc5, 0x43, 0x43, 0x00,
  0xd7, 0x4d, 0x4d, 0x9a, 0xd7, 0x4d, 0x4d, 0x00,
  0x55, 0x33, 0x33, 0x66, 0x55, 0x33, 0x33, 0x00,
  0x94, 0x85, 0x85, 0x11, 0x94, 0x85, 0x85, 0x00,
  0xcf, 0x45, 0x45, 0x8a, 0xcf, 0x45, 0x45, 0x00,
  0x10, 0xf9, 0xf9, 0xe9, 0x10, 0xf9, 0xf9, 0x00,
  0x06, 0x02, 0x02, 0x04, 0x06, 0x02, 0x02, 0x00,
  0x81, 0x7f, 0x7f, 0xfe, 0x81, 0x7f, 0x7f, 0x00,
  0xf0, 0x50, 0x50, 0xa0, 0xf0, 0x50, 0x50, 0x00,
  0x44, 0x3c, 0x3c, 0x78, 0x44, 0x3c, 0x3c, 0x00,
  0xba, 0x9f, 0x9f, 0x25, 0xba, 0x9f, 0x9f, 0x00,
  0xe3, 0xa8, 0xa8, 0x4b, 0xe3, 0xa8, 0xa8, 0x00,
  0xf3, 0x51, 0x51, 0xa2, 0xf3, 0x51, 0x51, 0x00,
  0xfe, 0xa3, 0xa3, 0x5d, 0xfe, 0xa3, 0xa3, 0x00,
  0xc0, 0x40, 0x40, 0x80, 0xc0, 0x40, 0x40, 0x00,
  0x8a, 0x8f, 0x8f, 0x05, 0x8a, 0x8f, 0x8f, 0x00,
  0xad, 0x92, 0x92, 0x3f, 0xad, 0x92, 0x92, 0x00,
  0xbc, 0x9d, 0x9d, 0x21, 0xbc, 0x9d, 0x9d, 0x00,
  0x48, 0x38, 0x38, 0x70, 0x48, 0x38, 0x38, 0x00,
  0x04, 0xf5, 0xf5, 0xf1, 0x04, 0xf5, 0xf5, 0x00,
  0xdf, 0xbc, 0xbc, 0x63, 0xdf, 0xbc, 0xbc, 0x00,
  0xc1, 0xb6, 0xb6, 0x77, 0xc1, 0xb6, 0xb6, 0x00,
  0x75, 0xda, 0xda, 0xaf, 0x75, 0xda, 0xda, 0x00,
  0x63, 0x21, 0x21, 0x42, 0x63, 0x21, 0x21, 0x00,
  0x30, 0x10, 0x10, 0x20, 0x30, 0x10, 0x10, 0x00,
  0x1a, 0xff, 0xff, 0xe5, 0x1a, 0xff, 0xff, 0x00,
  0x0e, 0xf3, 0xf3, 0xfd, 0x0e, 0xf3, 0xf3, 0x00,
  0x6d, 0xd2, 0xd2, 0xbf, 0x6d, 0xd2, 0xd2, 0x00,
  0x4c, 0xcd, 0xcd, 0x81, 0x4c, 0xcd, 0xcd, 0x00,
  0x14, 0x0c, 0x0c, 0x18, 0x14, 0x0c, 0x0c, 0x00,
  0x35, 0x13, 0x13, 0x26, 0x35, 0x13, 0x13, 0x00,
  0x2f, 0xec, 0xec, 0xc3, 0x2f, 0xec, 0xec, 0x00,
  0xe1, 0x5f, 0x5f, 0xbe, 0xe1, 0x5f, 0x5f, 0x00,
  0xa2, 0x97, 0x97, 0x35, 0xa2, 0x97, 0x97, 0x00,
  0xcc, 0x44, 0x44, 0x88, 0xcc, 0x44, 0x44, 0x00,
  0x39, 0x17, 0x17, 0x2e, 0x39, 0x17, 0x17, 0x00,
  0x57, 0xc4, 0xc4, 0x93, 0x57, 0xc4, 0xc4, 0x00,
  0xf2, 0xa7, 0xa7, 0x55, 0xf2, 0xa7, 0xa7, 0x00,
  0x82, 0x7e, 0x7e, 0xfc, 0x82, 0x7e, 0x7e, 0x00,
  0x47, 0x3d, 0x3d, 0x7a, 0x47, 0x3d, 0x3d, 0x00,
  0xac, 0x64, 0x64, 0xc8, 0xac, 0x64, 0x64, 0x00,
  0xe7, 0x5d, 0x5d, 0xba, 0xe7, 0x5d, 0x5d, 0x00,
  0x2b, 0x19, 0x19, 0x32, 0x2b, 0x19, 0x19, 0x00,
  0x95, 0x73, 0x73, 0xe6, 0x95, 0x73, 0x73, 0x00,
  0xa0, 0x60, 0x60, 0xc0, 0xa0, 0x60, 0x60, 0x00,
  0x98, 0x81, 0x81, 0x19, 0x98, 0x81, 0x81, 0x00,
  0xd1, 0x4f, 0x4f, 0x9e, 0xd1, 0x4f, 0x4f, 0x00,
  0x7f, 0xdc, 0xdc, 0xa3, 0x7f, 0xdc, 0xdc, 0x00,
  0x66, 0x22, 0x22, 0x44, 0x66, 0x22, 0x22, 0x00,
  0x7e, 0x2a, 0x2a, 0x54, 0x7e, 0x2a, 0x2a, 0x00,
  0xab, 0x90, 0x90, 0x3b, 0xab, 0x90, 0x90, 0x00,
  0x83, 0x88, 0x88, 0x0b, 0x83, 0x88, 0x88, 0x00,
  0xca, 0x46, 0x46, 0x8c, 0xca, 0x46, 0x46, 0x00,
  0x29, 0xee, 0xee, 0xc7, 0x29, 0xee, 0xee, 0x00,
  0xd3, 0xb8, 0xb8, 0x6b, 0xd3, 0xb8, 0xb8, 0x00,
  0x3c, 0x14, 0x14, 0x28, 0x3c, 0x14, 0x14, 0x00,
  0x79, 0xde, 0xde, 0xa7, 0x79, 0xde, 0xde, 0x00,
  0xe2, 0x5e, 0x5e, 0xbc, 0xe2, 0x5e, 0x5e, 0x00,
  0x1d, 0x0b, 0x0b, 0x16, 0x1d, 0x0b, 0x0b, 0x00,
  0x76, 0xdb, 0xdb, 0xad, 0x76, 0xdb, 0xdb, 0x00,
  0x3b, 0xe0, 0xe0, 0xdb, 0x3b, 0xe0, 0xe0, 0x00,
  0x56, 0x32, 0x32, 0x64, 0x56, 0x32, 0x32, 0x00,
  0x4e, 0x3a, 0x3a, 0x74, 0x4e, 0x3a, 0x3a, 0x00,
  0x1e, 0x0a, 0x0a, 0x14, 0x1e, 0x0a, 0x0a, 0x00,
  0xdb, 0x49, 0x49, 0x92, 0xdb, 0x49, 0x49, 0x00,
  0x0a, 0x06, 0x06, 0x0c, 0x0a, 0x06, 0x06, 0x00,
  0x6c, 0x24, 0x24, 0x48, 0x6c, 0x24, 0x24, 0x00,
  0xe4, 0x5c, 0x5c, 0xb8, 0xe4, 0x5c, 0x5c, 0x00,
  0x5d, 0xc2, 0xc2, 0x9f, 0x5d, 0xc2, 0xc2, 0x00,
  0x6e, 0xd3, 0xd3, 0xbd, 0x6e, 0xd3, 0xd3, 0x00,
  0xef, 0xac, 0xac, 0x43, 0xef, 0xac, 0xac, 0x00,
  0xa6, 0x62, 0x62, 0xc4, 0xa6, 0x62, 0x62, 0x00,
  0xa8, 0x91, 0x91, 0x39, 0xa8, 0x91, 0x91, 0x00,
  0xa4, 0x95, 0x95, 0x31, 0xa4, 0x95, 0x95, 0x00,
  0x37, 0xe4, 0xe4, 0xd3, 0x37, 0xe4, 0xe4, 0x00,
  0x8b, 0x79, 0x79, 0xf2, 0x8b, 0x79, 0x79, 0x00,
  0x32, 0xe7, 0xe7, 0xd5, 0x32, 0xe7, 0xe7, 0x00,
  0x43, 0xc8, 0xc8, 0x8b, 0x43, 0xc8, 0xc8, 0x00,
  0x59, 0x37, 0x37, 0x6e, 0x59, 0x37, 0x37, 0x00,
  0xb7, 0x6d, 0x6d, 0xda, 0xb7, 0x6d, 0x6d, 0x00,
  0x8c, 0x8d, 0x8d, 0x01, 0x8c, 0x8d, 0x8d, 0x00,
  0x64, 0xd5, 0xd5, 0xb1, 0x64, 0xd5, 0xd5, 0x00,
  0xd2, 0x4e, 0x4e, 0x9c, 0xd2, 0x4e, 0x4e, 0x00,
  0xe0, 0xa9, 0xa9, 0x49, 0xe0, 0xa9, 0xa9, 0x00,
  0xb4, 0x6c, 0x6c, 0xd8, 0xb4, 0x6c, 0x6c, 0x00,
  0xfa, 0x56, 0x56, 0xac, 0xfa, 0x56, 0x56, 0x00,
  0x07, 0xf4, 0xf4, 0xf3, 0x07, 0xf4, 0xf4, 0x00,
  0x25, 0xea, 0xea, 0xcf, 0x25, 0xea, 0xea, 0x00,
  0xaf, 0x65, 0x65, 0xca, 0xaf, 0x65, 0x65, 0x00,
  0x8e, 0x7a, 0x7a, 0xf4, 0x8e, 0x7a, 0x7a, 0x00,
  0xe9, 0xae, 0xae, 0x47, 0xe9, 0xae, 0xae, 0x00,
  0x18, 0x08, 0x08, 0x10, 0x18, 0x08, 0x08, 0x00,
  0xd5, 0xba, 0xba, 0x6f, 0xd5, 0xba, 0xba, 0x00,
  0x88, 0x78, 0x78, 0xf0, 0x88, 0x78, 0x78, 0x00,
  0x6f, 0x25, 0x25, 0x4a, 0x6f, 0x25, 0x25, 0x00,
  0x72, 0x2e, 0x2e, 0x5c, 0x72, 0x2e, 0x2e, 0x00,
  0x24, 0x1c, 0x1c, 0x38, 0x24, 0x1c, 0x1c, 0x00,
  0xf1, 0xa6, 0xa6, 0x57, 0xf1, 0xa6, 0xa6, 0x00,
  0xc7, 0xb4, 0xb4, 0x73, 0xc7, 0xb4, 0xb4, 0x00,
  0x51, 0xc6, 0xc6, 0x97, 0x51, 0xc6, 0xc6, 0x00,
  0x23, 0xe8, 0xe8, 0xcb, 0x23, 0xe8, 0xe8, 0x00,
  0x7c, 0xdd, 0xdd, 0xa1, 0x7c, 0xdd, 0xdd, 0x00,
  0x9c, 0x74, 0x74, 0xe8, 0x9c, 0x74, 0x74, 0x00,
  0x21, 0x1f, 0x1f, 0x3e, 0x21, 0x1f, 0x1f, 0x00,
  0xdd, 0x4b, 0x4b, 0x96, 0xdd, 0x4b, 0x4b, 0x00,
  0xdc, 0xbd, 0xbd, 0x61, 0xdc, 0xbd, 0xbd, 0x00,
  0x86, 0x8b, 0x8b, 0x0d, 0x86, 0x8b, 0x8b, 0x00,
  0x85, 0x8a, 0x8a, 0x0f, 0x85, 0x8a, 0x8a, 0x00,
  0x90, 0x70, 0x70, 0xe0, 0x90, 0x70, 0x70, 0x00,
  0x42, 0x3e, 0x3e, 0x7c, 0x42, 0x3e, 0x3e, 0x00,
  0xc4, 0xb5, 0xb5, 0x71, 0xc4, 0xb5, 0xb5, 0x00,
  0xaa, 0x66, 0x66, 0xcc, 0xaa, 0x66, 0x66, 0x00,
  0xd8, 0x48, 0x48, 0x90, 0xd8, 0x48, 0x48, 0x00,
  0x05, 0x03, 0x03, 0x06, 0x05, 0x03, 0x03, 0x00,
  0x01, 0xf6, 0xf6, 0xf7, 0x01, 0xf6, 0xf6, 0x00,
  0x12, 0x0e, 0x0e, 0x1c, 0x12, 0x0e, 0x0e, 0x00,
  0xa3, 0x61, 0x61, 0xc2, 0xa3, 0x61, 0x61, 0x00,
  0x5f, 0x35, 0x35, 0x6a, 0x5f, 0x35, 0x35, 0x00,
  0xf9, 0x57, 0x57, 0xae, 0xf9, 0x57, 0x57, 0x00,
  0xd0, 0xb9, 0xb9, 0x69, 0xd0, 0xb9, 0xb9, 0x00,
  0x91, 0x86, 0x86, 0x17, 0x91, 0x86, 0x86, 0x00,
  0x58, 0xc1, 0xc1, 0x99, 0x58, 0xc1, 0xc1, 0x00,
  0x27, 0x1d, 0x1d, 0x3a, 0x27, 0x1d, 0x1d, 0x00,
  0xb9, 0x9e, 0x9e, 0x27, 0xb9, 0x9e, 0x9e, 0x00,
  0x38, 0xe1, 0xe1, 0xd9, 0x38, 0xe1, 0xe1, 0x00,
  0x13, 0xf8, 0xf8, 0xeb, 0x13, 0xf8, 0xf8, 0x00,
  0xb3, 0x98, 0x98, 0x2b, 0xb3, 0x98, 0x98, 0x00,
  0x33, 0x11, 0x11, 0x22, 0x33, 0x11, 0x11, 0x00,
  0xbb, 0x69, 0x69, 0xd2, 0xbb, 0x69, 0x69, 0x00,
  0x70, 0xd9, 0xd9, 0xa9, 0x70, 0xd9, 0xd9, 0x00,
  0x89, 0x8e, 0x8e, 0x07, 0x89, 0x8e, 0x8e, 0x00,
  0xa7, 0x94, 0x94, 0x33, 0xa7, 0x94, 0x94, 0x00,
  0xb6, 0x9b, 0x9b, 0x2d, 0xb6, 0x9b, 0x9b, 0x00,
  0x22, 0x1e, 0x1e, 0x3c, 0x22, 0x1e, 0x1e, 0x00,
  0x92, 0x87, 0x87, 0x15, 0x92, 0x87, 0x87, 0x00,
  0x20, 0xe9, 0xe9, 0xc9, 0x20, 0xe9, 0xe9, 0x00,
  0x49, 0xce, 0xce, 0x87, 0x49, 0xce, 0xce, 0x00,
  0xff, 0x55, 0x55, 0xaa, 0xff, 0x55, 0x55, 0x00,
  0x78, 0x28, 0x28, 0x50, 0x78, 0x28, 0x28, 0x00,
  0x7a, 0xdf, 0xdf, 0xa5, 0x7a, 0xdf, 0xdf, 0x00,
  0x8f, 0x8c, 0x8c, 0x03, 0x8f, 0x8c, 0x8c, 0x00,
  0xf8, 0xa1, 0xa1, 0x59, 0xf8, 0xa1, 0xa1, 0x00,
  0x80, 0x89, 0x89, 0x09, 0x80, 0x89, 0x89, 0x00,
  0x17, 0x0d, 0x0d, 0x1a, 0x17, 0x0d, 0x0d, 0x00,
  0xda, 0xbf, 0xbf, 0x65, 0xda, 0xbf, 0xbf, 0x00,
  0x31, 0xe6, 0xe6, 0xd7, 0x31, 0xe6, 0xe6, 0x00,
  0xc6, 0x42, 0x42, 0x84, 0xc6, 0x42, 0x42, 0x00,
  0xb8, 0x68, 0x68, 0xd0, 0xb8, 0x68, 0x68, 0x00,
  0xc3, 0x41, 0x41, 0x82, 0xc3, 0x41, 0x41, 0x00,
  0xb0, 0x99, 0x99, 0x29, 0xb0, 0x99, 0x99, 0x00,
  0x77, 0x2d, 0x2d, 0x5a, 0x77, 0x2d, 0x2d, 0x00,
  0x11, 0x0f, 0x0f, 0x1e, 0x11, 0x0f, 0x0f, 0x00,
  0xcb, 0xb0, 0xb0, 0x7b, 0xcb, 0xb0, 0xb0, 0x00,
  0xfc, 0x54, 0x54, 0xa8, 0xfc, 0x54, 0x54, 0x00,
  0xd6, 0xbb, 0xbb, 0x6d, 0xd6, 0xbb, 0xbb, 0x00,
  0x3a, 0x16, 0x16, 0x2c, 0x3a, 0x16, 0x16, 0x00
};

#define enc_T0(b) (((const uint32 *)(enc_T    ))[(b) << 1])

#define enc_T1(b) (((const uint32 *)(enc_T + 1))[(b) << 1])

#define enc_T2(b) (((const uint32 *)(enc_T + 2))[(b) << 1])

#define enc_T3(b) (((const uint32 *)(enc_T + 3))[(b) << 1])

#define S(b) (((const uint8 *)(enc_T + 1))[(b) << 3])

static uint32 Rcon[] = {
  0x00000000,                   /* unused */
  0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
  0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000
};

void aes128_init(aes128_ctx *ctx, const byte *key)
{
  int i;
  uint32 *w;

  w = ctx->w;
  bindata_unpack(key, "> L[4]", w);

  for (i = 4; i < 44; i += 1) {
    uint32 tmp = w[i-1];
    if ((i % 4) == 0) {
      tmp = ((S((tmp >> 16) & 0xff) << 24) ^
             (S((tmp >>  8) & 0xff) << 16) ^
             (S((tmp      ) & 0xff) <<  8) ^
             (S((tmp >> 24) & 0xff)      ) ^
             Rcon[i/4]);
    }
    w[i] = w[i-4] ^ tmp;
  }
}

static uint32 enc_roundstep(uint32 a, uint32 b, uint32 c, uint32 d, uint32 k)
{
  return (enc_T0((a >> 24) & 0xff) ^
          enc_T1((b >> 16) & 0xff) ^
          enc_T2((c >>  8) & 0xff) ^
          enc_T3((d      ) & 0xff) ^
          k);
}

#define ENC_ROUND(e, a) do {                            \
    e[0] = enc_roundstep(a[0], a[1], a[2], a[3], *w++); \
    e[1] = enc_roundstep(a[1], a[2], a[3], a[0], *w++); \
    e[2] = enc_roundstep(a[2], a[3], a[0], a[1], *w++); \
    e[3] = enc_roundstep(a[3], a[0], a[1], a[2], *w++); \
  } while (0)

static uint32 enc_finalroundstep(uint32 a, uint32 b, uint32 c, uint32 d,
                                 uint32 k)
{
  return ((enc_T2((a >> 24) & 0xff) & 0xff000000) ^
          (enc_T3((b >> 16) & 0xff) & 0x00ff0000) ^
          (enc_T0((c >>  8) & 0xff) & 0x0000ff00) ^
          (enc_T1((d      ) & 0xff) & 0x000000ff) ^
          k);
}

void aes128_encrypt(const aes128_ctx *ctx, const byte *p, byte *c)
{
  const uint32 *w;
  uint32 m[4];
  uint32 n[4];

  w = ctx->w;
  bindata_unpack(p, "> L[4]", m);

  m[0] ^= *w++;
  m[1] ^= *w++;
  m[2] ^= *w++;
  m[3] ^= *w++;

  ENC_ROUND(n, m);              /* 1 */
  ENC_ROUND(m, n);              /* 2 */
  ENC_ROUND(n, m);              /* 3 */
  ENC_ROUND(m, n);              /* 4 */
  ENC_ROUND(n, m);              /* 5 */
  ENC_ROUND(m, n);              /* 6 */
  ENC_ROUND(n, m);              /* 7 */
  ENC_ROUND(m, n);              /* 8 */
  ENC_ROUND(n, m);              /* 9 */

  /* 10 */
  m[0] = enc_finalroundstep(n[0], n[1], n[2], n[3], *w++);
  m[1] = enc_finalroundstep(n[1], n[2], n[3], n[0], *w++);
  m[2] = enc_finalroundstep(n[2], n[3], n[0], n[1], *w++);
  m[3] = enc_finalroundstep(n[3], n[0], n[1], n[2], *w++);

  bindata_pack(c, "> L[4]", m);
}

/* TODO doesn't work currently */
void aes128_decrypt(const aes128_ctx *ctx, const byte *c, byte *p)
{
  const uint32 *w;
  uint32 m[4];

  w = ctx->w;
  bindata_unpack(c, "> L[4]", m);

  bindata_pack(p, "> L[4]", m);
}

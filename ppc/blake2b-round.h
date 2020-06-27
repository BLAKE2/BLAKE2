/*
   BLAKE2 reference source code package - optimized C implementations

   Copyright 2012, Samuel Neves <sneves@dei.uc.pt>.  You may use this under the
   terms of the CC0, the OpenSSL Licence, or the Apache Public License 2.0, at
   your option.  The terms of these licenses can be found at:

   - CC0 1.0 Universal : http://creativecommons.org/publicdomain/zero/1.0
   - OpenSSL license   : https://www.openssl.org/source/license.html
   - Apache 2.0        : http://www.apache.org/licenses/LICENSE-2.0

   More information about the BLAKE2 hash function can be found at
   https://blake2.net.
*/
#ifndef ROUND_H
#define ROUND_H

#define LIKELY(x) __builtin_expect((x),1)

#define vec_ror_16(x) vec_rl(x, mask_ror_16)
#define vec_ror_24(x) vec_rl(x, mask_ror_24)
#define vec_ror_32(x) vec_rl(x, mask_ror_32)
#define vec_ror_63(x) vec_rl(x, mask_ror_63)


#define G1(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h,b0,b1) \
do { \
  row1l = vec_add(vec_add(row1l, b0), row2l); \
  row1h = vec_add(vec_add(row1h, b1), row2h); \
  row4l = vec_xor(row4l, row1l); \
  row4h = vec_xor(row4h, row1h); \
  row4l = vec_ror_32(row4l); \
  row4h = vec_ror_32(row4h); \
  row3l = vec_add(row3l, row4l); \
  row3h = vec_add(row3h, row4h); \
  row2l = vec_xor(row2l, row3l); \
  row2h = vec_xor(row2h, row3h); \
  row2l = vec_ror_24(row2l); \
  row2h = vec_ror_24(row2h); \
} while(0)

#define G2(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h,b0,b1) \
do { \
  row1l = vec_add(vec_add(row1l, b0), row2l); \
  row1h = vec_add(vec_add(row1h, b1), row2h); \
  row4l = vec_xor(row4l, row1l); \
  row4h = vec_xor(row4h, row1h); \
  row4l = vec_ror_16(row4l); \
  row4h = vec_ror_16(row4h); \
  row3l = vec_add(row3l, row4l); \
  row3h = vec_add(row3h, row4h); \
  row2l = vec_xor(row2l, row3l); \
  row2h = vec_xor(row2h, row3h); \
  row2l = vec_ror_63(row2l); \
  row2h = vec_ror_63(row2h); \
} while(0)

#define DIAGONALIZE(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h) \
do { \
  uint64x2_p t0, t1; \
  t0 = vec_shl_8(row2l, row2h); \
  t1 = vec_shl_8(row2h, row2l); \
  row2l = t0; \
  row2h = t1; \
  t0 = row3l; \
  row3l = row3h; \
  row3h = t0; \
  t0 = vec_shl_8(row4h, row4l); \
  t1 = vec_shl_8(row4l, row4h); \
  row4l = t0; \
  row4h = t1; \
} while(0)

#define UNDIAGONALIZE(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h) \
do { \
  uint64x2_p t0, t1; \
  t0 = vec_shl_8(row2h, row2l); \
  t1 = vec_shl_8(row2l, row2h); \
  row2l = t0; \
  row2h = t1; \
  t0 = row3l; \
  row3l = row3h; row3h = t0; \
  t0 = vec_shl_8(row4l, row4h); \
  t1 = vec_shl_8(row4h, row4l); \
  row4l = t0; \
  row4h = t1; \
} while(0)

#if defined(__VSX__) || defined(_ARCH_PWR8)
#include "blake2b-load-pwr8.h"
#endif

#define ROUND(r) \
do { \
  uint64x2_p b0, b1; \
  LOAD_MSG_ ##r ##_1(b0, b1); \
  G1(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h,b0,b1); \
  LOAD_MSG_ ##r ##_2(b0, b1); \
  G2(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h,b0,b1); \
  DIAGONALIZE(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h); \
  LOAD_MSG_ ##r ##_3(b0, b1); \
  G1(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h,b0,b1); \
  LOAD_MSG_ ##r ##_4(b0, b1); \
  G2(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h,b0,b1); \
  UNDIAGONALIZE(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h); \
} while(0)

#endif

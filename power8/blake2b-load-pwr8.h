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
#ifndef BLAKE2B_LOAD_SSE2_H
#define BLAKE2B_LOAD_SSE2_H

#define vec_merge_hi(a, b) vec_mergeh(a,b)
#define vec_merge_hi_lo(a, b) vec_mergeh(a,(uint64x2_p)vec_sld((uint8x16_p)b,(uint8x16_p)b,8))
#define vec_merge_lo(a, b) vec_mergel(a,b)

#if defined(NATIVE_BIG_ENDIAN)
# define vec_shl_8(a,b) (uint64x2_p)vec_sld((uint8x16_p)a, (uint8x16_p)b, 8);
#else
# define vec_shl_8(a,b) (uint64x2_p)vec_sld((uint8x16_p)b, (uint8x16_p)a, 16-8);
#endif

#define LOAD_MSG_0_1(b0, b1) \
do { \
	 b0 = vec_merge_hi(m0, m1); \
	 b1 = vec_merge_hi(m2, m3); \
} while(0)

#define LOAD_MSG_0_2(b0, b1) \
do { \
	 b0 = vec_merge_lo(m0, m1); \
	 b1 = vec_merge_lo(m2, m3); \
} while(0)

#define LOAD_MSG_0_3(b0, b1) \
do { \
	 b0 = vec_merge_hi(m4, m5); \
	 b1 = vec_merge_hi(m6, m7); \
} while(0)

#define LOAD_MSG_0_4(b0, b1) \
do { \
	 b0 = vec_merge_lo(m4, m5); \
	 b1 = vec_merge_lo(m6, m7); \
} while(0)

#define LOAD_MSG_1_1(b0, b1) \
do { \
	 b0 = vec_merge_hi(m7, m2); \
	 b1 = vec_merge_lo(m4, m6); \
} while(0)

#define LOAD_MSG_1_2(b0, b1) \
do { \
	 b0 = vec_merge_hi(m5, m4); \
	 b1 = vec_shl_8(m7, m3); \
} while(0)

#define LOAD_MSG_1_3(b0, b1) \
do { \
	 b0 = vec_shl_8(m0, m0); \
	 b1 = vec_merge_lo(m5, m2); \
} while(0)

#define LOAD_MSG_1_4(b0, b1) \
do { \
	 b0 = vec_merge_hi(m6, m1); \
	 b1 = vec_merge_lo(m3, m1); \
} while(0)

#define LOAD_MSG_2_1(b0, b1) \
do { \
	 b0 = vec_shl_8(m5, m6); \
	 b1 = vec_merge_lo(m2, m7); \
} while(0)

#define LOAD_MSG_2_2(b0, b1) \
do { \
	 b0 = vec_merge_hi(m4, m0); \
	 b1 = vec_merge_hi_lo(m1, m6); \
} while(0)

#define LOAD_MSG_2_3(b0, b1) \
   do { \
	 b0 = vec_merge_hi_lo(m5, m1); \
	 b1 = vec_merge_lo(m3, m4); \
} while(0)

#define LOAD_MSG_2_4(b0, b1) \
   do { \
	 b0 = vec_merge_hi(m7, m3); \
	 b1 = vec_shl_8(m0, m2); \
} while(0)

#define LOAD_MSG_3_1(b0, b1) \
   do { \
	 b0 = vec_merge_lo(m3, m1); \
	 b1 = vec_merge_lo(m6, m5); \
} while(0)

#define LOAD_MSG_3_2(b0, b1) \
   do { \
	 b0 = vec_merge_lo(m4, m0); \
	 b1 = vec_merge_hi(m6, m7); \
} while(0)

#define LOAD_MSG_3_3(b0, b1) \
   do { \
	 b0 = vec_merge_hi_lo(m1, m2); \
	 b1 = vec_merge_hi_lo(m2, m7); \
} while(0)

#define LOAD_MSG_3_4(b0, b1) \
   do { \
	 b0 = vec_merge_hi(m3, m5); \
	 b1 = vec_merge_hi(m0, m4); \
} while(0)

#define LOAD_MSG_4_1(b0, b1) \
   do { \
	 b0 = vec_merge_lo(m4, m2); \
	 b1 = vec_merge_hi(m1, m5); \
} while(0)

#define LOAD_MSG_4_2(b0, b1) \
   do { \
	 b0 = vec_merge_hi_lo(m0, m3); \
	 b1 = vec_merge_hi_lo(m2, m7); \
} while(0)

#define LOAD_MSG_4_3(b0, b1) \
   do { \
	 b0 = vec_merge_hi_lo(m7, m5); \
	 b1 = vec_merge_hi_lo(m3, m1); \
} while(0)

#define LOAD_MSG_4_4(b0, b1) \
   do { \
	 b0 = vec_shl_8(m0, m6); \
	 b1 = vec_merge_hi_lo(m4, m6); \
} while(0)

#define LOAD_MSG_5_1(b0, b1) \
   do { \
	 b0 = vec_merge_hi(m1, m3); \
	 b1 = vec_merge_hi(m0, m4); \
} while(0)

#define LOAD_MSG_5_2(b0, b1) \
   do { \
	 b0 = vec_merge_hi(m6, m5); \
	 b1 = vec_merge_lo(m5, m1); \
} while(0)

#define LOAD_MSG_5_3(b0, b1) \
   do { \
	 b0 = vec_merge_hi_lo(m2, m3); \
	 b1 = vec_merge_lo(m7, m0); \
} while(0)

#define LOAD_MSG_5_4(b0, b1) \
   do { \
	 b0 = vec_merge_lo(m6, m2); \
	 b1 = vec_merge_hi_lo(m7, m4); \
} while(0)

#define LOAD_MSG_6_1(b0, b1) \
   do { \
	 b0 = vec_merge_hi_lo(m6, m0); \
	 b1 = vec_merge_hi(m7, m2); \
} while(0)

#define LOAD_MSG_6_2(b0, b1) \
   do { \
	 b0 = vec_merge_lo(m2, m7); \
	 b1 = vec_shl_8(m6, m5); \
} while(0)

#define LOAD_MSG_6_3(b0, b1) \
   do { \
	 b0 = vec_merge_hi(m0, m3); \
	 b1 = vec_shl_8(m4, m4); \
} while(0)

#define LOAD_MSG_6_4(b0, b1) \
   do { \
	 b0 = vec_merge_lo(m3, m1); \
	 b1 = vec_merge_hi_lo(m1, m5); \
} while(0)

#define LOAD_MSG_7_1(b0, b1) \
   do { \
	 b0 = vec_merge_lo(m6, m3); \
	 b1 = vec_merge_hi_lo(m6, m1); \
} while(0)

#define LOAD_MSG_7_2(b0, b1) \
   do { \
	 b0 = vec_shl_8(m5, m7); \
	 b1 = vec_merge_lo(m0, m4); \
} while(0)

#define LOAD_MSG_7_3(b0, b1) \
   do { \
	 b0 = vec_merge_lo(m2, m7); \
	 b1 = vec_merge_hi(m4, m1); \
} while(0)

#define LOAD_MSG_7_4(b0, b1) \
   do { \
	 b0 = vec_merge_hi(m0, m2); \
	 b1 = vec_merge_hi(m3, m5); \
} while(0)

#define LOAD_MSG_8_1(b0, b1) \
   do { \
	 b0 = vec_merge_hi(m3, m7); \
	 b1 = vec_shl_8(m5, m0); \
} while(0)

#define LOAD_MSG_8_2(b0, b1) \
   do { \
	 b0 = vec_merge_lo(m7, m4); \
	 b1 = vec_shl_8(m1, m4); \
} while(0)

#define LOAD_MSG_8_3(b0, b1) \
   do { \
	 b0 = m6; \
	 b1 = vec_shl_8(m0, m5); \
} while(0)

#define LOAD_MSG_8_4(b0, b1) \
   do { \
	 b0 = vec_merge_hi_lo(m1, m3); \
	 b1 = m2; \
} while(0)

#define LOAD_MSG_9_1(b0, b1) \
   do { \
	 b0 = vec_merge_hi(m5, m4); \
	 b1 = vec_merge_lo(m3, m0); \
} while(0)

#define LOAD_MSG_9_2(b0, b1) \
   do { \
	 b0 = vec_merge_hi(m1, m2); \
	 b1 = vec_merge_hi_lo(m3, m2); \
} while(0)

#define LOAD_MSG_9_3(b0, b1) \
   do { \
	 b0 = vec_merge_lo(m7, m4); \
	 b1 = vec_merge_lo(m1, m6); \
} while(0)

#define LOAD_MSG_9_4(b0, b1) \
   do { \
	 b0 = vec_shl_8(m5, m7); \
	 b1 = vec_merge_hi(m6, m0); \
} while(0)

#define LOAD_MSG_10_1(b0, b1) \
   do { \
	 b0 = vec_merge_hi(m0, m1); \
	 b1 = vec_merge_hi(m2, m3); \
} while(0)

#define LOAD_MSG_10_2(b0, b1) \
   do { \
	 b0 = vec_merge_lo(m0, m1); \
	 b1 = vec_merge_lo(m2, m3); \
} while(0)

#define LOAD_MSG_10_3(b0, b1) \
   do { \
	 b0 = vec_merge_hi(m4, m5); \
	 b1 = vec_merge_hi(m6, m7); \
} while(0)

#define LOAD_MSG_10_4(b0, b1) \
   do { \
	 b0 = vec_merge_lo(m4, m5); \
	 b1 = vec_merge_lo(m6, m7); \
} while(0)

#define LOAD_MSG_11_1(b0, b1) \
   do { \
	 b0 = vec_merge_hi(m7, m2); \
	 b1 = vec_merge_lo(m4, m6); \
} while(0)

#define LOAD_MSG_11_2(b0, b1) \
   do { \
	 b0 = vec_merge_hi(m5, m4); \
	 b1 = vec_shl_8(m7, m3); \
} while(0)

#define LOAD_MSG_11_3(b0, b1) \
   do { \
	 b0 = vec_shl_8(m0, m0); \
	 b1 = vec_merge_lo(m5, m2); \
} while(0)

#define LOAD_MSG_11_4(b0, b1) \
   do { \
	 b0 = vec_merge_hi(m6, m1); \
	 b1 = vec_merge_lo(m3, m1); \
} while(0)

#endif

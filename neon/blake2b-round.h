/*
   BLAKE2 reference source code package - reference C implementations

   Copyright 2012, Samuel Neves <sneves@dei.uc.pt>.  You may use this under the
   terms of the CC0, the OpenSSL Licence, or the Apache Public License 2.0, at
   your option.  The terms of these licenses can be found at:

   - CC0 1.0 Universal : http://creativecommons.org/publicdomain/zero/1.0
   - OpenSSL license   : https://www.openssl.org/source/license.html
   - Apache 2.0        : http://www.apache.org/licenses/LICENSE-2.0

   More information about the BLAKE2 hash function can be found at
   https://blake2.net.
*/
#ifndef BLAKE2B_ROUND_H
#define BLAKE2B_ROUND_H

#define vrorq_n_u64_32(x) vreinterpretq_u64_u32(vrev64q_u32(vreinterpretq_u32_u64((x))))

#define vrorq_n_u64_24(x) vcombine_u64( \
      vreinterpret_u64_u8(vext_u8(vreinterpret_u8_u64(vget_low_u64(x)), vreinterpret_u8_u64(vget_low_u64(x)), 3)), \
      vreinterpret_u64_u8(vext_u8(vreinterpret_u8_u64(vget_high_u64(x)), vreinterpret_u8_u64(vget_high_u64(x)), 3)))

#define vrorq_n_u64_16(x) vcombine_u64( \
      vreinterpret_u64_u8(vext_u8(vreinterpret_u8_u64(vget_low_u64(x)), vreinterpret_u8_u64(vget_low_u64(x)), 2)), \
      vreinterpret_u64_u8(vext_u8(vreinterpret_u8_u64(vget_high_u64(x)), vreinterpret_u8_u64(vget_high_u64(x)), 2)))

#define vrorq_n_u64_63(x) veorq_u64(vaddq_u64(x, x), vshrq_n_u64(x, 63))

/// \brief Three-way XOR
/// \param a the first value
/// \param b the second value
/// \param c the third value
/// \return three-way exclusive OR of the values
/// \details VEOR3() performs veor3q_u64(). VEOR3 is provided as GCC inline assembly due
///  to Clang and lack of support for the intrinsic.
/// \details VEOR3 requires ARMv8.4.
inline uint64x2_t VEOR3(uint64x2_t a, uint64x2_t b, uint64x2_t c)
{
#if defined(_MSC_VER)
# error "Not implemented"
#else
    uint64x2_t r;
    __asm__ ("eor3   %0.16b, %1.16b, %2.16b, %3.16b   \n\t"
            :"=w" (r) : "w" (a), "w" (b), "w" (c));
    return r;
#endif
}

/// \brief XOR and rotate
/// \param a the first value
/// \param b the second value
/// \param c the third value
/// \return two-way exclusive OR of the values, then rotated by imm6
/// \details VXARQ() performs vxarq_u64(). VXARQ is provided as GCC inline assembly due
///  to Clang and lack of support for the intrinsic.
/// \details VXARQ requires ARMv8.4.
inline uint64x2_t VXARQ(uint64x2_t a, uint64x2_t b, const int imm6)
{
#if defined(_MSC_VER)
# error "Not implemented"
#else
    uint64x2_t r;
    __asm__ ("xar   %0.2d, %1.2d, %2.2d, %3   \n\t"
            :"=w" (r) : "w" (a), "w" (b), "I" (imm6));
    return r;
#endif
}

/// \brief XOR and rotate
/// \tparam C the rotate amount
/// \param a the first value
/// \param b the second value
/// \return two-way exclusive OR of the values, then rotated by C
/// \details VXARQ() performs vxarq_u64(). VXARQ is provided as GCC inline assembly due
///  to Clang and lack of support for the intrinsic.
/// \details VXARQ requires ARMv8.4.
template <unsigned int C>
inline uint64x2_t VXARQ(uint64x2_t a, uint64x2_t b)
{
#if defined(_MSC_VER)
# error "Not implemented"
#else
    uint64x2_t r;
    __asm__ ("xar   %0.2d, %1.2d, %2.2d, %3   \n\t"
            :"=w" (r) : "w" (a), "w" (b), "I" (C));
    return r;
#endif
}

#if defined(__ARM_FEATURE_SHA3)
#define G1(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h,b0,b1) \
  row1l = vaddq_u64(vaddq_u64(row1l, b0), row2l); \
  row1h = vaddq_u64(vaddq_u64(row1h, b1), row2h); \
  row4l = VXARQ(row4l, row1l, 32); row4h = VXARQ(row4h, row1h, 32); \
  row3l = vaddq_u64(row3l, row4l); row3h = vaddq_u64(row3h, row4h); \
  row2l = VXARQ(row2l, row3l, 24); row2h = VXARQ(row2h, row3h, 24);

#define G2(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h,b0,b1) \
  row1l = vaddq_u64(vaddq_u64(row1l, b0), row2l); \
  row1h = vaddq_u64(vaddq_u64(row1h, b1), row2h); \
  row4l = VXARQ(row4l, row1l, 16); row4h = VXARQ(row4h, row1h, 16); \
  row3l = vaddq_u64(row3l, row4l); row3h = vaddq_u64(row3h, row4h); \
  row2l = VXARQ(row2l, row3l, 63); row2h = VXARQ(row2h, row3h, 63);

#else
/* No SHA3 support */
#define G1(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h,b0,b1) \
  row1l = vaddq_u64(vaddq_u64(row1l, b0), row2l); \
  row1h = vaddq_u64(vaddq_u64(row1h, b1), row2h); \
  row4l = VXARQ(row4l, row1l, 32); row4h = VXARQ(row4h, row1h, 32); \
  row3l = vaddq_u64(row3l, row4l); row3h = vaddq_u64(row3h, row4h); \
  row2l = veorq_u64(row2l, row3l); row2h = veorq_u64(row2h, row3h); \
  row2l = vrorq_n_u64_24(row2l); row2h = vrorq_n_u64_24(row2h);

#define G2(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h,b0,b1) \
  row1l = vaddq_u64(vaddq_u64(row1l, b0), row2l); \
  row1h = vaddq_u64(vaddq_u64(row1h, b1), row2h); \
  row4l = veorq_u64(row4l, row1l); row4h = veorq_u64(row4h, row1h); \
  row4l = vrorq_n_u64_16(row4l); row4h = vrorq_n_u64_16(row4h); \
  row3l = vaddq_u64(row3l, row4l); row3h = vaddq_u64(row3h, row4h); \
  row2l = veorq_u64(row2l, row3l); row2h = veorq_u64(row2h, row3h); \
  row2l = vrorq_n_u64_63(row2l); row2h = vrorq_n_u64_63(row2h);
#endif

#define DIAGONALIZE(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h) \
    t0 = vextq_u64(row2l, row2h, 1); \
    t1 = vextq_u64(row2h, row2l, 1); \
    row2l = t0; row2h = t1; t0 = row3l;  row3l = row3h; row3h = t0; \
    t0 = vextq_u64(row4h, row4l, 1); t1 = vextq_u64(row4l, row4h, 1); \
    row4l = t0; row4h = t1;

#define UNDIAGONALIZE(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h) \
    t0 = vextq_u64(row2h, row2l, 1); \
    t1 = vextq_u64(row2l, row2h, 1); \
    row2l = t0; row2h = t1; t0 = row3l; row3l = row3h; row3h = t0; \
    t0 = vextq_u64(row4l, row4h, 1); t1 = vextq_u64(row4h, row4l, 1); \
    row4l = t0; row4h = t1;

#include "blake2b-load-neon.h"

#define ROUND(r) \
  LOAD_MSG_ ##r ##_1(b0, b1); \
  G1(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h,b0,b1); \
  LOAD_MSG_ ##r ##_2(b0, b1); \
  G2(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h,b0,b1); \
  DIAGONALIZE(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h); \
  LOAD_MSG_ ##r ##_3(b0, b1); \
  G1(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h,b0,b1); \
  LOAD_MSG_ ##r ##_4(b0, b1); \
  G2(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h,b0,b1); \
  UNDIAGONALIZE(row1l,row2l,row3l,row4l,row1h,row2h,row3h,row4h);

#endif

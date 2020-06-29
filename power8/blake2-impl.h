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
#ifndef BLAKE2_IMPL_H
#define BLAKE2_IMPL_H

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "blake2-config.h"

#if !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
  #if defined(__GNUC__)
    #define BLAKE2_INLINE __inline__
  #else
    #define BLAKE2_INLINE
  #endif
#else
  #define BLAKE2_INLINE inline
#endif

#if defined(__ALTIVEC__)
# include <altivec.h>
# undef vector
# undef pixel
# undef bool
#endif

/* The PowerPC ABI says loads are non-const. Ugh... */
#ifndef CONST_V8_CAST
#define CONST_V8_CAST(x)   ((unsigned char*)(x))
#endif
#ifndef CONST_V32_CAST
#define CONST_V32_CAST(x)  ((unsigned int*)(x))
#endif
#ifndef NCONST_V8_CAST
#define NCONST_V8_CAST(x)   ((unsigned char*)(x))
#endif
#ifndef NCONST_V32_CAST
#define NCONST_V32_CAST(x)  ((unsigned int*)(x))
#endif

#ifndef BLAKE2_UNUSED
#define BLAKE2_UNUSED(x) ((void)(x))
#endif

#if defined(__ALTIVEC__)
typedef __vector unsigned char uint8x16_p;
typedef __vector unsigned int  uint32x4_p;
#if defined(__VSX__) || defined(_ARCH_PWR8)
typedef __vector unsigned long long uint64x2_p;
#endif
#endif

static BLAKE2_INLINE uint32_t load32( const void *src )
{
#if defined(NATIVE_LITTLE_ENDIAN)
  uint32_t w;
  memcpy(&w, src, sizeof w);
  return w;
#else
  const uint8_t *p = ( const uint8_t * )src;
  return (( uint32_t )( p[0] ) <<  0) |
         (( uint32_t )( p[1] ) <<  8) |
         (( uint32_t )( p[2] ) << 16) |
         (( uint32_t )( p[3] ) << 24) ;
#endif
}

static BLAKE2_INLINE uint64_t load64( const void *src )
{
#if defined(NATIVE_LITTLE_ENDIAN)
  uint64_t w;
  memcpy(&w, src, sizeof w);
  return w;
#else
  const uint8_t *p = ( const uint8_t * )src;
  return (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) |
         (( uint64_t )( p[6] ) << 48) |
         (( uint64_t )( p[7] ) << 56) ;
#endif
}

static BLAKE2_INLINE uint16_t load16( const void *src )
{
#if defined(NATIVE_LITTLE_ENDIAN)
  uint16_t w;
  memcpy(&w, src, sizeof w);
  return w;
#else
  const uint8_t *p = ( const uint8_t * )src;
  return ( uint16_t )((( uint32_t )( p[0] ) <<  0) |
                      (( uint32_t )( p[1] ) <<  8));
#endif
}

static BLAKE2_INLINE void store16( void *dst, uint16_t w )
{
#if defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, sizeof w);
#else
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
#endif
}

static BLAKE2_INLINE void store32( void *dst, uint32_t w )
{
#if defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, sizeof w);
#else
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
#endif
}

static BLAKE2_INLINE void store64( void *dst, uint64_t w )
{
#if defined(NATIVE_LITTLE_ENDIAN)
  memcpy(dst, &w, sizeof w);
#else
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
  p[6] = (uint8_t)(w >> 48);
  p[7] = (uint8_t)(w >> 56);
#endif
}

static BLAKE2_INLINE uint64_t load48( const void *src )
{
  const uint8_t *p = ( const uint8_t * )src;
  return (( uint64_t )( p[0] ) <<  0) |
         (( uint64_t )( p[1] ) <<  8) |
         (( uint64_t )( p[2] ) << 16) |
         (( uint64_t )( p[3] ) << 24) |
         (( uint64_t )( p[4] ) << 32) |
         (( uint64_t )( p[5] ) << 40) ;
}

static BLAKE2_INLINE void store48( void *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  p[0] = (uint8_t)(w >>  0);
  p[1] = (uint8_t)(w >>  8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
  p[4] = (uint8_t)(w >> 32);
  p[5] = (uint8_t)(w >> 40);
}

static BLAKE2_INLINE uint32_t rotr32( const uint32_t w, const unsigned c )
{
  return ( w >> c ) | ( w << ( 32 - c ) );
}

static BLAKE2_INLINE uint64_t rotr64( const uint64_t w, const unsigned c )
{
  return ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
static BLAKE2_INLINE void secure_zero_memory(void *v, size_t n)
{
  static void *(*const volatile memset_v)(void *, int, size_t) = &memset;
  memset_v(v, 0, n);
}

static BLAKE2_INLINE uint64x2_p vec_load64( const void *src )
{
#if defined(_ARCH_PWR9)
  assert((uintptr_t)src % 2 == 0);
  return (uint64x2_p) vec_xl(0, CONST_V8_CAST(src))
#elif defined(__VSX__) || defined(_ARCH_PWR8)
  assert((uintptr_t)src % 4 == 0);
  return (uint64x2_p) vec_xl(0, CONST_V32_CAST(src));
#else
  assert((uintptr_t)src % 16 == 0);
  return (uint64x2_p) vec_ld(0, CONST_V8_CAST(src));
#endif
}

static BLAKE2_INLINE uint64x2_p vec_load64_le( const void *src, const uint8x16_p le_mask )
{
#if defined(NATIVE_BIG_ENDIAN)
  const uint64x2_p v = vec_load64(src);
  return vec_perm(v, v, le_mask);
#else
  BLAKE2_UNUSED(le_mask);
  return vec_load64(src);
#endif
}

static BLAKE2_INLINE void vec_store64( void *dst, uint64x2_p w )
{
#if defined(_ARCH_PWR9)
  assert((uintptr_t)dst % 2 == 0);
  vec_xst((uint8x16_p)w, 0, NCONST_V8_CAST(dst));
#elif defined(__VSX__) || defined(_ARCH_PWR8)
  assert((uintptr_t)dst % 4 == 0);
  vec_xst((uint32x4_p)w, 0, NCONST_V32_CAST(dst));
#else
  assert((uintptr_t)dst % 16 == 0);
  vec_st(w, 0, NCONST_V32_CAST(dst));
#endif
}

static BLAKE2_INLINE void vec_store64_le( void *dst, uint64x2_p w, const uint8x16_p le_mask)
{
#if defined(NATIVE_BIG_ENDIAN)
  uint64x2_p v = vec_perm(w, w, le_mask);
  vec_store64(dst, v);
#else
  BLAKE2_UNUSED(le_mask);
  vec_store64(dst, w);
#endif
}

#endif

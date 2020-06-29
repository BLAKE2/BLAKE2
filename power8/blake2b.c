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

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "blake2.h"
#include "blake2-impl.h"

#include "blake2b-round.h"

#if defined(__GNUC__)
/* Ignore "warning: vec_lvsl is deprecated..." */
# pragma GCC diagnostic ignored "-Wdeprecated"
#endif

static const uint64_t blake2b_IV[8] =
{
  0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

/* Some helper functions */
static void blake2b_set_lastnode( blake2b_state *S )
{
  S->f[1] = (uint64_t)-1;
}

static int blake2b_is_lastblock( const blake2b_state *S )
{
  return S->f[0] != 0;
}

static void blake2b_set_lastblock( blake2b_state *S )
{
  if( S->last_node ) blake2b_set_lastnode( S );

  S->f[0] = (uint64_t)-1;
}

static void blake2b_increment_counter( blake2b_state *S, const uint64_t inc )
{
  S->t[0] += inc;
  S->t[1] += ( S->t[0] < inc );
}

/* init xors IV with input parameter block */
int blake2b_init_param( blake2b_state *S, const blake2b_param *P )
{
  const uint8_t *p = ( const uint8_t * )( P );
  size_t i;

  memset( S, 0, sizeof( blake2b_state ) );

  /* IV XOR ParamBlock */
  for( i = 0; i < 8; ++i )
    S->h[i] = load64( p + sizeof( S->h[i] ) * i ) ^ blake2b_IV[i];

#if defined(NATIVE_BIG_ENDIAN)
  for( i = 0; i < 8; ++i )
    S->h[i] = __builtin_bswap64(S->h[i]);
#endif

  S->outlen = P->digest_length;
  return 0;
}


/* Some sort of default parameter block initialization, for sequential blake2b */
int blake2b_init( blake2b_state *S, size_t outlen )
{
  blake2b_param P[1];

  if ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) return -1;

  P->digest_length = (uint8_t)outlen;
  P->key_length    = 0;
  P->fanout        = 1;
  P->depth         = 1;
  store32( &P->leaf_length, 0 );
  store32( &P->node_offset, 0 );
  store32( &P->xof_length, 0 );
  P->node_depth    = 0;
  P->inner_length  = 0;
  memset( P->reserved, 0, sizeof( P->reserved ) );
  memset( P->salt,     0, sizeof( P->salt ) );
  memset( P->personal, 0, sizeof( P->personal ) );

  return blake2b_init_param( S, P );
}

int blake2b_init_key( blake2b_state *S, size_t outlen, const void *key, size_t keylen )
{
  blake2b_param P[1];

  if ( ( !outlen ) || ( outlen > BLAKE2B_OUTBYTES ) ) return -1;

  if ( ( !keylen ) || keylen > BLAKE2B_KEYBYTES ) return -1;

  P->digest_length = (uint8_t)outlen;
  P->key_length    = (uint8_t)keylen;
  P->fanout        = 1;
  P->depth         = 1;
  store32( &P->leaf_length, 0 );
  store32( &P->node_offset, 0 );
  store32( &P->xof_length, 0 );
  P->node_depth    = 0;
  P->inner_length  = 0;
  memset( P->reserved, 0, sizeof( P->reserved ) );
  memset( P->salt,     0, sizeof( P->salt ) );
  memset( P->personal, 0, sizeof( P->personal ) );

  if( blake2b_init_param( S, P ) < 0 )
    return 0;

  {
    uint8_t block[BLAKE2B_BLOCKBYTES];
    memset( block, 0, BLAKE2B_BLOCKBYTES );
    memcpy( block, key, keylen );
    blake2b_update( S, block, BLAKE2B_BLOCKBYTES );
    secure_zero_memory( block, BLAKE2B_BLOCKBYTES ); /* Burn the key from stack */
  }
  return 0;
}

static void blake2b_compress( blake2b_state *S, const uint8_t block[BLAKE2B_BLOCKBYTES] )
{
  /* Possibly unaligned user messages */
  uint64x2_p m0, m1, m2, m3, m4, m5, m6, m7;

  /* State variables */
  uint64x2_p row1l, row1h;
  uint64x2_p row2l, row2h;
  uint64x2_p row3l, row3h;
  uint64x2_p row4l, row4h;
  uint64x2_p h0, h2, h4, h6;

  /* Alignment check for message buffer load */
  const uintptr_t addr = (uintptr_t)block;

  /* Masks used for right rotates */
  const uint64x2_p mask_ror_16 = {64-16, 64-16};
  const uint64x2_p mask_ror_24 = {64-24, 64-24};
  const uint64x2_p mask_ror_32 = {64-32, 64-32};
  const uint64x2_p mask_ror_63 = {64-63, 64-63};

#if defined(NATIVE_BIG_ENDIAN)
  /* Mask to permute between big-endian and little-endian dword arrays */
  const uint8x16_p mask_le = {7,6,5,4, 3,2,1,0, 15,14,13,12, 11,10,9,8};
#endif

#if defined(_ARCH_PWR9)
  /* POWER9 provides loads for char's and short's */
  m0 = (uint64x2_p) vec_xl(  0, CONST_V8_CAST( block ));
  m1 = (uint64x2_p) vec_xl( 16, CONST_V8_CAST( block ));
  m2 = (uint64x2_p) vec_xl( 32, CONST_V8_CAST( block ));
  m3 = (uint64x2_p) vec_xl( 48, CONST_V8_CAST( block ));
  m4 = (uint64x2_p) vec_xl( 64, CONST_V8_CAST( block ));
  m5 = (uint64x2_p) vec_xl( 80, CONST_V8_CAST( block ));
  m6 = (uint64x2_p) vec_xl( 96, CONST_V8_CAST( block ));
  m7 = (uint64x2_p) vec_xl(112, CONST_V8_CAST( block ));
#else
  /* Altivec only provides 16-byte aligned loads. Low-order address bits */
  /* are masked. Linux provides 16-byte aligned buffers, AIX typically */
  /* provides 4-byte aligned buffers. */
  /* http://www.nxp.com/docs/en/reference-manual/ALTIVECPEM.pdf */
  const size_t off = addr%16;

  m0 = (uint64x2_p) vec_ld(  0, CONST_V8_CAST( block ));
  m1 = (uint64x2_p) vec_ld( 16, CONST_V8_CAST( block ));
  m2 = (uint64x2_p) vec_ld( 32, CONST_V8_CAST( block ));
  m3 = (uint64x2_p) vec_ld( 48, CONST_V8_CAST( block ));
  m4 = (uint64x2_p) vec_ld( 64, CONST_V8_CAST( block ));
  m5 = (uint64x2_p) vec_ld( 80, CONST_V8_CAST( block ));
  m6 = (uint64x2_p) vec_ld( 96, CONST_V8_CAST( block ));
  m7 = (uint64x2_p) vec_ld(112, CONST_V8_CAST( block ));

  if (off != 0)
  {
    /* User buffer not 16-byte aligned. Fix the vectors spread across loads */
    /* http://mirror.informatimago.com/next/developer.apple.com/ */
    /*        hardwaredrivers/ve/code_optimization.html */
    uint64x2_p ex; uint8x16_p perm;
    ex = (uint64x2_p) vec_ld(112+15, CONST_V8_CAST( block ));
    perm = vec_lvsl(0, CONST_V8_CAST( addr ));

    m0 = vec_perm(m0, m1, perm);
    m1 = vec_perm(m1, m2, perm);
    m2 = vec_perm(m2, m3, perm);
    m3 = vec_perm(m3, m4, perm);
    m4 = vec_perm(m4, m5, perm);
    m5 = vec_perm(m5, m6, perm);
    m6 = vec_perm(m6, m7, perm);
    m7 = vec_perm(m7, ex, perm);
  }
#endif

#if defined(NATIVE_BIG_ENDIAN)
  m0 = vec_perm(m0, m0, mask_le);
  m1 = vec_perm(m1, m1, mask_le);
  m2 = vec_perm(m2, m2, mask_le);
  m3 = vec_perm(m3, m3, mask_le);
  m4 = vec_perm(m4, m4, mask_le);
  m5 = vec_perm(m5, m5, mask_le);
  m6 = vec_perm(m6, m6, mask_le);
  m7 = vec_perm(m7, m7, mask_le);
#endif

#if defined(NATIVE_BIG_ENDIAN)
  h0 = row1l = vec_load64_le( &S->h[0], mask_le);
  h2 = row1h = vec_load64_le( &S->h[2], mask_le);
  h4 = row2l = vec_load64_le( &S->h[4], mask_le);
  h6 = row2h = vec_load64_le( &S->h[6], mask_le);
#else
  h0 = row1l = vec_load64( &S->h[0] );
  h2 = row1h = vec_load64( &S->h[2] );
  h4 = row2l = vec_load64( &S->h[4] );
  h6 = row2h = vec_load64( &S->h[6] );
#endif

  row3l = vec_load64( &blake2b_IV[0] );
  row3h = vec_load64( &blake2b_IV[2] );
  row4l = vec_xor( vec_load64( &blake2b_IV[4] ), vec_load64( &S->t[0] ) );
  row4h = vec_xor( vec_load64( &blake2b_IV[6] ), vec_load64( &S->f[0] ) );

  ROUND( 0 );
  ROUND( 1 );
  ROUND( 2 );
  ROUND( 3 );
  ROUND( 4 );
  ROUND( 5 );
  ROUND( 6 );
  ROUND( 7 );
  ROUND( 8 );
  ROUND( 9 );
  ROUND( 10 );
  ROUND( 11 );

  row1l = vec_xor( row3l, row1l );
  row1h = vec_xor( row3h, row1h );

#if defined(NATIVE_BIG_ENDIAN)
  vec_store64_le( &S->h[0], vec_xor( h0, row1l ), mask_le );
  vec_store64_le( &S->h[2], vec_xor( h2, row1h ), mask_le );
#else
  vec_store64( &S->h[0], vec_xor( h0, row1l ) );
  vec_store64( &S->h[2], vec_xor( h2, row1h ) );
#endif

  row2l = vec_xor( row4l, row2l );
  row2h = vec_xor( row4h, row2h );

#if defined(NATIVE_BIG_ENDIAN)
  vec_store64_le( &S->h[4], vec_xor( h4, row2l ), mask_le );
  vec_store64_le( &S->h[6], vec_xor( h6, row2h ), mask_le );
#else
  vec_store64( &S->h[4], vec_xor( h4, row2l ) );
  vec_store64( &S->h[6], vec_xor( h6, row2h ) );
#endif
}


int blake2b_update( blake2b_state *S, const void *pin, size_t inlen )
{
  const unsigned char * in = (const unsigned char *)pin;
  if( inlen > 0 )
  {
    size_t left = S->buflen;
    size_t fill = BLAKE2B_BLOCKBYTES - left;
    if( inlen > fill )
    {
      S->buflen = 0;
      memcpy( S->buf + left, in, fill ); /* Fill buffer */
      blake2b_increment_counter( S, BLAKE2B_BLOCKBYTES );
      blake2b_compress( S, S->buf ); /* Compress */
      in += fill; inlen -= fill;
      while(inlen > BLAKE2B_BLOCKBYTES) {
        blake2b_increment_counter(S, BLAKE2B_BLOCKBYTES);
        blake2b_compress( S, in );
        in += BLAKE2B_BLOCKBYTES;
        inlen -= BLAKE2B_BLOCKBYTES;
      }
    }
    memcpy( S->buf + S->buflen, in, inlen );
    S->buflen += inlen;
  }
  return 0;
}


int blake2b_final( blake2b_state *S, void *out, size_t outlen )
{
  if( out == NULL || outlen < S->outlen )
    return -1;

  if( blake2b_is_lastblock( S ) )
    return -1;

  blake2b_increment_counter( S, S->buflen );
  blake2b_set_lastblock( S );
  memset( S->buf + S->buflen, 0, BLAKE2B_BLOCKBYTES - S->buflen ); /* Padding */
  blake2b_compress( S, S->buf );

  memcpy( out, &S->h[0], S->outlen );
  return 0;
}


int blake2b( void *out, size_t outlen, const void *in, size_t inlen, const void *key, size_t keylen )
{
  blake2b_state S[1];

  /* Verify parameters */
  if ( NULL == in && inlen > 0 ) return -1;

  if ( NULL == out ) return -1;

  if( NULL == key && keylen > 0 ) return -1;

  if( !outlen || outlen > BLAKE2B_OUTBYTES ) return -1;

  if( keylen > BLAKE2B_KEYBYTES ) return -1;

  if( keylen )
  {
    if( blake2b_init_key( S, outlen, key, keylen ) < 0 ) return -1;
  }
  else
  {
    if( blake2b_init( S, outlen ) < 0 ) return -1;
  }

  blake2b_update( S, ( const uint8_t * )in, inlen );
  blake2b_final( S, out, outlen );
  return 0;
}

int blake2( void *out, size_t outlen, const void *in, size_t inlen, const void *key, size_t keylen ) {
  return blake2b(out, outlen, in, inlen, key, keylen);
}

#if defined(SUPERCOP)
int crypto_hash( unsigned char *out, unsigned char *in, unsigned long long inlen )
{
  return blake2b( out, BLAKE2B_OUTBYTES, in, inlen, NULL, 0 );
}
#endif

#if defined(BLAKE2B_SELFTEST)
#include <string.h>
#include "blake2-kat.h"
int main( void )
{
  uint8_t key[BLAKE2B_KEYBYTES];
  uint8_t buf[BLAKE2_KAT_LENGTH];
  size_t i, step;

  for( i = 0; i < BLAKE2B_KEYBYTES; ++i )
    key[i] = ( uint8_t )i;

  for( i = 0; i < BLAKE2_KAT_LENGTH; ++i )
    buf[i] = ( uint8_t )i;

  /* Test simple API */
  for( i = 0; i < BLAKE2_KAT_LENGTH; ++i )
  {
    uint8_t hash[BLAKE2B_OUTBYTES];
    blake2b( hash, BLAKE2B_OUTBYTES, buf, i, key, BLAKE2B_KEYBYTES );

    if( 0 != memcmp( hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES ) )
    {
      goto fail;
    }
  }

  /* Test streaming API */
  for(step = 1; step < BLAKE2B_BLOCKBYTES; ++step) {
    for (i = 0; i < BLAKE2_KAT_LENGTH; ++i) {
      uint8_t hash[BLAKE2B_OUTBYTES];
      blake2b_state S;
      uint8_t * p = buf;
      size_t mlen = i;
      int err = 0;

      if( (err = blake2b_init_key(&S, BLAKE2B_OUTBYTES, key, BLAKE2B_KEYBYTES)) < 0 ) {
        goto fail;
      }

      while (mlen >= step) {
        if ( (err = blake2b_update(&S, p, step)) < 0 ) {
          goto fail;
        }
        mlen -= step;
        p += step;
      }
      if ( (err = blake2b_update(&S, p, mlen)) < 0) {
        goto fail;
      }
      if ( (err = blake2b_final(&S, hash, BLAKE2B_OUTBYTES)) < 0) {
        goto fail;
      }

      if (0 != memcmp(hash, blake2b_keyed_kat[i], BLAKE2B_OUTBYTES)) {
        goto fail;
      }
    }
  }

  puts( "ok" );
  return 0;
fail:
  puts("error");
  return -1;
}
#endif

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "blake2.h"
#include "blake2-impl.h"


int blake2xs_init( blake2s_state *S, const uint16_t outlen, const void *key, const uint8_t keylen)
{
  blake2s_param P[1];

  if ( !outlen ) return -1;

  P->digest_length = BLAKE2S_OUTBYTES;
  P->key_length    = keylen;
  P->fanout        = 1;
  P->depth         = 1;
  store32( &P->leaf_length, 0 );
  store32( &P->node_offset, 0 );
  store16( &P->xof_length, outlen );
  P->node_depth    = 0;
  P->inner_length  = 0;
  memset( P->salt,     0, sizeof( P->salt ) );
  memset( P->personal, 0, sizeof( P->personal ) );

  if( blake2s_init_param( S, P ) < 0 ) return -1;

    if (keylen) 
    {
        if ( !key || keylen > BLAKE2S_KEYBYTES ) return -1;
        uint8_t block[BLAKE2S_BLOCKBYTES];
        memset( block, 0, BLAKE2S_BLOCKBYTES );
        memcpy( block, key, keylen );
        blake2s_update( S, block, BLAKE2S_BLOCKBYTES );
        secure_zero_memory( block, BLAKE2S_BLOCKBYTES );
    }
  return 0;
}


int blake2xs_update( blake2s_state *S, const uint8_t *in, uint64_t inlen )
{
    if ( !in ) return -1;

    return blake2s_update( S, in, inlen );

}

int blake2xs_final( blake2s_state *S, uint8_t *out, const uint16_t outlen ) {

    blake2s_state C[1];
    blake2s_param P[1];
    int i;
    uint16_t last_index = outlen/BLAKE2S_OUTBYTES;
    uint8_t root[BLAKE2S_BLOCKBYTES];

    if ( !out ) return 1;

    /* Finalize the root hash */
    if (blake2s_final( S, root, BLAKE2S_OUTBYTES ) < 0 )
        return -1;

    /* Set common block structure values */
    P->digest_length = BLAKE2S_OUTBYTES;
    P->key_length    = 0;
    P->fanout        = 0; 
    P->depth         = 0;
    store32( &P->leaf_length, BLAKE2S_OUTBYTES );
    store16( &P->xof_length, outlen );
    P->inner_length  = 0;
    P->node_depth    = 0;
    memset( P->salt,     0, sizeof( P->salt ) );
    memset( P->personal, 0, sizeof( P->personal ) );

    for( i = 0; i < last_index; ++ i ) {
        /* Initialize state */
        store32( &P->node_offset, i );
        if( blake2s_init_param( C, P ) < 0 ) return -1;
        /* Process key if needed */
        blake2s_update( C, root, BLAKE2S_OUTBYTES);
        blake2s_final( C, out + i*BLAKE2S_OUTBYTES, BLAKE2S_OUTBYTES );
    }
    /* Last instance */
    store32( &P->node_offset, last_index);
    P->digest_length = outlen % BLAKE2S_OUTBYTES;
    if( blake2s_init_param( C, P ) < 0 ) return -1;
    blake2s_update( C, root, BLAKE2S_OUTBYTES);
    blake2s_final( C, out + last_index*BLAKE2S_OUTBYTES, outlen % BLAKE2S_OUTBYTES );
     
     return 0;
}

int blake2xs( uint8_t *out, const void *in, const void *key, const
uint16_t outlen, const uint64_t inlen, const uint8_t keylen ) 
{
    blake2s_state S[1];

    /* Verify parameters */
    if ( NULL == in && inlen > 0 ) return -1;

    if ( NULL == out ) return -1;

    if ( NULL == key && keylen > 0) return -1;

    if( keylen > BLAKE2S_KEYBYTES ) return -1;

    if( !outlen ) return -1;

    /* Initialize the root block structure */
    if ( blake2xs_init( S, outlen, key, keylen ) < 0 ) {
        return -1;
    }

    /* Compute the root of the tree */
    if ( blake2xs_update( S, ( const uint8_t * )in, inlen ) < 0 )
        return -1;

    /* Compute the final hash using the counter construction */
    blake2xs_final( S, out, outlen );

    return 0;
}

#if defined(BLAKE2XS_SELFTEST)
#include <string.h>
#include "blake2-kat.h"
int main( int argc, char **argv )
{
  uint8_t key[BLAKE2S_KEYBYTES];
  uint8_t buf[BLAKE2_KAT_LENGTH];

  for( size_t i = 0; i < BLAKE2S_KEYBYTES; ++i )
    key[i] = ( uint8_t )i;

  for( size_t i = 0; i < BLAKE2_KAT_LENGTH; ++i )
    buf[i] = ( uint8_t )i;

  for( size_t i = 0; i < BLAKE2_KAT_LENGTH; ++i )
  {
      uint8_t hash[BLAKE2_KAT_LENGTH];
      blake2xs( hash, buf, key, i, BLAKE2_KAT_LENGTH, BLAKE2S_KEYBYTES );

      for( size_t j = 0; j < i; ++j ) {
          printf("%02x", hash[j]);
      }
      printf("\n");
  }

  //puts( "ok" );
  return 0;
}
#endif


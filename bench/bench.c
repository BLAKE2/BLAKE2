/*
   BLAKE2 reference source code package - benchmark tool

   Written in 2012 by Samuel Neves <sneves@dei.uc.pt>

   To the extent possible under law, the author(s) have dedicated all copyright
   and related and neighboring rights to this software to the public domain
   worldwide. This software is distributed without any warranty.

   You should have received a copy of the CC0 Public Domain Dedication along with
   this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int crypto_hash( unsigned char *out, const unsigned char *in, unsigned long long inlen );

static int bench_cmp( const void *x, const void *y )
{
  const int64_t *ix = ( const int64_t * )x;
  const int64_t *iy = ( const int64_t * )y;
  return *ix - *iy;
}

unsigned long long cpucycles( void );

void bench()
{
#define BENCH_TRIALS     32
#define BENCH_MAXLEN   1536
  static unsigned char in[4096];
  static unsigned long long median[4096 + 1];
  int i, j;
  printf( "#bytes  median  per byte\n" );

  /* 1 ... BENCH_MAXLEN */
  for( j = 0; j <= 4096; ++j )
  {
    uint64_t cycles[BENCH_TRIALS + 1];

    for( i = 0; i <= BENCH_TRIALS; ++i )
    {
      cycles[i] = cpucycles();
      crypto_hash( in, in, j );
    }

    for( i = 0; i < BENCH_TRIALS; ++i )
      cycles[i] = cycles[i + 1] - cycles[i];

    qsort( cycles, BENCH_TRIALS, sizeof( uint64_t ), bench_cmp );
    median[j] = cycles[BENCH_TRIALS / 2];
  }

  for( j = 0; j <= BENCH_MAXLEN; j += 8 )
    printf( "%5d, %7.2f\n", j, ( double )median[j] / j );

  printf( "#2048   %6llu   %7.2f\n", median[2048], ( double )median[2048] / 2048.0 );
  printf( "#4096   %6llu   %7.2f\n", median[4096], ( double )median[4096] / 4096.0 );
  printf( "#long     long   %7.2f\n", ( double )( median[4096] - median[2048] ) / 2048.0 );
}

int main()
{
  bench();
  return 0;
}

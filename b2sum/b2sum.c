/*
   BLAKE2 reference source code package - b2sum tool

   Copyright 2012, Samuel Neves <sneves@dei.uc.pt>.  You may use this under the
   terms of the CC0, the OpenSSL Licence, or the Apache Public License 2.0, at
   your option.  The terms of these licenses can be found at:

   - CC0 1.0 Universal : http://creativecommons.org/publicdomain/zero/1.0
   - OpenSSL license   : https://www.openssl.org/source/license.html
   - Apache 2.0        : http://www.apache.org/licenses/LICENSE-2.0

   More information about the BLAKE2 hash function can be found at
   https://blake2.net.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>

#include "blake2.h"

/* 256 byte keys should be more than enough for any use-case */
#define HMAC_MAX_KEY_LEN 256

/* This will help compatibility with coreutils */
int blake2s_stream( FILE *stream, void *resstream, size_t outbytes )
{
  int ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  static const size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  if( !buffer ) return -1;

  blake2s_init( S, outbytes );

  while( 1 )
  {
    sum = 0;

    while( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      if( buffer_length == sum )
        break;

      if( 0 == n )
      {
        if( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      if( feof( stream ) )
        goto final_process;
    }

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  if( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  return ret;
}

int blake2b_stream( FILE *stream, void *resstream, size_t outbytes )
{
  int ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  static const size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  if( !buffer ) return -1;

  blake2b_init( S, outbytes );

  while( 1 )
  {
    sum = 0;

    while( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      if( buffer_length == sum )
        break;

      if( 0 == n )
      {
        if( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      if( feof( stream ) )
        goto final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  if( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  return ret;
}

int blake2sp_stream( FILE *stream, void *resstream, size_t outbytes )
{
  int ret = -1;
  size_t sum, n;
  blake2sp_state S[1];
  static const size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  if( !buffer ) return -1;

  blake2sp_init( S, outbytes );

  while( 1 )
  {
    sum = 0;

    while( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      if( buffer_length == sum )
        break;

      if( 0 == n )
      {
        if( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      if( feof( stream ) )
        goto final_process;
    }

    blake2sp_update( S, buffer, buffer_length );
  }

final_process:;

  if( sum > 0 ) blake2sp_update( S, buffer, sum );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  return ret;
}


int blake2bp_stream( FILE *stream, void *resstream, size_t outbytes )
{
  int ret = -1;
  size_t sum, n;
  blake2bp_state S[1];
  static const size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  if( !buffer ) return -1;

  blake2bp_init( S, outbytes );

  while( 1 )
  {
    sum = 0;

    while( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      if( buffer_length == sum )
        break;

      if( 0 == n )
      {
        if( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      if( feof( stream ) )
        goto final_process;
    }

    blake2bp_update( S, buffer, buffer_length );
  }

final_process:;

  if( sum > 0 ) blake2bp_update( S, buffer, sum );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  return ret;
}


int blake2b_stream_hmac( unsigned char *key, FILE *stream, void *resstream, size_t outbytes )
{
  int ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  static const size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );
  size_t i;
  size_t j;

  if( !buffer ) return -1;

  blake2b_init( S, outbytes );
  i = 0;
  while(i < HMAC_MAX_KEY_LEN){
    for(j=0; j<buffer_length; ++j){
      if(j < HMAC_MAX_KEY_LEN){
        buffer[j] = key[j];
        ++ sum;
        ++ i;
      }
    }
  }
  blake2b_update( S, buffer, HMAC_MAX_KEY_LEN );

  while( 1 )
  {
    sum = 0;

    while( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      if( buffer_length == sum )
        break;

      if( 0 == n )
      {
        if( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      if( feof( stream ) )
        goto final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  if( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  return ret;
}
int blake2b_stream_build_key( unsigned char *data, size_t data_len, void *resstream, size_t outbytes )
{
  printf("Building key...");
  int ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  static const size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );
  size_t block_len;
  size_t i;
  size_t j;

  if( !buffer ) return -1;

  blake2b_init( S, outbytes );
  
  blake2b_update( S, data, data_len );
  for(i=0; i<data_len; ++i){
    if(data_len > buffer_length)
      buffer[i] = data[data_len - buffer_length + i];
    else
      buffer[i] = data[i];
  }
  sum = data_len;

final_process:;

  if( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  return ret;
}

typedef int ( *blake2fn )( FILE *, void *, size_t );


static void usage( char **argv, int errcode )
{
  FILE *out = errcode ? stderr : stdout;
  fprintf( out, "Usage: %s [OPTION]... [FILE]...\n", argv[0] );
  fprintf( out, "\n" );
  fprintf( out, "With no FILE, or when FILE is -, read standard input.\n" );
  fprintf( out, "\n" );
  fprintf( out, "  -a <algo>    hash algorithm (blake2b is default): \n"
                "               [blake2b|blake2s|blake2bp|blake2sp]\n" );
  fprintf( out, "  -h <key>     hash in HMAC mode\n"
                "               if <key> is '-' stdin will be used (only applies to blake2b)\n" );
  fprintf( out, "  -l <length>  digest length in bits, must not exceed the maximum for\n"
                "               the selected algorithm and must be a multiple of 8\n" );
  fprintf( out, "  --tag        create a BSD-style checksum\n" );
  fprintf( out, "  --help       display this help and exit\n" );
  exit( errcode );
}


int main( int argc, char **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  const char *algorithm = "BLAKE2b";
  unsigned long outbytes = 0;
  unsigned char hash[BLAKE2B_OUTBYTES] = {0};
  unsigned char hmac_key[HMAC_MAX_KEY_LEN + 1]; /* add an extra byte to make room for a null-terminator */
  unsigned char use_hmac = 0;
  size_t hmac_key_len = 0;
  bool bsdstyle = false;
  int c, i;
  opterr = 1;

  while( 1 )
  {
    int option_index = 0;
    char *end = NULL;
    unsigned long outbits;
    static struct option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { NULL, 0, NULL, 0 }
    };

    c = getopt_long( argc, argv, "a:l:h:", long_options, &option_index );
    if( c == -1 ) break;
    switch( c )
    {
    case 'a':
      if( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      else if ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      else if ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      else if ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      else
      {
        printf( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      break;

    case 'l':
      outbits = strtoul(optarg, &end, 10);
      if( !end || *end != '\0' || outbits % 8 != 0)
      {
        printf( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      break;
    case 'h':
      use_hmac = 0xFF;
      hmac_key_len = 0;
      if ( 0 == strcmp( optarg, "-" ) ){
        hmac_key_len = 0;
        while(hmac_key_len < HMAC_MAX_KEY_LEN){
          hmac_key[hmac_key_len++] = 0;
        }
        hmac_key[HMAC_MAX_KEY_LEN] = 0;
        hmac_key_len = 0;
        while(hmac_key_len < HMAC_MAX_KEY_LEN){
          unsigned char c = getc(stdin);
          if(c == '\n' || c == '\r') break;
          hmac_key[hmac_key_len++] = c;
        }
      } else {
        hmac_key_len = 0;
        while(hmac_key_len < HMAC_MAX_KEY_LEN){
          hmac_key[hmac_key_len++] = 0;
        }
        hmac_key[HMAC_MAX_KEY_LEN] = 0;
        hmac_key_len = 0;
        while(hmac_key_len < HMAC_MAX_KEY_LEN){
          unsigned char c = optarg[hmac_key_len];
          if(c == 0) break;
          hmac_key[hmac_key_len++] = c;
        }
      } /* Now you have a zero-padded key, per spec */
      break;
    case 0:
      if( 0 == strcmp( "help", long_options[option_index].name ) )
        usage( argv, 0 );
      else if( 0 == strcmp( "tag", long_options[option_index].name ) )
        bsdstyle = true;
      break;

    case '?':
      usage( argv, 1 );
      break;
    }
  }

  if(outbytes > maxbytes)
  {
    printf( "Invalid length argument: %lu\n", outbytes * 8 );
    printf( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  else if( outbytes == 0 )
    outbytes = maxbytes;

  if( optind == argc )
    argv[argc++] = (char *) "-";

  for( i = optind; i < argc; ++i )
  {
    FILE *f = NULL;
    if( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    else
      f = fopen( argv[i], "rb" );

    if( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      continue;
    }
    if(use_hmac){
      if(hmac_key_len > outbytes){
        size_t j;
        blake2b_stream_build_key( hmac_key, hmac_key_len, hash, outbytes );
        for(j=0; j<outbytes; ++j){
          hmac_key[j] = hash[j];
        }
        hmac_key_len = outbytes;
      }
	    /* The indentation is weird because I got lazy and used tab */
            size_t j;
            for(j=0; j<outbytes; ++j){
              hmac_key[j] ^= 0x5C; /* XOR the key with 0x5C */
            }
	    if( blake2b_stream_hmac( hmac_key, f, hash, outbytes ) < 0 )
	    {
	      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
	    }
	    else
	    {
              for(j=0; j<outbytes; ++j){
                hmac_key[j] ^= 0x5C; /* remove the previous XOR */
                hmac_key[j] ^= 0x36; /* apply the new XOR */
                /*
                  But Alex, why didn't you just merge them into 1 XOR?
                  So people reading this code can see the spec; why else?
                */
              }
	      if( blake2b_stream_hmac( hmac_key, f, hash, outbytes ) < 0 )
	      {
	        fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
	      }
	      else
	      {
		      size_t j;
		      if( bsdstyle )
		      {
			if( outbytes < maxbytes )
			  printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
			else
			  printf( "%s (%s) = ", algorithm, argv[i] );
		      }

		      for( j = 0; j < outbytes; ++j )
			printf( "%02x", hash[j] );

		      if( bsdstyle )
			printf( "\n" );
		      else
			printf( "  %s\n", argv[i] );
	      }
            }
    } else{
	    if( blake2_stream( f, hash, outbytes ) < 0 )
	    {
	      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
	    }
	    else
	    {
	      size_t j;
	      if( bsdstyle )
	      {
		if( outbytes < maxbytes )
		  printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
		else
		  printf( "%s (%s) = ", algorithm, argv[i] );
	      }

	      for( j = 0; j < outbytes; ++j )
		printf( "%02x", hash[j] );

	      if( bsdstyle )
		printf( "\n" );
	      else
		printf( "  %s\n", argv[i] );
	    }
    }

    if( f != stdin ) fclose( f );
  }

  return 0;
}

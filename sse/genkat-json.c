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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "blake2.h"

#define STR_(x) #x
#define STR(x) STR_(x)

#define LENGTH 256

#define MAKE_KAT(name,size_prefix) \
do  \
{ \
  for( size_t i = 0; i < LENGTH; ++i ) \
  { \
    printf("\n{\n");\
     \
    printf("    \"hash\": \"" #name "\",\n");\
    printf("    \"in\": \"");\
    for( int j = 0; j < i; ++j ) printf( "%02x", in[j]);\
    \
    printf( "\",\n" ); \
    printf("    \"key\": \"\",\n");\
    printf("    \"out\": \"");\
    \
    name( hash, in, NULL, size_prefix ## _OUTBYTES, i, 0 ); \
    \
    for( int j = 0; j < size_prefix ## _OUTBYTES; ++j ) \
        printf( "%02x", hash[j]);\
    printf( "\"\n" ); \
    printf( "}," ); \
  }\
} while (0)

#define MAKE_KEYED_KAT(name,size_prefix) \
do  \
{ \
  for( size_t i = 0; i < LENGTH; ++i ) \
  { \
    printf("\n{\n");\
     \
    printf("    \"hash\": \"" #name "\",\n");\
    printf("    \"in\": \"");\
    for( int j = 0; j < i; ++j ) printf( "%02x", in[j]);\
    \
    printf( "\",\n" ); \
    printf("    \"key\": \"");\
    for( int j = 0; j < size_prefix ## _KEYBYTES; ++j ) printf( "%02x", key[j]);\
    printf("\",\n");\
    printf("    \"out\": \"");\
    \
    name( hash, in, key, size_prefix ## _OUTBYTES, i, size_prefix ## _KEYBYTES ); \
    \
    for( int j = 0; j < size_prefix ## _OUTBYTES; ++j ) \
        printf( "%02x", hash[j]);\
    printf( "\"\n" ); \
    printf( "}," ); \
  }\
} while (0)


int main( int argc, char **argv )
{
  uint8_t key[64] = {0};
  uint8_t in[LENGTH] = {0};
  uint8_t hash[64] = {0};

  for( size_t i = 0; i < sizeof( in ); ++i )
    in[i] = i;

  for( size_t i = 0; i < sizeof( key ); ++i )
    key[i] = i;

  printf("[");          
  MAKE_KAT( blake2s, BLAKE2S );
  MAKE_KEYED_KAT( blake2s, BLAKE2S );
  MAKE_KAT( blake2b, BLAKE2B );
  MAKE_KEYED_KAT( blake2b, BLAKE2B );
  MAKE_KAT( blake2sp, BLAKE2S );
  MAKE_KEYED_KAT( blake2sp, BLAKE2S );
  MAKE_KAT( blake2bp, BLAKE2B );
  MAKE_KEYED_KAT( blake2bp, BLAKE2B );
  printf("\n]\n");          
  fflush(stdout);
  return 0;
}

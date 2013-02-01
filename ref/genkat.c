/*
   BLAKE2 reference source code package - reference C implementations

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

#include "blake2.h"

#define STR_(x) #x
#define STR(x) STR_(x)

#define LENGTH 256

#define MAKE_KAT(name,size_prefix) \
do  \
{ \
  printf( "static const uint8_t " #name "_kat[KAT_LENGTH][" #size_prefix "_OUTBYTES] = \n{\n" ); \
   \
  for( size_t i = 0; i < LENGTH; ++i ) \
  { \
    name( hash, in, NULL, size_prefix ## _OUTBYTES, i, 0 ); \
    printf( "\t{\n\t\t" ); \
 \
  for( int j = 0; j < size_prefix ## _OUTBYTES; ++j ) \
      printf( "0x%02X%s", hash[j], ( j + 1 ) == size_prefix ## _OUTBYTES ? "\n" : j && !( ( j + 1 ) % 8 ) ? ",\n\t\t" : ", " ); \
      \
    printf( "\t},\n" ); \
  } \
   \
  printf( "};\n\n\n\n\n" ); \
  \
} while (0)

#define MAKE_KEYED_KAT(name,size_prefix) \
do  \
{ \
  printf( "static const uint8_t " #name "_keyed_kat[KAT_LENGTH][" #size_prefix "_OUTBYTES] = \n{\n" ); \
  \
  for( size_t i = 0; i < LENGTH; ++i ) \
  { \
  name( hash, in, key, size_prefix ## _OUTBYTES, i, size_prefix ## _KEYBYTES ); \
  printf( "\t{\n\t\t" ); \
  \
  for( int j = 0; j < size_prefix ## _OUTBYTES; ++j ) \
  printf( "0x%02X%s", hash[j], ( j + 1 ) == size_prefix ## _OUTBYTES ? "\n" : j && !( ( j + 1 ) % 8 ) ? ",\n\t\t" : ", " ); \
  \
  printf( "\t},\n" ); \
  } \
  \
  printf( "};\n\n\n\n\n" ); \
  \
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

  puts( "#pragma once\n"
        "#ifndef __BLAKE2_KAT_H__\n"
        "#define __BLAKE2_KAT_H__\n\n\n"
        "#include <stdint.h>\n\n"
        "#define KAT_LENGTH " STR( LENGTH ) "\n\n\n" );
  MAKE_KAT( blake2s, BLAKE2S );
  MAKE_KEYED_KAT( blake2s, BLAKE2S );
  MAKE_KAT( blake2b, BLAKE2B );
  MAKE_KEYED_KAT( blake2b, BLAKE2B );
  MAKE_KAT( blake2sp, BLAKE2S );
  MAKE_KEYED_KAT( blake2sp, BLAKE2S );
  MAKE_KAT( blake2bp, BLAKE2B );
  MAKE_KEYED_KAT( blake2bp, BLAKE2B );
  /*printf( "static const uint8_t blake2s_kat[KAT_LENGTH][BLAKE2S_OUTBYTES] = \n{\n" );

  for( size_t i = 0; i < LENGTH; ++i )
  {
    blake2s( hash, in, NULL, BLAKE2S_OUTBYTES, i, 0 );
    printf( "\t{\n\t\t" );

    for( int j = 0; j < BLAKE2S_OUTBYTES; ++j )
      printf( "0x%02X%s", hash[j], ( j + 1 ) == BLAKE2S_OUTBYTES ? "\n" : j && !( ( j + 1 ) % 8 ) ? ",\n\t\t" : ", " );

    printf( "\t},\n" );
  }

  printf( "};\n\n\n\n\n" );
  printf( "static const uint8_t blake2s_keyed_kat[KAT_LENGTH][BLAKE2S_OUTBYTES] = \n{\n" );

  for( size_t i = 0; i < LENGTH; ++i )
  {
    blake2s( hash, in, key, BLAKE2S_OUTBYTES, i, BLAKE2S_KEYBYTES );
    printf( "\t{\n\t\t" );

    for( int j = 0; j < BLAKE2S_OUTBYTES; ++j )
      printf( "0x%02X%s", hash[j], ( j + 1 ) == BLAKE2S_OUTBYTES ? "\n" : j && !( ( j + 1 ) % 8 ) ? ",\n\t\t" : ", " );

    printf( "\t},\n" );
  }

  printf( "};\n\n\n\n\n" );
  printf( "static const uint8_t blake2b_kat[KAT_LENGTH][BLAKE2B_OUTBYTES] = \n{\n" );

  for( size_t i = 0; i < LENGTH; ++i )
  {
    blake2b( hash, in, NULL, BLAKE2B_OUTBYTES, i, 0 );
    printf( "\t{\n\t\t" );

    for( int j = 0; j < BLAKE2B_OUTBYTES; ++j )
      printf( "0x%02X%s", hash[j], ( j + 1 ) == BLAKE2B_OUTBYTES ? "\n" : j && !( ( j + 1 ) % 8 ) ? ",\n\t\t" : ", " );

    printf( "\t},\n" );
  }

  printf( "};\n\n\n\n\n" );
  printf( "static const uint8_t blake2b_keyed_kat[KAT_LENGTH][BLAKE2B_OUTBYTES] = \n{\n" );

  for( size_t i = 0; i < LENGTH; ++i )
  {
    blake2b( hash, in, key, BLAKE2B_OUTBYTES, i, BLAKE2B_KEYBYTES );
    printf( "\t{\n\t\t" );

    for( int j = 0; j < BLAKE2B_OUTBYTES; ++j )
      printf( "0x%02X%s", hash[j], ( j + 1 ) == BLAKE2B_OUTBYTES ? "\n" : j && !( ( j + 1 ) % 8 ) ? ",\n\t\t" : ", " );

    printf( "\t},\n" );
  }

  printf( "};\n\n\n\n\n" );


  printf( "static const uint8_t blake2sp_kat[KAT_LENGTH][BLAKE2S_OUTBYTES] = \n{\n" );

  for( size_t i = 0; i < LENGTH; ++i )
  {
    blake2sp( hash, in, NULL, BLAKE2S_OUTBYTES, i, 0 );
    printf( "\t{\n\t\t" );

    for( int j = 0; j < BLAKE2S_OUTBYTES; ++j )
      printf( "0x%02X%s", hash[j], ( j + 1 ) == BLAKE2S_OUTBYTES ? "\n" : j && !( ( j + 1 ) % 8 ) ? ",\n\t\t" : ", " );

    printf( "\t},\n" );
  }

  printf( "};\n\n\n\n\n" );
  printf( "static const uint8_t blake2sp_keyed_kat[KAT_LENGTH][BLAKE2S_OUTBYTES] = \n{\n" );

  for( size_t i = 0; i < LENGTH; ++i )
  {
    blake2sp( hash, in, key, BLAKE2S_OUTBYTES, i, BLAKE2S_KEYBYTES );
    printf( "\t{\n\t\t" );

    for( int j = 0; j < BLAKE2S_OUTBYTES; ++j )
      printf( "0x%02X%s", hash[j], ( j + 1 ) == BLAKE2S_OUTBYTES ? "\n" : j && !( ( j + 1 ) % 8 ) ? ",\n\t\t" : ", " );

    printf( "\t},\n" );
  }

  printf( "};\n\n\n\n\n" );


  printf( "static const uint8_t blake2bp_kat[KAT_LENGTH][BLAKE2B_OUTBYTES] = \n{\n" );

  for( size_t i = 0; i < LENGTH; ++i )
  {
    blake2bp( hash, in, NULL, BLAKE2B_OUTBYTES, i, 0 );
    printf( "\t{\n\t\t" );

    for( int j = 0; j < BLAKE2B_OUTBYTES; ++j )
      printf( "0x%02X%s", hash[j], ( j + 1 ) == BLAKE2B_OUTBYTES ? "\n" : j && !( ( j + 1 ) % 8 ) ? ",\n\t\t" : ", " );

    printf( "\t},\n" );
  }

  printf( "};\n\n\n\n\n" );
  printf( "static const uint8_t blake2bp_keyed_kat[KAT_LENGTH][BLAKE2B_OUTBYTES] = \n{\n" );

  for( size_t i = 0; i < LENGTH; ++i )
  {
    blake2bp( hash, in, key, BLAKE2B_OUTBYTES, i, BLAKE2B_KEYBYTES );
    printf( "\t{\n\t\t" );

    for( int j = 0; j < BLAKE2B_OUTBYTES; ++j )
      printf( "0x%02X%s", hash[j], ( j + 1 ) == BLAKE2B_OUTBYTES ? "\n" : j && !( ( j + 1 ) % 8 ) ? ",\n\t\t" : ", " );

    printf( "\t},\n" );
  }

  printf( "};\n\n\n\n\n" );*/
  puts( "#endif\n\n\n" );
  return 0;
}
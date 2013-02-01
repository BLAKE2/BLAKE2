/*
   BLAKE2 reference source code package - benchmark tool

   Written in 2012 by Samuel Neves <sneves@dei.uc.pt>

   To the extent possible under law, the author(s) have dedicated all copyright
   and related and neighboring rights to this software to the public domain
   worldwide. This software is distributed without any warranty.

   You should have received a copy of the CC0 Public Domain Dedication along with
   this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/
#include <stddef.h>
#include <openssl/md5.h>
//#include "crypto_hash.h"

int crypto_hash( unsigned char *out, const unsigned char *in, unsigned long long inlen )
{
  MD5( in, inlen, out );
  return 0;
}

/*
   BLAKE2 reference source code package - benchmark tool

   Copyright 2012, Samuel Neves <sneves@dei.uc.pt>.  You may use this under the
   terms of the CC0, the OpenSSL Licence, or the Apache Public License 2.0, at
   your option.  The terms of these licenses can be found at:

   - CC0 1.0 Universal : http://creativecommons.org/publicdomain/zero/1.0
   - OpenSSL license   : https://www.openssl.org/source/license.html
   - Apache 2.0        : http://www.apache.org/licenses/LICENSE-2.0

   More information about the BLAKE2 hash function can be found at
   https://blake2.net.
*/
#include <stddef.h>
#include <openssl/md5.h>

int crypto_hash( unsigned char *out, const unsigned char *in, unsigned long long inlen )
{
  MD5( in, inlen, out );
  return 0;
}

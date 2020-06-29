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
#ifndef BLAKE2_CONFIG_H
#define BLAKE2_CONFIG_H

#if !defined(__ALTIVEC__)
# error "This code requires at least Altivec."
#endif

#if !defined(_ARCH_PWR8)
# error "This code requires at least POWER8."
#endif

/* https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html */

#if  (defined(__GNUC__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)) || \
    ((defined(__xlc__) || defined(__xlC__)) && defined(__LITTLE_ENDIAN__))
# define NATIVE_LITTLE_ENDIAN 1
#endif

#if  (defined(__GNUC__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)) || \
    ((defined(__xlc__) || defined(__xlC__)) && defined(__BIG_ENDIAN__))
# define NATIVE_BIG_ENDIAN 1
#endif

#endif

/*
   BLAKE2 reference source code package - benchmark tool

   Written in 2012 by Samuel Neves <sneves@dei.uc.pt>

   To the extent possible under law, the author(s) have dedicated all copyright
   and related and neighboring rights to this software to the public domain
   worldwide. This software is distributed without any warranty.

   You should have received a copy of the CC0 Public Domain Dedication along with
   this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

unsigned long long cpucycles( void )
{
  unsigned long long result;
  asm volatile( ".byte 15;.byte 49;shlq $32,%%rdx;orq %%rdx,%%rax"
                : "=a" ( result ) ::  "%rdx" );
  return result;
}

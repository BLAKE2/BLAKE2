CC?=gcc
CFLAGS=-std=c99 -Wall -pedantic

all:		blake2s blake2b blake2sp blake2bp

blake2s:	blake2s-ref.c
		$(CC) blake2s-ref.c -o $@ $(CFLAGS) -DBLAKE2S_SELFTEST

blake2b:	blake2b-ref.c
		$(CC) blake2b-ref.c -o $@ $(CFLAGS) -DBLAKE2B_SELFTEST

blake2sp:	blake2sp-ref.c blake2s-ref.c
		$(CC) blake2sp-ref.c blake2s-ref.c -o $@ $(CFLAGS) -DBLAKE2SP_SELFTEST

blake2bp:	blake2bp-ref.c blake2b-ref.c
		$(CC) blake2bp-ref.c blake2b-ref.c -o $@ $(CFLAGS) -DBLAKE2BP_SELFTEST

kat:
		$(CC) $(CFLAGS) -o genkat genkat.c blake2b-ref.c blake2s-ref.c blake2sp-ref.c blake2bp-ref.c
		./genkat > blake2-kat.h

clean:		
		rm -rf *.o genkat blake2s blake2b blake2sp blake2bp

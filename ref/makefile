CC=gcc
CFLAGS=-O2 -Wall -I../testvectors

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
		$(CC) $(CFLAGS) -o genkat-c genkat-c.c blake2b-ref.c blake2s-ref.c blake2sp-ref.c blake2bp-ref.c
		$(CC) $(CFLAGS) -g -o genkat-json genkat-json.c blake2b-ref.c blake2s-ref.c blake2sp-ref.c blake2bp-ref.c
		./genkat-c > blake2-kat.h
		./genkat-json > blake2-kat.json

clean:		
		rm -rf *.o genkat-c genkat-json blake2s blake2b blake2sp blake2bp

CC=gcc
CFLAGS=-Wall -O3 -march=native -I../testvectors

all:		blake2s blake2b blake2sp blake2bp

blake2s:	blake2s.c
		$(CC) blake2s.c -o $@ $(CFLAGS) -DBLAKE2S_SELFTEST

blake2b:	blake2b.c
		$(CC) blake2b.c -o $@ $(CFLAGS) -DBLAKE2B_SELFTEST

blake2sp:	blake2sp.c blake2s.c
		$(CC) blake2sp.c blake2s.c -o $@ $(CFLAGS) -DBLAKE2SP_SELFTEST

blake2bp:	blake2bp.c blake2b.c
		$(CC) blake2bp.c blake2b.c -o $@ $(CFLAGS) -DBLAKE2BP_SELFTEST

kat:
		$(CC) $(CFLAGS) -o genkat-c genkat-c.c blake2b.c blake2s.c blake2sp.c blake2bp.c
		$(CC) $(CFLAGS) -g -o genkat-json genkat-json.c blake2b.c blake2s.c blake2sp.c blake2bp.c
		./genkat-c > blake2-kat.h
		./genkat-json > blake2-kat.json

clean:
		rm -rf *.o blake2s blake2b blake2sp blake2bp genkat-c genkat-json

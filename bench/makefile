CC=gcc
# std to gnu99 to support inline asm
CFLAGS=-O3 -march=native -Wall -Wextra -DSUPERCOP # -DHAVE_XOP # uncomment on XOP-enabled CPUs
FILES=bench.c

all: bench

bench: bench.c
	$(CC) $(FILES) $(CFLAGS) ../sse/blake2b.c -o blake2b
	$(CC) $(FILES) $(CFLAGS) ../sse/blake2s.c -o blake2s
	$(CC) $(FILES) $(CFLAGS) md5.c -o md5  -lcrypto -lz

plot: bench
	./blake2b > blake2b.data
	./blake2s > blake2s.data
	./md5 > md5.data
	gnuplot do.gplot

clean:
	rm -f blake2b blake2s md5 plotcycles.pdf blake2b.data blake2s.data md5.data

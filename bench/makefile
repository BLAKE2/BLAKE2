CC=gcc
# std to gnu99 to support inline asm
CFLAGS=-std=gnu99 -O3 -march=native -DSUPERCOP # -DHAVE_XOP # uncomment on XOP-enabled CPUs
FILES=amd64cpuinfo.c bench.c

all:
	$(CC) $(FILES) $(CFLAGS) ../sse/blake2b.c -o blake2b
	$(CC) $(FILES) $(CFLAGS) ../sse/blake2s.c -o blake2s
	$(CC) $(FILES) $(CFLAGS) md5.c -o md5  -lcrypto -lz
	./blake2b > blake2b.data
	./blake2s > blake2s.data
	./md5 > md5.data
	gnuplot do.gplot

clean:
	rm -f blake2b blake2s md5 plotcycles.pdf blake2b.data blake2s.data md5.data

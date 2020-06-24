# BLAKE2 Makefile.am
# Written and placed in public domain by Jeffrey Walton
#
# To use the Autotools files, copy them to the root directory, then:
#
#    autoreconf --verbose --force --install
#    configure
#    make
#    make check
#    make install
#
# The makefile produces three or four installable artifacts:
#
#    * b2sum
#    * blake2-genkat
#    * blake2-genkat-json
#    * blake2-bench (x86 and x64 only)
#
# The check and benchmark programs are built for in-place testing.
# They do not work after installation because they are no wired-up
# for the directory layout after install. In fact the check programs
# are not installed. For example, the check programs will not find the
# test vectors located in $(datarootdir)/b2sum/testvectors. And the
# benchmark program will not find do.gplot in $(libexecdir)/b2sum/.

AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4
AM_DEFAULT_SOURCE_EXT = .c

AM_CPPFLAGS = @AM_CPPFLAGS@
AM_CFLAGS = @AM_CFLAGS@
AM_LDFLAGS = @AM_LDFLAGS@

bin_PROGRAMS = b2sum blake2-genkat blake2-genkat-json
if BLAKE2_BENCH_SOURCES
bin_PROGRAMS += blake2-bench
endif

# Move to bin_PROGRAMS if you want to install them.
check_PROGRAMS = \
	blake2b-check \
	blake2s-check \
	blake2bp-check \
	blake2sp-check \
	blake2xb-check \
	blake2xs-check

# If SSE sources are available, then SSE2, XOP, SSE4 or AVX
# can be used depending on the compiler options used during
# configure and make. Ditto for ARM and NEON.

if BLAKE2_SSE_SOURCES
b2sum_SOURCES = \
	sse/blake2b.c \
	sse/blake2s.c \
	sse/blake2bp.c \
	sse/blake2sp.c \
	sse/blake2xb.c \
	sse/blake2xs.c \
	b2sum-src/b2sum.c
else
if BLAKE2_NEON_SOURCES
b2sum_SOURCES = \
	neon/blake2b.c \
	neon/blake2s.c \
	neon/blake2bp.c \
	neon/blake2sp.c \
	neon/blake2xb.c \
	neon/blake2xs.c \
	b2sum-src/b2sum.c
else
b2sum_SOURCES = \
	ref/blake2b.c \
	ref/blake2s.c \
	ref/blake2bp.c \
	ref/blake2sp.c \
	ref/blake2xb.c \
	ref/blake2xs.c \
	b2sum-src/b2sum.c
endif # BLAKE2_NEON_SOURCES
endif # BLAKE2_SSE_SOURCES

# The KAT generators are built with the reference implementation
# to avoid carrying an ISA implementation error into the KATs.

blake2_genkat_SOURCES = \
	ref/blake2b.c \
	ref/blake2s.c \
	ref/blake2bp.c \
	ref/blake2sp.c \
	ref/blake2xb.c \
	ref/blake2xs.c \
	ref/genkat-c.c

blake2_genkat_json_SOURCES = \
	ref/blake2b.c \
	ref/blake2s.c \
	ref/blake2bp.c \
	ref/blake2sp.c \
	ref/blake2xb.c \
	ref/blake2xs.c \
	ref/genkat-json.c

blake2b_check_CPPFLAGS  = $(AM_CPPFLAGS) -DBLAKE2B_SELFTEST=1
blake2s_check_CPPFLAGS  = $(AM_CPPFLAGS) -DBLAKE2S_SELFTEST=1
blake2bp_check_CPPFLAGS = $(AM_CPPFLAGS) -DBLAKE2BP_SELFTEST=1
blake2sp_check_CPPFLAGS = $(AM_CPPFLAGS) -DBLAKE2SP_SELFTEST=1
blake2xb_check_CPPFLAGS = $(AM_CPPFLAGS) -DBLAKE2XB_SELFTEST=1
blake2xs_check_CPPFLAGS = $(AM_CPPFLAGS) -DBLAKE2XS_SELFTEST=1

blake2b_check_SOURCES  = check-src/blake2b.c
blake2s_check_SOURCES  = check-src/blake2s.c
blake2bp_check_SOURCES = check-src/blake2bp.c check-src/blake2b.c
blake2sp_check_SOURCES = check-src/blake2sp.c check-src/blake2s.c
blake2xb_check_SOURCES = check-src/blake2xb.c check-src/blake2b.c
blake2xs_check_SOURCES = check-src/blake2xs.c check-src/blake2s.c

testvector_FILES = \
	testvectors/blake2b-kat.txt \
	testvectors/blake2bp-kat.txt \
	testvectors/blake2-kat.json \
	testvectors/blake2s-kat.txt \
	testvectors/blake2sp-kat.txt

# Uncomment if you want to install them.
#nobase_dist_pkgdata_DATA = \
#    $(testvector_FILES)

# The benchmark program is only available on x86 with a hires timer.
# The benchmark program needs OpenSSL for a compare/contrast with MD5.
if BLAKE2_BENCH_SOURCES
blake2_bench_CPPFLAGS = $(AM_CPPFLAGS) $(OPENSSL_CPPFLAGS)
blake2_bench_CFLAGS = $(AM_CFLAGS) $(OPENSSL_CFLAGS)
blake2_bench_LDFLAGS = $(AM_LDFLAGS) $(OPENSSL_LDFLAGS)
blake2_bench_LDADD = $(OPENSSL_LIBS)

blake2_bench_SOURCES = \
	bench/md5.c \
	bench/bench.c
endif # BLAKE2_BENCH_SOURCES

# Documentation
man1_MANS = b2sum-src/b2sum.1

check test:
	./blake2b-check
	./blake2s-check
	./blake2bp-check
	./blake2sp-check
	./blake2xb-check
	./blake2xs-check

# A custom recipe to audit the binary
audit: b2sum
	@if ( test -n `command -v checksec` ); then \
	checksec --file=b2sum; \
	else \
	echo "Please install checksec to audit the binary"; \
	fi

## Automake does not properly uninstall the test vectors
uninstall-hook:
	-rm -rf ${pkgdatadir}

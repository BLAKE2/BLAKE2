# BLAKE2 configure.ac
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
#    * blake2-bench (on x86 only)
#
# There have been two GitHub releases of the BLAKE2 software: 20160619.tar.gz
# and 20190724.tar.gz. The BLAKE2 versions are as follows for this configure.ac.
# The versions may be completely wrong.
#
#    * 20160619 is version 1.0 (blake2b, blake2s, blake2bp, blake2sp)
#    * 20190724 is version 2.0 (add blake2xb and blake2xs)
#

# Should we use the BLAKE2 or b2sum as the package name and tarball name here?
# Should we use the release version here, like 20190724, instead of 2.0?
# https://www.gnu.org/software/autoconf/manual/autoconf-2.67/html_node/Initializing-configure.html
AC_INIT([b2sum], [2.0], [https://github.com/BLAKE2/BLAKE2/issues], [b2sum], [https://blake2.net/])

# Safety checks in case user overwritten --srcdir
AC_CONFIG_SRCDIR([])

# Store the auxiliary build tools, like install-sh, config.sub
# and config.guess, in the build-aux directory
AC_CONFIG_AUX_DIR([build-aux])

# We distribute an updated pkg.m4 for older machines, like PowerMac.
# Unfortunately, AC_CONFIG_MACRO_DIRS breaks those old machines.
# AC_CONFIG_MACRO_DIRS([m4])

# Parse the Host triplet. The Host is the one the package runs on.
AC_CANONICAL_HOST
# Parse the Build triplet. The Build is the machine that does the compiling.
AC_CANONICAL_BUILD

# Set the language
AC_LANG([C])
# LT_LANG([C])

# Init automake, and specify this program use relaxed structures.
# i.e. this program doesn't follow the gnu coding standards, and doesn't have
# ChangeLog, COPYING, AUTHORS, INSTALL, README etc. files.
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

# Tell Automake to create a Makefile
# See https://www.gnu.org/software/automake/manual/html_node/Requirements.html
AC_CONFIG_FILES([Makefile])

# Autoconf tests
AC_PROG_CC
AC_PROG_SED
AC_PROG_GREP
AC_PROG_EGREP
AC_PROG_MKDIR_P
PKG_PROG_PKG_CONFIG

# https://ilgthegeek.wordpress.com/2015/02/05/pkg-config-search-path/
AS_IF([test "$build" != "$host"] && [test "x$PKG_CONFIG_LIBDIR" = "x"],
    AC_MSG_WARN(["PKG_CONFIG_LIBDIR should be set for cross-compiles"])
)

# Enable -Wall if not present in flags and available
has_wall=`echo "$AM_CPPFLAGS $CPPFLAGS $AM_CFLAGS $CFLAGS" | $GREP -i -c '\-Wall'`
AS_IF([test "$has_wall" -eq 0],
[
    saved_CFLAGS="$CFLAGS"
    CFLAGS="-Wall"
    AC_MSG_CHECKING([whether compiler supports -Wall])
    AC_COMPILE_IFELSE(
       [AC_LANG_PROGRAM([])],
       [AM_CFLAGS="$AM_CFLAGS -Wall"
        AC_MSG_RESULT([yes])
       ],
       [AC_MSG_RESULT([no])]
    )
    CFLAGS="$saved_CFLAGS"
])

# If a user specifies --enable-native, then try to find -march=native or equivalent.
# The default is OFF for distros. Distros build and distribute for a minimal machine.
AC_ARG_ENABLE([native],
    AS_HELP_STRING([--enable-native], [Enable native machine option]))

AS_IF([test "x$enable_native" = "xyes"],
[
    saved_CFLAGS="$CFLAGS"
    AS_IF([test "x$blake2_native" = "x"],
    [
       CFLAGS="-march=native"
       AC_MSG_CHECKING([whether compiler supports -march=native])
       AC_COMPILE_IFELSE(
           [AC_LANG_PROGRAM([])],
           [AM_CFLAGS="$AM_CFLAGS -march=native"
            AC_SUBST([blake2_native],["-march=native"])
            AC_MSG_RESULT([yes])
           ],
           [AC_MSG_RESULT([no])]
       )
    ])
    AS_IF([test "x$blake2_native" = "x"],
    [
       CFLAGS="-mtune=native"
       AC_MSG_CHECKING([whether compiler supports -mtune=native])
       AC_COMPILE_IFELSE(
           [AC_LANG_PROGRAM([])],
           [AM_CFLAGS="$AM_CFLAGS -mtune=native"
            AC_SUBST([blake2_native],["-mtune=native"])
            AC_MSG_RESULT([yes])
           ],
           [AC_MSG_RESULT([no])]
       )
    ])
    AS_IF([test "x$blake2_native" = "x"],
    [
       CFLAGS="-native"
       AC_MSG_CHECKING([whether compiler supports -native])
       AC_COMPILE_IFELSE(
           [AC_LANG_PROGRAM([])],
           [AM_CFLAGS="$AM_CFLAGS -native"
            AC_SUBST([blake2_native],["-native"])
            AC_MSG_RESULT([yes])
           ],
           [AC_MSG_RESULT([no])]
       )
    ])
    AS_IF([test "x$blake2_native" = "x"],
        [AC_SUBST([blake2_native],["not found"])]
    )
    CFLAGS="$saved_CFLAGS"
])

# Default value for $blake2_native if unset
AS_IF([test "x$blake2_native" = "x"],
    [AC_SUBST([blake2_native],["not enabled"])]
)

# armv8 is included due to Clang and armv8l-*-gnueabihf,
# which appears to be non-aarch32 32-bit OS on aarch64.
case "$host" in
armv7*-* | armv8*-*)
    # If a user specifies --enable-armv7, then use -march=armv7.
    # The default is OFF for distros. Distros build and distribute for a minimal machine.
    AC_ARG_ENABLE([armv7],
        AS_HELP_STRING([--enable-armv7], [Enable armv7 machine option on ARM]))

    AS_IF([test "x$enable_armv7" = "xyes"],
        [AM_CFLAGS="$AM_CFLAGS -march=armv7"]
    )

    # If a user specifies --enable-neon, then use -march=neon.
    # The default is OFF for distros. Distros build and distribute for a minimal machine.
    AC_ARG_ENABLE([neon],
        AS_HELP_STRING([--enable-neon], [Enable neon fpu option on ARM]))

    AS_IF([test "x$enable_neon" = "xyes"],
        [AM_CFLAGS="$AM_CFLAGS -mfpu=neon"]
    )
    ;;
aarch32-* | aarch64-*)
    # If a user specifies --enable-asimd, then use NEON sources on aarch32 and aarch64.
    # NEON is slower than C on Cortex A-53 processors. Cortex A-57 processors should be OK.
    # The default is OFF for distros. Distros build and distribute for a minimal machine.
    AC_ARG_ENABLE([asimd],
        AS_HELP_STRING([--enable-asimd], [Enable asimd option on ARM64]))
    ;;
*)
    ;;
esac

# If -mfpu=neon is specified, then set $our_host to the neon triplet.
has_neon=`echo "$AM_CPPFLAGS $CPPFLAGS $AM_CFLAGS $CFLAGS" | $GREP -i -c '\-mfpu=neon'`
AS_IF([test "$has_neon" -eq 0],
    [our_host="$host"],
    [our_host="neon-neon-neon"]
)

# Select SSE versus NEON versus C implementation.
case "$our_host" in
i686-*)
    AC_SUBST([blake2_sse_sources], [true])
    AM_CPPFLAGS="$AM_CPPFLAGS -I./sse"
    AM_CFLAGS="-msse2"
    ;;
amd64-* | x86_64-*)
    AC_SUBST([blake2_sse_sources], [true])
    AM_CPPFLAGS="$AM_CPPFLAGS -I./sse"
    # SSE2 is part of base ISA
    ;;
neon-*)
    AC_SUBST([blake2_neon_sources], [true])
    AM_CPPFLAGS="$AM_CPPFLAGS -I./neon"
    ;;
armv7*-* | armv8*-*)
    # NEON is optional for armv6 and armv7. armv8 is
    # included due to Clang and armv8l-*-gnueabihf, which
    # appears to be non-aarch32 32-bit OS on aarch64.
    AM_CPPFLAGS="$AM_CPPFLAGS -I./ref"
    ;;
aarch32-* | aarch64-*)
    # NEON is called ASIMD and part of base ISA, but
    # ASIMD on Cortex A-53 is slower then straight C.
    # Users must specify --enable-asimd to engage NEON.
    AS_IF([test "x$enable_asimd" = "xyes"],
        [AC_SUBST([blake2_neon_sources], [true])
         AM_CPPFLAGS="$AM_CPPFLAGS -I./neon"],
        [AC_SUBST([blake2_neon_sources], [false])
         AM_CPPFLAGS="$AM_CPPFLAGS -I./ref"]
    )
    ;;
*)
    # Use reference implementation
    AM_CPPFLAGS="$AM_CPPFLAGS -I./ref"
    ;;
esac

# Default value for $blake2_sse_sources if unset
AS_IF([test "x$blake2_sse_sources" = "x"],
    [AC_SUBST([blake2_sse_sources],[false])]
)
# Default value for $blake2_neon_sources if unset
AS_IF([test "x$blake2_neon_sources" = "x"],
    [AC_SUBST([blake2_neon_sources],[false])]
)

# These are for the BSDs. Legendary for security, but missing secure flags.
saved_CFLAGS="$CFLAGS"
CFLAGS="-fstack-protector-all"
AC_MSG_CHECKING([whether compiler supports -fstack-protector-all])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([])],
    [AM_CFLAGS="$AM_CFLAGS -fstack-protector-all"; AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])
     # Try with just -fstack-protector
     CFLAGS="-fstack-protector"
     AC_MSG_CHECKING([whether compiler supports -fstack-protector])
     AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM([])],
        [AM_CFLAGS="$AM_CFLAGS -fstack-protector"; AC_MSG_RESULT([yes])],
        [AC_MSG_RESULT([no])]
     )
    ]
)
CFLAGS="-fstack-clash-protection"
AC_MSG_CHECKING([whether compiler supports -fstack-clash-protection])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([])],
    [AM_CFLAGS="$AM_CFLAGS -fstack-clash-protection"; AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])]
)
CFLAGS="-mcet -fcf-protection"
AC_MSG_CHECKING([whether compiler supports -mcet -fcf-protection])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([])],
    [AM_CFLAGS="$AM_CFLAGS -mcet -fcf-protection"; AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])]
)
CFLAGS="-fplugin=annobin"
AC_MSG_CHECKING([whether compiler supports -fplugin=annobin])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([])],
    [AM_CFLAGS="$AM_CFLAGS -fplugin=annobin"; AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])]
)
CFLAGS="-fexceptions"
AC_MSG_CHECKING([whether compiler supports -fexceptions])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([])],
    [AM_CFLAGS="$AM_CFLAGS -fexceptions"; AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])]
)
CFLAGS="-fPIC"
AC_MSG_CHECKING([whether compiler supports -fPIC])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([])],
    [
        has_pic=`echo "$AM_CFLAGS $saved_CFLAGS" | $EGREP -i -c '\-kPIC|\-fPIC|\-fPIE'`
        AS_IF([test "$has_pic" -eq 0],
            [AM_CFLAGS="$AM_CFLAGS -fPIC"]
        )
        AC_MSG_RESULT([yes])
    ],
    [AC_MSG_RESULT([no])]
)
CFLAGS="$saved_CFLAGS"

# These are for the BSDs. Legendary for security, but missing secure flags.
saved_LDFLAGS="$LDFLAGS"
LDFLAGS="-pie"
AC_MSG_CHECKING([whether linker supports -pie])
AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([])],
    [
        has_pie=`echo "$AM_LDFLAGS $saved_LDFLAGS" | $GREP -i -c '\-pie'`
        AS_IF([test "$has_pie" -eq 0],
            [AM_LDFLAGS="$AM_LDFLAGS -pie"]
        )
        AC_MSG_RESULT([yes])
    ],
    [AC_MSG_RESULT([no])]
)
LDFLAGS="-Wl,-z,relro"
AC_MSG_CHECKING([whether linker supports -Wl,-z,relro])
AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([])],
    [AM_LDFLAGS="$AM_LDFLAGS -Wl,-z,relro"; AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])]
)
LDFLAGS="-Wl,-z,now"
AC_MSG_CHECKING([whether linker supports -Wl,-z,now])
AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([])],
    [AM_LDFLAGS="$AM_LDFLAGS -Wl,-z,now"; AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])]
)
LDFLAGS="-Wl,-z,defs"
AC_MSG_CHECKING([whether linker supports -Wl,-z,defs])
AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([])],
    [AM_LDFLAGS="$AM_LDFLAGS -Wl,-z,defs"; AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])]
)
LDFLAGS="-Wl,-z,noexecstack"
AC_MSG_CHECKING([whether linker supports -Wl,-z,noexecstack])
AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([])],
    [AM_LDFLAGS="$AM_LDFLAGS -Wl,-z,noexecstack"; AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])]
)
LDFLAGS="$saved_LDFLAGS"

# And keep the binary tidy
saved_LDFLAGS="$LDFLAGS"
LDFLAGS="-Wl,--as-needed"
AC_MSG_CHECKING([whether linker supports -Wl,--as-needed])
AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([])],
    [AM_LDFLAGS="$AM_LDFLAGS -Wl,--as-needed"; AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])]
)
LDFLAGS="$saved_LDFLAGS"

# Benchmarks are only built on x86 if OpenSSL is available.
# The benchmarks can be disabled on x86 with --disable-benchmark.
AC_ARG_ENABLE([benchmark],
    AS_HELP_STRING([--disable-benchmark], [Disable benchmarks on x86 machines]))

AS_IF([test "x$enable_benchmark" != "xno"],
[
    case "$host" in
    i386-* | i486-* | i586-* | i686-* | i86pc-* | amd64-* | x86_64-*)
        # We provide an updated PKG_CHECK_MODULES via pkg.m4.
        # The updated pkg.m4 is needed on PowerMacs.
        PKG_CHECK_MODULES(OPENSSL, libcrypto >= 1.0,
            [AC_SUBST([blake2_bench_sources],[true])],
            [AC_MSG_NOTICE(["OpenSSL not found. Disabling benchmark program"])
             AC_SUBST([blake2_bench_sources],[false])])
        ;;
    *)
        ;;
    esac
])

# Default value for $blake2_bench_sources if unset
AS_IF([test "x$blake2_bench_sources" = "x"],
    [AC_SUBST([blake2_bench_sources],[false])]
)

# This greatly simplifies building programs and test programs
AS_IF([test -f ref/blake2b-ref.c],
    [AC_MSG_NOTICE([renaming ref/blake2b-ref.c to blake2b.c])
     mv ref/blake2b-ref.c ref/blake2b.c]
)
AS_IF([test -f ref/blake2s-ref.c],
    [AC_MSG_NOTICE([renaming ref/blake2s-ref.c to blake2s.c])
     mv ref/blake2s-ref.c ref/blake2s.c]
)
AS_IF([test -f ref/blake2bp-ref.c],
    [AC_MSG_NOTICE([renaming ref/blake2bp-ref.c to blake2bp.c])
     mv ref/blake2bp-ref.c ref/blake2bp.c]
)
AS_IF([test -f ref/blake2sp-ref.c],
    [AC_MSG_NOTICE([renaming ref/blake2sp-ref.c to blake2sp.c])
     mv ref/blake2sp-ref.c ref/blake2sp.c]
)
AS_IF([test -f ref/blake2xb-ref.c],
    [AC_MSG_NOTICE([renaming ref/blake2xb-ref.c to blake2xb.c])
     mv ref/blake2xb-ref.c ref/blake2xb.c]
)
AS_IF([test -f ref/blake2xs-ref.c],
    [AC_MSG_NOTICE([renaming ref/blake2xs-ref.c to blake2xs.c])
     mv ref/blake2xs-ref.c ref/blake2xs.c]
)

# Remove unneeded makefiles. Autotools makefile does all the work.
AS_IF([test -f ref/makefile],
    [AC_MSG_NOTICE([removing ref/makefile])
     rm -f ref/makefile]
)
AS_IF([test -f sse/makefile],
    [AC_MSG_NOTICE([removing sse/makefile])
     rm -f sse/makefile]
)
AS_IF([test -f neon/makefile],
    [AC_MSG_NOTICE([removing neon/makefile])
     rm -f neon/makefile]
)
AS_IF([test -f b2sum/makefile],
    [AC_MSG_NOTICE([removing b2sum/makefile])
     rm -f b2sum/makefile]
)
AS_IF([test -f bench/makefile],
    [AC_MSG_NOTICE([removing bench/makefile])
     rm -f bench/makefile]
)

# b2sum/ is where the program sources are located. b2sum is also the name of
# the output program. Automake needs the directory moved to a different name.
AS_IF([test -d b2sum/],
    [AC_MSG_NOTICE([moving b2sum/ to b2sum-src/])
     mv b2sum/ b2sum-src/]
)

# The test programs are baked into the sources, like ref/blake2b.c, and
# activated for a #define. We are going to create a check-src/ directory,
# and copy the sources into them. Then we can add a first class tests.
AS_IF([test ! -d check-src/],
    [AC_MSG_NOTICE([creating check-src/])
     $MKDIR_P check-src/])

# Populate check-src/.
AC_MSG_NOTICE([populating check-src/])
AS_IF([test "x$blake2_sse_sources" = "xtrue"],
        [cp sse/*.h sse/*.c check-src/],
    [test "x$blake2_neon_sources" = "xtrue"],
        [cp neon/*.h neon/*.c check-src/],
    [cp ref/*.h ref/*.c check-src/])
cp testvectors/blake2-kat.h check-src/
rm -f check-src/genkat*

# Cleanup whitespace
AM_CPPFLAGS=`echo "$AM_CPPFLAGS" | $SED 's/^[ \t]*//'`
AM_CFLAGS=`echo "$AM_CFLAGS" | $SED 's/^[ \t]*//'`
AM_LDFLAGS=`echo "$AM_LDFLAGS" | $SED 's/^[ \t]*//'`

# Set our flags for the Makefile
AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_LDFLAGS])

# Enable source files in the Makefile
AM_CONDITIONAL([BLAKE2_SSE_SOURCES], [$blake2_sse_sources])
AM_CONDITIONAL([BLAKE2_NEON_SOURCES], [$blake2_neon_sources])
AM_CONDITIONAL([BLAKE2_BENCH_SOURCES], [$blake2_bench_sources])

# Generate the output
AC_OUTPUT

# Print a summary
printf "\n"
printf "****************************************\n"
printf "  Target: %s\n" "$host"
printf "  Native: %s\n" "$blake2_native"
printf " SSE src: %s\n" "$blake2_sse_sources"
printf "NEON src: %s\n" "$blake2_neon_sources"
printf "****************************************\n"
printf "\n"

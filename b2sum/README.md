# b2sum #

This is a simple tool to create BLAKE2 hashes of files or streams. For more usage information refer to the manpage, e.g., `man b2sum.1`.

## Building and Installing ##

To build `b2sum`, just run `make`. To install, run `make install`. The install path can be altered using the `PREFIX` variable, which defaults to `/usr/local`. Use `PREFIX=/some/path make install` to install somewhere else.

`b2sum` makes use of OpenMP by default. Some compilers do not support this technology, in which case `b2sum` can be built without it using the `NO_OPENMP` variable. Use `NO_OPENMP=1 make` to build without OpenMP.

For OS X, there is a prebuilt [Homebrew formula](https://github.com/Homebrew/homebrew-core/commit/e016cda47dfa298c6628de3e9d0fd976eecd91be) which may be installed with `brew install b2sum`.


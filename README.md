# BLAKE2

This is the reference source code package of BLAKE2, which includes

* `ref/`: C implementations of BLAKE2b, BLAKE2Bp, BLAKE2s, BLAKE2Sp,
  aimed at portability and simplicity. 

* `sse/`: C implementations of BLAKE2b, BLAKE2Bp, BLAKE2s, BLAKE2Sp,
  optimized for speed on CPUs supporting SSE2, SSSE3, SSE4.1, AVX, or
  XOP.

* `csharp/`: C# implementation of BLAKE2b. 

* `b2sum/`: Command line utility to hash files, based on the `sse/`
  implementations.

* `bench/`: Benchmark tool to measure cycles-per-byte speeds and produce
  graphs copyright.

All code is licensed under CC0.

Contact: contact@blake2.net

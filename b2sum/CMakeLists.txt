set(B2_COMPILE_OPTIONS -O3 -Wall -Wextra -Wno-long-long -march=native)

add_executable(b2sum "")

target_link_libraries(b2sum PRIVATE blake2b blake2s blake2sp blake2bp)

target_sources(b2sum
    PRIVATE
		b2sum.c
)

target_compile_options(b2sum PRIVATE ${B2_COMPILE_OPTIONS} )
target_compile_definitions(b2sum PRIVATE _POSIX_C_SOURCE=2)


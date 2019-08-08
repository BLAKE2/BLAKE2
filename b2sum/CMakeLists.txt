set(B2_COMPILE_OPTIONS -O3 -Wall -Wextra -Wno-long-long -march=native)

add_compile_options( ${B2_COMPILE_OPTIONS} )

add_executable(b2sum)

target_sources(b2sum
    PRIVATE
		b2sum.c
)

target_link_libraries(b2sum PRIVATE blake2b blake2s blake2sp blake2bp)

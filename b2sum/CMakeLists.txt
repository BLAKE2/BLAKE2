set(B2_COMPILE_OPTIONS -O3 -Wall -Wextra -Wno-long-long -march=native)

add_compile_options( ${B2_COMPILE_OPTIONS} )

add_executable(b2sum "")

target_link_libraries(b2sum PRIVATE blake2b blake2s blake2sp blake2bp)

target_sources(b2sum
    PRIVATE
		b2sum.c
)

install(TARGETS b2sum RUNTIME DESTINATION bin ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)

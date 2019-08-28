set(B2_COMPILE_OPTIONS -O3 -Wall -Wextra -Wno-long-long -march=native)

add_compile_options( ${B2_COMPILE_OPTIONS} )

add_executable(b2sum "")

target_link_libraries(b2sum PRIVATE blake2b blake2s blake2sp blake2bp)

target_sources(b2sum
    PRIVATE
		b2sum.c
)

install(TARGETS b2sum
EXPORT b2sum-targets
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} 
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 
PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/b2sum
)

install( EXPORT b2sum-targets
		 NAMESPACE blake2::
		 DESTINATION ${CMAKE_INSTALL_BINDIR}/cmake/b2sum
)
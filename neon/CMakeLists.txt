set(B2_COMPILE_OPTIONS -march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard -O3 ${COMMON_C_FLAGS})

add_compile_options( ${B2_COMPILE_OPTIONS} )

target_include_directories(blake2b  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(blake2s  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(blake2sp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(blake2xb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(blake2bp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(blake2xs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )

target_sources(blake2b
    PRIVATE
		blake2b.c
		blake2b-round.h
		blake2-impl.h
		blake2b-load-neon.h
	PUBLIC
		blake2.h
)

target_sources(blake2s
    PRIVATE
		blake2s.c
		blake2s-round.h
		blake2-impl.h
		blake2s-load-neon.h
	PUBLIC
		blake2.h
)

target_sources(blake2sp
    PRIVATE
		blake2sp.c
		blake2s.c
		blake2s-round.h
		blake2-impl.h
		blake2s-load-neon.h
	PUBLIC
		blake2.h
)

target_compile_options(blake2sp PRIVATE -fopenmp)

target_sources(blake2xb
    PRIVATE
		blake2xb.c
		blake2b.c
		blake2b-round.h
		blake2-impl.h
		blake2b-load-neon.h
	PUBLIC
		blake2.h
)

target_sources(blake2bp
    PRIVATE
		blake2bp.c
		blake2b.c
		blake2b-round.h
		blake2-impl.h
		blake2b-load-neon.h
	PUBLIC
		blake2.h
)

target_compile_options(blake2bp PRIVATE -fopenmp)

target_sources(blake2xs
    PRIVATE
		blake2xs.c
		blake2s.c
		blake2s-round.h
		blake2-impl.h
		blake2s-load-neon.h
	PUBLIC
		blake2.h
)

set(B2_COMPILE_OPTIONS -O2 ${COMMON_C_FLAGS})

add_compile_options( ${B2_COMPILE_OPTIONS} )

target_include_directories(blake2b  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(blake2s  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(blake2sp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(blake2xb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(blake2bp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(blake2xs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )

target_sources(blake2b
    PRIVATE
		blake2b-ref.c
		blake2-impl.h
	PUBLIC
		blake2.h
)

target_sources(blake2s
    PRIVATE
		blake2s-ref.c
		blake2-impl.h
	PUBLIC
		blake2.h
)

target_sources(blake2sp
    PRIVATE
		blake2sp-ref.c
		blake2s-ref.c
		blake2-impl.h
	PUBLIC
		blake2.h
)

target_sources(blake2xb
    PRIVATE
		blake2xb-ref.c
		blake2b-ref.c
		blake2-impl.h
	PUBLIC
		blake2.h
)

target_sources(blake2bp
    PRIVATE
		blake2bp-ref.c
		blake2b-ref.c
		blake2-impl.h
	PUBLIC
		blake2.h
)

target_sources(blake2xs
    PRIVATE
		blake2xs-ref.c
		blake2s-ref.c
		blake2-impl.h
	PUBLIC
		blake2.h
)

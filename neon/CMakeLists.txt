set(B2_COMPILE_OPTIONS -march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard -O3 ${COMMON_C_FLAGS})

add_compile_options( ${B2_COMPILE_OPTIONS} )

# Build executables with check definition and target for running them if -DCHECK_BUILD=1 is passed
if( BUILD_TESTING )
	include_directories(../testvectors)
endif()

foreach(BLAKE2_TARGET IN LISTS BLAKE2_TARGETS)
	if( BUILD_TESTING )
		add_executable(${BLAKE2_TARGET} "")
		string(TOUPPER "${BLAKE2_TARGET}" BLAKE2_TARGET_UPPER)
		target_compile_definitions(${BLAKE2_TARGET}  PRIVATE ${BLAKE2_TARGET_UPPER}_SELFTEST )
	else()
		add_library(${BLAKE2_TARGET} "")
	endif()
	
	target_include_directories(${BLAKE2_TARGET}  
		PUBLIC 
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
			$<INSTALL_INTERFACE:include>
	)
	set_target_properties(${BLAKE2_TARGET} PROPERTIES PUBLIC_HEADER "${CMAKE_CURRENT_LIST_DIR}/blake2.h")
endforeach()


target_sources(blake2b
    PRIVATE
		blake2b.c
		blake2b-round.h
		blake2-impl.h
		blake2b-load-neon.h
	#Headers
		${CMAKE_CURRENT_LIST_DIR}/blake2.h
)

target_sources(blake2s
    PRIVATE
		blake2s.c
		blake2s-round.h
		blake2-impl.h
		blake2s-load-neon.h
	#Headers
		${CMAKE_CURRENT_LIST_DIR}/blake2.h
)

target_sources(blake2sp
    PRIVATE
		blake2sp.c
		blake2s.c
		blake2s-round.h
		blake2-impl.h
		blake2s-load-neon.h
	#Headers
		${CMAKE_CURRENT_LIST_DIR}/blake2.h
)

target_compile_options(blake2sp PRIVATE -fopenmp)

target_sources(blake2xb
    PRIVATE
		blake2xb.c
		blake2b.c
		blake2b-round.h
		blake2-impl.h
		blake2b-load-neon.h
	#Headers
		${CMAKE_CURRENT_LIST_DIR}/blake2.h
)

target_sources(blake2bp
    PRIVATE
		blake2bp.c
		blake2b.c
		blake2b-round.h
		blake2-impl.h
		blake2b-load-neon.h
	#Headers
		${CMAKE_CURRENT_LIST_DIR}/blake2.h
)

target_compile_options(blake2bp PRIVATE -fopenmp)

target_sources(blake2xs
    PRIVATE
		blake2xs.c
		blake2s.c
		blake2s-round.h
		blake2-impl.h
		blake2s-load-neon.h
	#Headers
		${CMAKE_CURRENT_LIST_DIR}/blake2.h
)
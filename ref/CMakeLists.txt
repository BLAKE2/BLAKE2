set(B2_COMPILE_OPTIONS -O2 ${COMMON_C_FLAGS})

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
		blake2b-ref.c
		blake2-impl.h
	#Headers
		${CMAKE_CURRENT_LIST_DIR}/blake2.h
)

target_sources(blake2s
    PRIVATE
		blake2s-ref.c
		blake2-impl.h
	#Headers
		${CMAKE_CURRENT_LIST_DIR}/blake2.h
)

target_sources(blake2sp
    PRIVATE
		blake2sp-ref.c
		blake2s-ref.c
		blake2-impl.h
	#Headers
		${CMAKE_CURRENT_LIST_DIR}/blake2.h
)

target_sources(blake2xb
    PRIVATE
		blake2xb-ref.c
		blake2b-ref.c
		blake2-impl.h
	#Headers
		${CMAKE_CURRENT_LIST_DIR}/blake2.h
)

target_sources(blake2bp
    PRIVATE
		blake2bp-ref.c
		blake2b-ref.c
		blake2-impl.h
	#Headers
		${CMAKE_CURRENT_LIST_DIR}/blake2.h
)

target_sources(blake2xs
    PRIVATE
		blake2xs-ref.c
		blake2s-ref.c
		blake2-impl.h
	#Headers
		${CMAKE_CURRENT_LIST_DIR}/blake2.h
)
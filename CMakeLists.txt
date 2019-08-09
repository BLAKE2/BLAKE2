macro(enableXOP)
	if ( XOP_ENABLED )	
		check_c_compiler_flag(-mxop   XOP_SUPPORTED)
		if( XOP_SUPPORTED )
			set(COMMON_C_FLAGS ${COMMON_C_FLAGS} -mxop)
			message(STATUS "Enabled XOP")
		else()
			message(FATAL_ERROR "XOP not supported by compiler!")
		endif()
	endif()
endmacro()

# Enable new versioning (old might be depracated)
cmake_policy(SET CMP0048 NEW)

project(blake2 LANGUAGES C)

# Set minimum global cmake version
cmake_minimum_required(VERSION 3.14.0)

# Requires no extension and C90 standard
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Set flags used in all builds
set(COMMON_C_FLAGS -Wall -Wextra -Wno-long-long -pedantic)

# Check if the compiler supports SSE or NEON flags
include(CheckCCompilerFlag)

# Set allowed arguments
set(ISA_EXTENSION "Best" CACHE STRING "Instruction set to be used")
set_property(
    CACHE ISA_EXTENSION
    PROPERTY STRINGS
    "Best SSE2 SSSE3 SSE4.1 AVX AVX2 Neon None"
)

set(XOP_ENABLED ON CACHE BOOL "Adds XOP extension")

# Check for user ISA extension setting
if ( ISA_EXTENSION STREQUAL "SSE2" )
	check_c_compiler_flag(-msse2  SSE2_SUPPORTED)
	if ( SSE2_SUPPORTED )
		set(COMMON_C_FLAGS ${COMMON_C_FLAGS} -msse2)
		set( SSE_BUILD ON )
		message(STATUS "Selected SSE2")
		enableXOP()
	else()
		message(FATAL_ERROR "SSE2 not supported by compiler!")
	endif()
elseif ( ISA_EXTENSION STREQUAL "SSSE3" )
	check_c_compiler_flag(-mssse3 SSSE3_SUPPORTED)
	if ( SSSE3_SUPPORTED )
		set(COMMON_C_FLAGS ${COMMON_C_FLAGS} -mssse3)
		set( SSE_BUILD ON )
		message(STATUS "Selected SSSE3")
		enableXOP()
	else()
		message(FATAL_ERROR "SSSE2 not supported by compiler!")
	endif()
elseif ( ISA_EXTENSION STREQUAL "SSE4.1" )
	check_c_compiler_flag(-msse4.1 SSE41_SUPPORTED)
	if ( SSE41_SUPPORTED )
		set(COMMON_C_FLAGS ${COMMON_C_FLAGS} -msse4.1)
		set( SSE_BUILD ON )
		message(STATUS "Selected SSE4.1")
		enableXOP()
	else()
		message(FATAL_ERROR "SSE41 not supported by compiler!")
	endif()
elseif ( ISA_EXTENSION STREQUAL "AVX" )
	check_c_compiler_flag(-mavx   AVX_SUPPORTED)
	if ( AVX_SUPPORTED )
		set(COMMON_C_FLAGS ${COMMON_C_FLAGS} -mavx)
		set( SSE_BUILD ON )
		message(STATUS "Selected AVX")
		enableXOP()
	else()
		message(FATAL_ERROR "AVX not supported by compiler!")
	endif()
elseif ( ISA_EXTENSION STREQUAL "AVX2" )
	check_c_compiler_flag(-mavx2  AVX2_SUPPORTED)
	if ( AVX2_SUPPORTED )
		set(COMMON_C_FLAGS ${COMMON_C_FLAGS} -mavx2)
		set( SSE_BUILD ON )
		message(STATUS "Selected AVX2")
		enableXOP()
	else()
		message(FATAL_ERROR "AVX2 not supported by compiler!")
	endif()
elseif ( ISA_EXTENSION STREQUAL "Neon" )
	check_c_compiler_flag(-mfpu=neon-vfpv4 NEON_SUPPORTED)
	if ( NEON_SUPPORTED )
		set( NEON_BUILD ON )
		message(STATUS "Selected Neon")
	else()
		message(FATAL_ERROR "NEON not supported by compiler!")
	endif()
elseif ( ISA_EXTENSION STREQUAL "None" )
	set(REF_BUILD ON )
	message(STATUS "Selected reference")
elseif( ISA_EXTENSION STREQUAL "Best" )
	message(STATUS "Finding best ISA extension...")
	
	check_c_compiler_flag(-msse2  SSE2_SUPPORTED)
	check_c_compiler_flag(-mssse3 SSSE3_SUPPORTED)
	check_c_compiler_flag(-msse4.1 SSE41_SUPPORTED)
	check_c_compiler_flag(-mavx   AVX_SUPPORTED)
	check_c_compiler_flag(-mavx2  AVX2_SUPPORTED)
	check_c_compiler_flag(-mxop   XOP_SUPPORTED)
	check_c_compiler_flag(-mfpu=neon-vfpv4 NEON_SUPPORTED)
	
	if ( SSE2_SUPPORTED AND NEON_SUPPORTED )
		message(FATAL_ERROR "Cannot select best as SSE and NEON are available!")
	endif()
	
	if (NEON_SUPPORTED)
		set(NEON_BUILD ON)
		message(STATUS "Selected Neon")
	elseif( AVX2_SUPPORTED )
		set(COMMON_C_FLAGS ${COMMON_C_FLAGS} -mavx2)
		set( SSE_BUILD ON )
		message(STATUS "Selected AVX2")
		enableXOP()
	elseif( AVX_SUPPORTED )
		set(COMMON_C_FLAGS ${COMMON_C_FLAGS} -mavx)
		set( SSE_BUILD ON )
		message(STATUS "Selected AVX")
		enableXOP()
	elseif( SSE41_SUPPORTED )
		set(COMMON_C_FLAGS ${COMMON_C_FLAGS} -msse4.1)
		set( SSE_BUILD ON )
		message(STATUS "Selected SSE4.1")
		enableXOP()
	elseif( SSSE3_SUPPORTED )
		set(COMMON_C_FLAGS ${COMMON_C_FLAGS} -mssse3)
		set( SSE_BUILD ON )
		message(STATUS "Selected SSSE3")
		enableXOP()
	elseif( SSE2_SUPPORTED )
		set(COMMON_C_FLAGS ${COMMON_C_FLAGS} -msse2)
		set( SSE_BUILD ON )
		message(STATUS "Selected SSE2")
		enableXOP()
	else()
		set( REF_BUILD ON )
		message(STATUS "Selected reference")
	endif()
endif()
	
# Enable a build based on user setting
if( SSE_BUILD )
	message(STATUS "Configuring for SSE or newer")
	set(CONFIGURED_DIRECTORY sse)
elseif( NEON_BUILD )
	message(STATUS "Configuring for neon")
	set(CONFIGURED_DIRECTORY neon)
elseif( REF_BUILD)
	message(STATUS "Configuring reference suited for all")
	set(CONFIGURED_DIRECTORY ref)
else()
	message(FATAL_ERROR "Undefined build, cannot configure!")
endif()

# Add correct subdirectory based on the above selection
add_subdirectory(${CONFIGURED_DIRECTORY})

# Add checks if a check build
if( CHECK_BUILD )
	add_custom_target(check ALL
		COMMAND "$<SHELL_PATH:$<TARGET_FILE:blake2b>>"
		COMMAND "$<SHELL_PATH:$<TARGET_FILE:blake2bp>>"
		COMMAND "$<SHELL_PATH:$<TARGET_FILE:blake2s>>"
		COMMAND "$<SHELL_PATH:$<TARGET_FILE:blake2sp>>"
		COMMAND "$<SHELL_PATH:$<TARGET_FILE:blake2xb>>"
		COMMAND "$<SHELL_PATH:$<TARGET_FILE:blake2xs>>"
		DEPENDS blake2b blake2bp blake2s blake2sp blake2xb blake2xs
	)
else()
	add_subdirectory(b2sum)
	# Install binaries to bin, libs to lib and headers to include/blake2 using CMAKE_INSTALL_PREFIX
	install(TARGETS blake2s blake2b blake2sp blake2xb blake2bp blake2xs b2sum RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
	install(FILES "${CONFIGURED_DIRECTORY}/blake2.h" DESTINATION "include/blake2")
endif()


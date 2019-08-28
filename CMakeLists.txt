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

# Set minimum global cmake version
cmake_minimum_required(VERSION 3.2.0)

# Enable new versioning (old might be depracated)
cmake_policy(SET CMP0048 NEW)

# Build with install RPATH to prevent relinking which is unsupported by ninja on windows
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

project(blake2 LANGUAGES C)

# Include GNU install dirs for standard dirs
include(GNUInstallDirs)

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

set(XOP_ENABLED OFF CACHE BOOL "Adds XOP extension")

set(BUILD_B2SUM ON CACHE BOOL "Build b2sum executable")

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

list(APPEND BLAKE2_TARGETS blake2s blake2b blake2sp blake2xb blake2bp blake2xs)

# Add correct subdirectory based on the above selection
add_subdirectory(${CONFIGURED_DIRECTORY})

# Add tests if a check build
if( BUILD_TESTING )
	enable_testing()
	foreach(BLAKE2_TARGET IN LISTS BLAKE2_TARGETS)
		add_test(NAME ${BLAKE2_TARGET}  COMMAND $<TARGET_FILE:${BLAKE2_TARGET}> )
	endforeach()
else()
	if( BUILD_B2SUM )
		add_subdirectory(b2sum)
		list(APPEND BLAKE2_TARGETS b2sum)
	endif()
endif()

install(TARGETS ${BLAKE2_TARGETS}
EXPORT blake2-targets
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} 
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 
PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/blake2
)

install( EXPORT blake2-targets
		 FILE blake2Config.cmake
		 NAMESPACE blake2::
		 DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/blake2
)
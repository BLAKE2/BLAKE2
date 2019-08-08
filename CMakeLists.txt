# Enable new versioning (old might be depracated)
cmake_policy(SET CMP0048 NEW)

project(blake2 LANGUAGES C)

# Set minimum global cmake version
cmake_minimum_required(VERSION 3.15.0)

# Requires no extension and C90 standard
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Set flags used in all builds
set(COMMON_C_FLAGS -Wall -Wextra -Wno-long-long -pedantic)

# Build executables with check definition and target for running them if -DCHECK_BUILD=1 is passed
if( CHECK_BUILD )
	include_directories(testvectors)
	add_executable(blake2s)
    add_executable(blake2b)
    add_executable(blake2sp)
    add_executable(blake2xb)
    add_executable(blake2bp)
    add_executable(blake2xs)
	target_compile_definitions(blake2b  PRIVATE BLAKE2B_SELFTEST )
	target_compile_definitions(blake2s  PRIVATE BLAKE2S_SELFTEST )
	target_compile_definitions(blake2sp PRIVATE BLAKE2SP_SELFTEST)
	target_compile_definitions(blake2xb PRIVATE BLAKE2XB_SELFTEST)
	target_compile_definitions(blake2bp PRIVATE BLAKE2BP_SELFTEST)
	target_compile_definitions(blake2xs PRIVATE BLAKE2XS_SELFTEST)
	add_custom_target(check ALL
		COMMAND "$<SHELL_PATH:$<TARGET_FILE:blake2b>>"
		COMMAND "$<SHELL_PATH:$<TARGET_FILE:blake2bp>>"
		COMMAND "$<SHELL_PATH:$<TARGET_FILE:blake2s>>"
		COMMAND "$<SHELL_PATH:$<TARGET_FILE:blake2sp>>"
		COMMAND "$<SHELL_PATH:$<TARGET_FILE:blake2xb>>"
		COMMAND "$<SHELL_PATH:$<TARGET_FILE:blake2xs>>"
		DEPENDS blake2b blake2bp blake2s blake2sp blake2xb blake2xs
	)
# Build libraries and b2sum is nothing is passed
else()
	add_library(blake2s)
    add_library(blake2b)
    add_library(blake2sp)
    add_library(blake2xb)
    add_library(blake2bp)
    add_library(blake2xs)
	add_subdirectory(b2sum)
endif()

# Check if the compiler supports SSE or NEON flags
include(CheckCCompilerFlag)
check_c_compiler_flag(-msse2 SSE2_SUPPORTED)
check_c_compiler_flag(-mfpu=neon-vfpv4 NEON_SUPPORTED)

# Based on command line options (-DSSE_BUILD=1, -REF_BUILD=1 or -DNEON_BUILD=1) go for one of the builds
# If nothing is passed figure it out based on the SSE and NEON detection
if( SSE_BUILD )
	message(STATUS "Configuring for SSE or newer")
	set(CONFIGURED_DIRECTORY sse)
elseif( NEON_BUILD )
	message(STATUS "Configuring for neon")
	set(CONFIGURED_DIRECTORY neon)
elseif( REF_BUILD)
	message(STATUS "Configuring reference suited for all")
	set(CONFIGURED_DIRECTORY ref)
elseif( NEON_SUPPORTED AND SSE2_SUPPORTED )
	message(FATAL_ERROR "Cannot decide if neon, sse or ref as all are supported, please select using -DNEON_BUILD=1, -DSSE_BUILD=1 or -DREF_BUILD=1")
elseif( SSE2_SUPPORTED )
	message(STATUS "Automatically configuring for SSE or newer")
	set(CONFIGURED_DIRECTORY sse)
elseif( NEON_SUPPORTED )
	message(STATUS "Automatically configuring for neon")
	set(CONFIGURED_DIRECTORY neon)
else()
	message(STATUS "Automatically configuring reference suited for all")
	set(CONFIGURED_DIRECTORY ref)
endif()

# Add correct subdirectory based on the above selection
add_subdirectory(${CONFIGURED_DIRECTORY})

# Install binaries to bin, libs to lib and headers to include/blake2 using CMAKE_INSTALL_PREFIX
install(TARGETS blake2s blake2b blake2sp blake2xb blake2bp blake2xs b2sum RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
install(FILES "${CONFIGURED_DIRECTORY}/blake2.h" DESTINATION "include/blake2")

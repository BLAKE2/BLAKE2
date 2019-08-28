set(B2_COMPILE_OPTIONS -march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard -O3 ${COMMON_C_FLAGS})

add_compile_options( ${B2_COMPILE_OPTIONS} )

# Build executables with check definition and target for running them if -DCHECK_BUILD=1 is passed
if( BUILD_TESTING )
	include_directories(../testvectors)
	add_executable(blake2s "")
    add_executable(blake2b "")
    add_executable(blake2sp "")
    add_executable(blake2xb "")
    add_executable(blake2bp "")
    add_executable(blake2xs "")
	target_compile_definitions(blake2b  PRIVATE BLAKE2B_SELFTEST )
	target_compile_definitions(blake2s  PRIVATE BLAKE2S_SELFTEST )
	target_compile_definitions(blake2sp PRIVATE BLAKE2SP_SELFTEST)
	target_compile_definitions(blake2xb PRIVATE BLAKE2XB_SELFTEST)
	target_compile_definitions(blake2bp PRIVATE BLAKE2BP_SELFTEST)
	target_compile_definitions(blake2xs PRIVATE BLAKE2XS_SELFTEST)
# Build libraries and b2sum is nothing is passed
else()
	add_library(blake2s "")
    add_library(blake2b "")
    add_library(blake2sp "")
    add_library(blake2xb "")
    add_library(blake2bp "")
    add_library(blake2xs "")
endif()

target_include_directories(blake2b  
	PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include/blake2>)
target_include_directories(blake2s  
	PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include/blake2>)
target_include_directories(blake2sp  
	PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include/blake2>)
target_include_directories(blake2xb 
	PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include/blake2>)
target_include_directories(blake2bp  
	PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include/blake2>)
target_include_directories(blake2xs  
	PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include/blake2>)

set_target_properties(blake2b PROPERTIES PUBLIC_HEADER "blake2.h")
set_target_properties(blake2s PROPERTIES PUBLIC_HEADER "blake2.h")
set_target_properties(blake2sp PROPERTIES PUBLIC_HEADER "blake2.h")
set_target_properties(blake2xb PROPERTIES PUBLIC_HEADER "blake2.h")
set_target_properties(blake2bp PROPERTIES PUBLIC_HEADER "blake2.h")
set_target_properties(blake2xs PROPERTIES PUBLIC_HEADER "blake2.h")

target_sources(blake2b
    PRIVATE
		blake2b.c
		blake2b-round.h
		blake2-impl.h
		blake2b-load-neon.h
	#Headers
		blake2.h
)

target_sources(blake2s
    PRIVATE
		blake2s.c
		blake2s-round.h
		blake2-impl.h
		blake2s-load-neon.h
	#Headers
		blake2.h
)

target_sources(blake2sp
    PRIVATE
		blake2sp.c
		blake2s.c
		blake2s-round.h
		blake2-impl.h
		blake2s-load-neon.h
	#Headers
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
	#Headers
		blake2.h
)

target_sources(blake2bp
    PRIVATE
		blake2bp.c
		blake2b.c
		blake2b-round.h
		blake2-impl.h
		blake2b-load-neon.h
	#Headers
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
	#Headers
		blake2.h
)

install(TARGETS blake2s blake2b blake2sp blake2xb blake2bp blake2xs 
EXPORT blake2-targets
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} 
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 
PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/blake2
)

install( EXPORT blake2-targets
		 NAMESPACE blake2::
		 DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/blake2
)
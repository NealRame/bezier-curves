# Try to find LibSDL2
# Once done this will define
#  LIBSDL2_FOUND - System has LibSDL2
#  LIBSDL2_INCLUDE_DIRS - The LibSDL2 include directories
#  LIBSDL2_LIBRARIES - The libraries needed to use LibSDL2
#  LIBSDL2_DEFINITIONS - Compiler switches required for using LibSDL2

include(FindPackageHandleStandardArgs)

find_package(PkgConfig)

pkg_check_modules(PC_LIBSDL2 QUIET sdl2)
set(LIBSDL2_DEFINITIONS ${PC_LIBSDL2_CFLAGS_OTHER})

find_path(
	LIBSDL2_INCLUDE_DIR SDL.h
	HINTS ${PC_LIBSDL2_INCLUDEDIR} ${PC_LIBSDL2_INCLUDE_DIRS}
)

find_library(
	LIBSDL2_LIBRARY NAMES sdl2 libsdl2
	HINTS ${PC_LIBSDL2_LIBDIR} ${PC_LIBSDL2_LIBRARY_DIRS}
)

set(LIBSDL2_INCLUDE_DIRS ${LIBSDL2_INCLUDE_DIR})
set(LIBSDL2_LIBRARIES ${LIBSDL2_LIBRARY})
set(LIBSDL2_LIBRARY_DIRS ${PC_LIBSDL2_LIBRARY_DIRS})

# handle the QUIETLY and REQUIRED arguments and set LIBSDL2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
	LibSDL2 DEFAULT_MSG
	LIBSDL2_LIBRARY LIBSDL2_INCLUDE_DIR
)

mark_as_advanced(LIBSDL2_INCLUDE_DIR LIBSDL2_LIBRARY)

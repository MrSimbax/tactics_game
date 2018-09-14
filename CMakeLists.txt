cmake_minimum_required (VERSION 3.8)

project(mylearnopenglplayground)

# Options
option(BUILD_STATIC "Creates a static build" ON)

# C++11
set(CMAKE_CXX_STANDARD 11)

# Fixes error with SDLmain
set(CMAKE_EXE_LINKER_FLAGS "/ENTRY:mainCRTStartup")

# For debugging
macro(print_link_flags)
  set(MSVC_C_CXX_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_MINSIZEREL
    CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_RELWITHDEBINFO
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_MINSIZEREL
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_RELWITHDEBINFO
  )
  message(STATUS "Build flags:")
  foreach(flag ${MSVC_C_CXX_FLAGS})
    message(STATUS " ${flag}: ${${flag}}")
  endforeach()
  message(STATUS "")
endmacro()

# Set correct compiler flags in order to avoid mixing static and dynamic libraries
if(MSVC)
  set(MSVC_C_CXX_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_MINSIZEREL
    CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_RELWITHDEBINFO
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_MINSIZEREL
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_RELWITHDEBINFO
  )
  if(${BUILD_STATIC})
    foreach(flag ${MSVC_C_CXX_FLAGS})
      if(${flag} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
      endif()
    endforeach()
  else()
    foreach(flag ${MSVC_C_CXX_FLAGS})
      if(${flag} MATCHES "/MT")
        string(REGEX REPLACE "/MT" "/MD" ${flag} "${${flag}}")
      endif()
    endforeach()
  endif()
endif()

# Hide console window on not debug builds
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_WIN32_EXECUTABLE 1)
endif()

# Directories
SET(SOURCES_DIR "src")
SET(LIBS_DIR "src/libs")
SET(LIBS_SOURCES_DIR "${LIBS_DIR}/src")
SET(LIBS_INCLUDE_DIR "${LIBS_DIR}/include")

# Dependencies
find_package(SDL2 CONFIG REQUIRED)

add_library(GLAD STATIC "${LIBS_SOURCES_DIR}/glad.c")
target_include_directories(GLAD PRIVATE "${LIBS_INCLUDE_DIR}")

set(LIBS
	GLAD
	SDL2::SDL2main
	SDL2::SDL2-static
)

# Project files
set(SOURCE_FILES
	"${SOURCES_DIR}/application/application.cpp"
	"${SOURCES_DIR}/main.cpp"
)

add_executable (mylearnopenglplayground ${SOURCE_FILES})
target_link_libraries(mylearnopenglplayground PRIVATE ${LIBS})
target_include_directories(mylearnopenglplayground PRIVATE ${LIBS_INCLUDE_DIR})

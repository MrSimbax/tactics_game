cmake_minimum_required (VERSION 3.8)
message("CMake version ${CMAKE_VERSION}")

project(mylearnopenglplayground)

# Options
option(BUILD_STATIC "Creates a static build" ON)
option(FAST_STL "Improves performance for assimp" ON)

# C++11
set(CMAKE_CXX_STANDARD 17)

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
set(SOURCES_DIR "${PROJECT_SOURCE_DIR}/src")
set(LIBS_DIR "${PROJECT_SOURCE_DIR}/libs")
set(LIBS_SOURCES_DIR "${LIBS_DIR}/src")
set(LIBS_INCLUDE_DIR "${LIBS_DIR}/include")
set(ASSETS_FOLDER "assets")
set(ASSETS_DIR "${PROJECT_SOURCE_DIR}/${ASSETS_FOLDER}")

# Dependencies
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2 CONFIG REQUIRED)
find_package(ASSIMP REQUIRED)

add_library(GLAD STATIC "${LIBS_SOURCES_DIR}/glad.c")
target_include_directories(GLAD PRIVATE "${LIBS_INCLUDE_DIR}")

set(LIBS
    ${ASSIMP_LIBRARIES}
	GLAD
	SDL2::SDL2main
	SDL2::SDL2-static
)

# Visual Leak Detector
if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
	find_package(VLD)
	if(${VLD_FOUND})
		set(LIBS ${LIBS} ${VLD_LIBRARIES})
		set(LIBS_INCLUDE_DIR ${LIBS_INCLUDE_DIR} ${VLD_INCLUDE_DIRS})
		add_definitions(-DVLD_FOUND)
	endif()
endif()

# Project files
set(SOURCE_FILES
	"${SOURCES_DIR}/misc/custom_log.cpp"
	"${SOURCES_DIR}/misc/utils.cpp"
	"${SOURCES_DIR}/services/assets/assets_service_impl.cpp"
	"${SOURCES_DIR}/services/services.cpp"
	"${SOURCES_DIR}/graphics/window/sdl_window.cpp"
	"${SOURCES_DIR}/graphics/objects/triangle.cpp"
    #"${SOURCES_DIR}/graphics/objects/rectangle.cpp"
    "${SOURCES_DIR}/graphics/model/vao.cpp"
    "${SOURCES_DIR}/graphics/model/bo.cpp"
    "${SOURCES_DIR}/graphics/shader/shader.cpp"
	"${SOURCES_DIR}/graphics/shader/shader_program.cpp"
	"${SOURCES_DIR}/application/game_application.cpp"
	"${SOURCES_DIR}/main.cpp"
)

add_executable (mylearnopenglplayground ${SOURCE_FILES})
target_link_libraries(mylearnopenglplayground PRIVATE ${LIBS})
target_include_directories(mylearnopenglplayground PRIVATE ${LIBS_INCLUDE_DIR})

add_custom_command(TARGET mylearnopenglplayground POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_DIR} $<TARGET_FILE_DIR:mylearnopenglplayground>/${ASSETS_FOLDER}
)

if(${FAST_STL})
    add_definitions(-D_HAS_ITERATOR_DEBUGGING=0 -D_SECURE_SCL=0)
endif()

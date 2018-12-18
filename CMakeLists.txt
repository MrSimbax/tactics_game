cmake_minimum_required (VERSION 3.8)
message("CMake version ${CMAKE_VERSION}")

project(tactics_game)

# Options
option(BUILD_STATIC "Creates a static build" ON) # only MSVC
option(FAST_STL "Improves performance for assimp?" OFF)

# C++11
set(CMAKE_CXX_STANDARD 17)

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

# Flags for different compilers
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # if(${BUILD_STATIC})
    #     set(BUILD_SHARED_LIBS OFF)
    #     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    #     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/home/simbax/vcpkg/installed/x64-linux/lib -static -static-libgcc -static-libstdc++")
    #     set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    # endif()
else() # MSVC
    # Fixes error with SDLmain
    set(CMAKE_EXE_LINKER_FLAGS "/ENTRY:mainCRTStartup")
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
find_package(assimp CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

add_library(GLAD STATIC "${LIBS_SOURCES_DIR}/glad.c")
target_include_directories(GLAD PRIVATE "${LIBS_INCLUDE_DIR}")

add_library(IRRXML STATIC "${LIBS_INCLUDE_DIR}/irrxml/irrXML.cpp")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(LIBS
        assimp
        ZLIB::ZLIB
        IRRXML
        GLAD
        SDL2::SDL2
        SDL2::SDL2main
        stdc++fs
        dl
    )
else() # MSVC
    set(LIBS
        ASSIMP::ASSIMP
        ZLIB::ZLIB
        IRRXML
        GLAD
        SDL2::SDL2main
        SDL2::SDL2-static
    )
endif()

set(LIBS_INCLUDE_DIRS
    ${LIBS_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${ASSIMP_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
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
	"${SOURCES_DIR}/graphics/window/sdl_window.cpp"
    "${SOURCES_DIR}/graphics/model/lights.cpp"
    "${SOURCES_DIR}/graphics/model/buffered_light_object.cpp"
    "${SOURCES_DIR}/graphics/model/light_object.cpp"
    "${SOURCES_DIR}/graphics/model/top_camera.cpp"
    "${SOURCES_DIR}/graphics/model/free_roam_camera.cpp"
    "${SOURCES_DIR}/graphics/model/vao.cpp"
    "${SOURCES_DIR}/graphics/model/bo.cpp"
    "${SOURCES_DIR}/graphics/model/mesh.cpp"
    "${SOURCES_DIR}/graphics/model/model.cpp"
    "${SOURCES_DIR}/graphics/model/graphics_object.cpp"
    "${SOURCES_DIR}/graphics/model/buffered_mesh.cpp"
    "${SOURCES_DIR}/graphics/model/buffered_model.cpp"
    "${SOURCES_DIR}/graphics/model/buffered_graphics_object.cpp"
    "${SOURCES_DIR}/graphics/shader/shader.cpp"
	"${SOURCES_DIR}/graphics/shader/shader_program.cpp"
    "${SOURCES_DIR}/logic/path_finder.cpp"
    "${SOURCES_DIR}/logic/game_unit.cpp"
    "${SOURCES_DIR}/logic/player.cpp"
    "${SOURCES_DIR}/logic/game_map.cpp"
    "${SOURCES_DIR}/logic/game_scene.cpp"
    "${SOURCES_DIR}/logic/rendering/light_objects_renderer.cpp"
    "${SOURCES_DIR}/logic/rendering/unit_renderer.cpp"
    "${SOURCES_DIR}/logic/rendering/player_renderer.cpp"
    "${SOURCES_DIR}/logic/rendering/game_scene_renderer.cpp"
    "${SOURCES_DIR}/logic/rendering/game_map_renderer.cpp"
    "${SOURCES_DIR}/application/assets_manager.cpp"
    "${SOURCES_DIR}/application/input_manager.cpp"
	"${SOURCES_DIR}/application/game_application.cpp"
	"${SOURCES_DIR}/main.cpp"
)

add_executable (tactics_game ${SOURCE_FILES})
target_link_libraries(tactics_game PRIVATE ${LIBS})
target_include_directories(tactics_game PRIVATE ${LIBS_INCLUDE_DIRS})

add_custom_command(TARGET tactics_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_DIR} $<TARGET_FILE_DIR:tactics_game>/${ASSETS_FOLDER}
)

if(${FAST_STL})
    add_definitions(-D_HAS_ITERATOR_DEBUGGING=0 -D_SECURE_SCL=0)
endif()

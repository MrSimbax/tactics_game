cmake_minimum_required (VERSION 3.8)

project(mylearnopenglplayground)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "/W3")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Od")
set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2")

set(CMAKE_EXE_LINKER_FLAGS "/NODEFAULTLIB:MSVCRTD /ENTRY:mainCRTStartup")

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_WIN32_EXECUTABLE 1)
endif()

find_package(glfw3 CONFIG REQUIRED)

SET(SOURCES_DIR "src")

SET(LIBS_DIR "src/libs")
SET(LIBS_SOURCES_DIR "${LIBS_DIR}/src")
SET(LIBS_INCLUDE_DIR "${LIBS_DIR}/include")

add_library(GLAD "${LIBS_SOURCES_DIR}/glad.c")
target_include_directories(GLAD PRIVATE "${LIBS_INCLUDE_DIR}")

set(SOURCE_FILES
	"${SOURCES_DIR}/main.cpp"
)

set(LIBS
	glfw
	GLAD
)

add_executable (mylearnopenglplayground ${SOURCE_FILES})

target_link_libraries(mylearnopenglplayground PRIVATE ${LIBS})
target_include_directories(mylearnopenglplayground PRIVATE ${LIBS_INCLUDE_DIR})

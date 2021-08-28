cmake_minimum_required(VERSION 3.3)
project(AnandCraft)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
set_source_files_properties(SOURCE_FILES PROPERTIES COMPILE_FLAGS "-Werror -pedantic -Wall -Wextra")

add_executable(AnandCraft ${SOURCE_FILES})
target_link_libraries(AnandCraft ${SDL2_LIBRARY} GLEW::GLEW ${GLM_LIBRARIES} ${OPENGL_LIBRARIES})

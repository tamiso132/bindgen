cmake_minimum_required(VERSION 3.28)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wno-nullability-completeness -fdiagnostics-color=always")
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_COMPILER clang)



set(CMAKE_BUILD_TYPE Debug)
set(ROOT_DIR ${CMAKE_SOURCE_DIR})

if(NOT DEFINED LIB_NAME)
    set(LIB_NAME "libDynamicLibVulkan")
endif()

# Project name
project(DynamicLibVulkan)


# Set the default build type if not specified
# Include the source directory
set(SOURCES_FILES_GAME_ENGINE)
set(INCLUDE_DIRECTORIES_ENGINE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)


add_executable(${LIB_NAME} ${SOURCES_FILES_GAME_ENGINE})
include_directories(${CLANG_INCLUDE_DIRS} ${INCLUDE_DIRECTORIES_ENGINE})
target_link_libraries(${LIB_NAME} PRIVATE ${CLANG_LIBRARIES} clang)

set_target_properties(${LIB_NAME} PROPERTIES C_STANDARD 23)
target_compile_options(${LIB_NAME} PUBLIC -Wall -Wextra -Wno-nullability-completeness -std=c23)

set_target_properties(${LIB_NAME} PROPERTIES C_STANDARD 11)
target_compile_options(${LIB_NAME} PUBLIC -Wall -Wextra -Wno-nullability-completeness)

message(INFO, ${LIB_NAME})

cmake_minimum_required(VERSION 3.25)
project(WaMax)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

set(WAMAX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
cmake_path(NORMAL_PATH WAMAX_DIR OUTPUT_VARIABLE WAMAX_DIR)

set(WAMAX_SOURCE_DIR ${WAMAX_DIR}/source)
message(STATUS "SOURCE DIRECTORY @ ${PROJECT_NAME}: ${WAMAX_SOURCE_DIR}")

set(C74_MIN_API_DIR ${WAMAX_SOURCE_DIR}/min-api)

include(${WAMAX_SOURCE_DIR}/_common/common.cmake)

set(APPLICATIONS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../applications)

add_subdirectory(maxglobal)
add_subdirectory(maxobjects)

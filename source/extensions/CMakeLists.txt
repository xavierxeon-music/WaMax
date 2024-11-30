cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

string(REGEX REPLACE "(.*)/" "" THIS_FOLDER_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
project(${THIS_FOLDER_NAME})

if(APPLE)
   if(${CMAKE_GENERATOR} MATCHES "Xcode")
      set(C74_BUILD_FAT YES)
   endif()

   if(NOT CMAKE_OSX_ARCHITECTURES)
      if(C74_BUILD_FAT)
         set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "macOS architecture" FORCE)
      else()
         set(CMAKE_OSX_ARCHITECTURES ${CMAKE_SYSTEM_PROCESSOR} CACHE STRING "macOS architecture" FORCE)
      endif()

      message("CMAKE_OSX_ARCHITECTURES set to ${CMAKE_OSX_ARCHITECTURES}")
   endif()
endif()

set(C74_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../externals")

FILE(GLOB PROJECT_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

if(WIN32)
   list(REMOVE_ITEM PROJECT_LIST "wa.push2_display")
endif()

foreach(PROJECT_DIR ${PROJECT_LIST})
   set(project_path ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_DIR})

   if(EXISTS "${project_path}/CMakeLists.txt")
      message("Generating: ${PROJECT_DIR}")
      add_subdirectory(${project_path})
   endif()
endforeach()

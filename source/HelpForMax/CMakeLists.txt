cmake_minimum_required(VERSION 3.20)
project(HelpForMax LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   add_compile_definitions(TEST_CLIENT_AVAILABLE)
endif()

#
find_package(Qt6 REQUIRED COMPONENTS Widgets Network Svg Xml)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#
include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/Graph
   ${CMAKE_CURRENT_SOURCE_DIR}/Package
   ${CMAKE_CURRENT_SOURCE_DIR}/Patch
)

file(GLOB_RECURSE SOURCE_FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/*.h
   ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/*.ui
   ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc
)

# remove build dir
list(FILTER SOURCE_FILES EXCLUDE REGEX "${PROJECT_SOURCE_DIR}/build/.*")

if(APPLE)
   set(MACOSX_BUNDLE_ICON_FILE HelpForMax.icns)
   set(APP_ICON ${CMAKE_CURRENT_SOURCE_DIR}/icons/HelpForMax.icns)
   set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

   if(CMAKE_BUILD_TYPE STREQUAL "Release")
      set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{HOME}/Applications)
   endif()

   set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0")

   qt_add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${APP_ICON} ${SOURCE_FILES})
   target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
elseif(UNIX)
   if(CMAKE_BUILD_TYPE STREQUAL "Release")
      set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{HOME}/bin)
   endif()

   qt_add_executable(${PROJECT_NAME} ${SOURCE_FILES})
   target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
elseif(WIN32)
   # see https://doc.qt.io/qt-6/appicon.html
   # QMAKE_CXXFLAGS += /WX
   # RC_ICONS = $$PWD/icons/HelpForMax.ico
   qt_add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Network Qt6::Svg Qt6::Xml)

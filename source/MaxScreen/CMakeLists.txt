cmake_minimum_required(VERSION 3.20)
project(MaxScreen LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

find_package(Qt6 REQUIRED COMPONENTS Widgets Svg Network QuickWidgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   add_compile_definitions(TEST_CLIENT_AVAILABLE)
endif()

# C++
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
find_package(WaTools REQUIRED)

file(GLOB SOURCE_FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/*.h
   ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/Gui/*.qrc
   ${CMAKE_CURRENT_SOURCE_DIR}/Icons/*.qrc
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
   if(APPLE)
      set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{HOME}/Applications)
      message(STATUS "DESTINATION DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
   endif()
endif()

qt_add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
   WIN32_EXECUTABLE TRUE
   MACOSX_BUNDLE TRUE
)

# QML
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/QmlTypes)

file(GLOB QML_FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/QmlTypes/*.h
   ${CMAKE_CURRENT_SOURCE_DIR}/QmlTypes/*.cpp
)

qt_add_qml_module(${PROJECT_NAME} URI ScreenServer VERSION 1.0 SOURCES ${QML_FILES})

# GENERAL
if(APPLE)
   set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0")

   set(MACOSX_BUNDLE_ICON_FILE ${PROJECT_NAME}.icns)
   set(APP_ICON ${CMAKE_CURRENT_SOURCE_DIR}/Icons/${PROJECT_NAME}.icns)
   set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
   target_sources(${PROJECT_NAME} PRIVATE ${APP_ICON})
elseif(WIN32)
   set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.rc")
   target_sources(${PROJECT_NAME} PRIVATE ${APP_ICON})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Svg Qt6::Network Qt6::QuickWidgets)

include(${CMAKE_CURRENT_LIST_DIR}/Common/build.cmake)

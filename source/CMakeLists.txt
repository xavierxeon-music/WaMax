cmake_minimum_required(VERSION 3.20)
project(WaMax)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

set(APPLICATIONS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../applications)

add_subdirectory(maxglobal)
add_subdirectory(maxobjects)

# add_subdirectory(HelpForMax)
# add_subdirectory(MaxScreen)
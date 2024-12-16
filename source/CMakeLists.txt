cmake_minimum_required(VERSION 3.20)
project(WaMax)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   add_compile_definitions(TEST_CLIENT_AVAILABLE)
endif()

add_subdirectory(maxglobal)
add_subdirectory(maxobjects)
add_subdirectory(HelpForMax)
add_subdirectory(MaxScreen)
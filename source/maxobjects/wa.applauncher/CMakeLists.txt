set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

# ############################################################
# MAX EXTERNAL
# ############################################################
include_directories(
   "${C74_INCLUDES}"
)

# see https://github.com/Cycling74/min-devkit/tree/main/source/projects/min.environment
file(GLOB PROJECT_SRC
   "*.h"
   "*.c"
   "*.cpp"
)

if(APPLE)
   list(REMOVE_ITEM PROJECT_SRC "${PROJECT_SOURCE_DIR}/helper_win.cpp")
else()
   list(REMOVE_ITEM PROJECT_SRC "${PROJECT_SOURCE_DIR}/helper_mac.cpp")
endif()

# message(STATUS "APPLUNCHER SOURCE FILES: ${PROJECT_SRC}")
add_library(
   ${PROJECT_NAME}
   MODULE
   ${PROJECT_SRC}
)

find_package(Qt6 REQUIRED COMPONENTS Core Network)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Network)

if(APPLE)
   set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0") # for Qt
   find_library(CARBON_LIBRARY "Carbon")
   target_link_libraries(${PROJECT_NAME} PUBLIC ${CARBON_LIBRARY})
endif()

include(../_common/common.cmake)
include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

# ############################################################
# MAX EXTERNAL
# ############################################################
include_directories(
   "${C74_INCLUDES}"
)

file(GLOB PROJECT_SRC
   "*.h"
   "*.cpp"
)

if(APPLE)
   list(REMOVE_ITEM PROJECT_SRC "${PROJECT_SOURCE_DIR}/ProcessWrapper_linux.cpp")
   list(REMOVE_ITEM PROJECT_SRC "${PROJECT_SOURCE_DIR}/ProcessWrapper_win.cpp")
elseif(WIN32)
   list(REMOVE_ITEM PROJECT_SRC "${PROJECT_SOURCE_DIR}/ProcessWrapper_linux.cpp")
   list(REMOVE_ITEM PROJECT_SRC "${PROJECT_SOURCE_DIR}/ProcessWrapper_mac.cpp")
else()
   list(REMOVE_ITEM PROJECT_SRC "${PROJECT_SOURCE_DIR}/ProcessWrapper_mac.cpp")
   list(REMOVE_ITEM PROJECT_SRC "${PROJECT_SOURCE_DIR}/ProcessWrapper_win.cpp")
endif()

# message(STATUS "APPLUNCHER SOURCE FILES: ${PROJECT_SRC}")
add_library(
   ${PROJECT_NAME}
   MODULE
   ${PROJECT_SRC}
)

find_package(Qt6 REQUIRED COMPONENTS Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)

if(APPLE)
   find_library(CARBON_LIBRARY "Carbon")
   target_link_libraries(${PROJECT_NAME} PUBLIC ${CARBON_LIBRARY})
endif()

include(../_common/common.cmake)
include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

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
force_cpp_version()

find_package(Qt6 REQUIRED COMPONENTS Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)

use_carbon()

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

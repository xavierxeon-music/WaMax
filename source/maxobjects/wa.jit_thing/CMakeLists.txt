
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
   "*.c"
)

add_library(
   ${PROJECT_NAME}
   MODULE
   ${PROJECT_SRC}
)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

include(../_common/common.cmake)

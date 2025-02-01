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
   "*.c"
   "*.cpp"
)

add_library(
   ${PROJECT_NAME}
   MODULE
   ${PROJECT_SRC}
)
force_cpp_version()

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

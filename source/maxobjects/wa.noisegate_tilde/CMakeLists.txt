
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

include_directories("${C74_INCLUDES}")

file(GLOB PROJECT_SRC
   "*.h"
   "*.cpp"
)
add_library(
   ${PROJECT_NAME}
   MODULE
   ${PROJECT_SRC}
)
force_cpp_version()

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

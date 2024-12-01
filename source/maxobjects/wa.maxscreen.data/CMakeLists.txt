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
add_library(
   ${PROJECT_NAME}
   MODULE
   ${PROJECT_SRC}
)

include(../_common/common.cmake)
include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

if(APPLE)
   target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
endif()

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
force_cpp_version()

find_package(Qt6 REQUIRED COMPONENTS Core Network)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Network)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

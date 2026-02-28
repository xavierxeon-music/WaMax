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

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(XX REQUIRED COMPONENTS Math)

add_library(
   ${PROJECT_NAME}
   MODULE
   ${PROJECT_SRC}
)
force_cpp_version()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

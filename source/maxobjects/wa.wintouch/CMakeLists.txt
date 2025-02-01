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
force_cpp_version()

find_package(Qt6 REQUIRED COMPONENTS Gui)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Gui)

find_package(WaTools REQUIRED)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

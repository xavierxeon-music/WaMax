include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

include_directories("${C74_INCLUDES}")

file(GLOB PROJECT_SRC
   "*.h"
   "*.cpp"
)

find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(WaTools REQUIRED COMPONENTS General Core )
find_package(CURL REQUIRED)

add_library(
   ${PROJECT_NAME}
   MODULE
   ${PROJECT_SRC}
)
force_cpp_version()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Gui)
target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

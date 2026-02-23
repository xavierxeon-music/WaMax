include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

# ############################################################
# MAX EXTERNAL
# ############################################################
include_directories(
   "${C74_INCLUDES}"
   "${APPLICATIONS_DIR}/MaxScreen/Common"
)

file(GLOB PROJECT_SRC
   "*.h"
   "*.cpp"
)

find_package(Qt6 REQUIRED COMPONENTS Gui Network)
find_package(XX REQUIRED COMPONENTS Cpp Core Network)

add_library(
   ${PROJECT_NAME}
   MODULE
   ${PROJECT_SRC}
)
force_cpp_version()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Gui Qt6::Network)


include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

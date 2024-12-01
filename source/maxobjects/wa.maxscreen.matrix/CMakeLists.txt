set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

# ############################################################
# MAX EXTERNAL
# ############################################################
include_directories(
   "${C74_INCLUDES}"
   "${CMAKE_CURRENT_LIST_DIR}/../../MaxScreen/Common"
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

find_package(Qt6 REQUIRED COMPONENTS Gui Network)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Gui Qt6::Network)

find_package(WaTools REQUIRED)

if(APPLE)
   set(CMAKE_OSX_DEPLOYMENT_TARGET "15.0") # for Qt
endif()

include(../_common/common.cmake)
include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

if(APPLE)
   target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
endif()
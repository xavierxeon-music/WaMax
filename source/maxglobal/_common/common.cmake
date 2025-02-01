include_directories(${CMAKE_CURRENT_LIST_DIR})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

if(APPLE)
   set(CMAKE_OSX_DEPLOYMENT_TARGET "15.0") # for Qt
elseif(WIN32)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
endif()

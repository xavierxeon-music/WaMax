include_directories(${CMAKE_CURRENT_LIST_DIR})

if(APPLE)
   set(CMAKE_OSX_DEPLOYMENT_TARGET "15.0") # for Qt
   target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
endif()

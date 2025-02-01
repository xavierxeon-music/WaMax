include_directories(${CMAKE_CURRENT_LIST_DIR})

if(APPLE)
   set(CMAKE_OSX_DEPLOYMENT_TARGET "15.0") # for Qt
elseif(WIN32)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
endif()

# FUNCTIONS
function(use_carbon)
   if(APPLE)
      find_library(CARBON_LIBRARY "Carbon")
      target_link_libraries(${PROJECT_NAME} PUBLIC ${CARBON_LIBRARY})
   endif()
endfunction()

function(force_cpp_version)
   target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
endfunction()
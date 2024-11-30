include_directories(${CMAKE_CURRENT_LIST_DIR})

target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/SystemClipboard.h)

if(APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE -fobjc-arc)
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/SystemClipboard_mac.mm)

    find_library(APPKIT_LIBRARY AppKit)
   target_link_libraries(${PROJECT_NAME} PRIVATE ${APPKIT_LIBRARY})
elseif(WIN32)
   target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/SystemClipboard_win.cpp)
elseif(UNIX)
   target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/SystemClipboard_linux.cpp)
endif()

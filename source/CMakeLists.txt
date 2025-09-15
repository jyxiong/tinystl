add_library(tinystl INTERFACE)
target_include_directories(tinystl INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
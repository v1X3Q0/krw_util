if(NOT DEFINED ANDROID_TOOL_BASE)
    message("no ANDROID_TOOL_BASE, setting to working directory")
    set(ANDROID_TOOL_BASE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

add_subdirectory(krw_op)
add_subdirectory(krw_util)
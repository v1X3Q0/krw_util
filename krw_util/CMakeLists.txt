project(krw_util)

if(NOT DEFINED ANDROID_TOOL_BASE)
    message(FATAL_ERROR "need ANDROID_TOOL_BASE")
endif()

set(INCLUDE_LIST
    ${ANDROID_TOOL_BASE}/common/local_util
    ${ANDROID_TOOL_BASE}/common/bgrep_c
    ${ANDROID_TOOL_BASE}/common/kern_img/common
    ${ANDROID_TOOL_BASE}/common/kern_img/${MY_HOST_COMPILER}
    ${ANDROID_TOOL_BASE}/${MY_HOST_COMPILER}/${RW_DRIVER}/include)

if(${MY_HOST_COMPILER} STREQUAL "windows")
    list(APPEND INCLUDE_LIST ${ANDROID_TOOL_BASE}/${MY_HOST_COMPILER}/${RW_DRIVER}/targkrw)
endif()

add_library(${PROJECT_NAME} STATIC krw_util.cpp unix_dkrw.cpp kBaseRoll.cpp)

if (ARCH STREQUAL "arm64")
    list(APPEND INCLUDE_LIST ${ANDROID_TOOL_BASE}/common/hde_armv8)
elseif (ARCH STREQUAL "AMD64" OR ARCH STREQUAL "i386" OR ARCH MATCHES "x86_64")
    list(APPEND INCLUDE_LIST ${ANDROID_TOOL_BASE}/common/hde_x86)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_LIST})

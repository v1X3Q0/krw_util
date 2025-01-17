project(krw_util)

if(NOT DEFINED ANDROID_TOOL_BASE)
    message(FATAL_ERROR "need ANDROID_TOOL_BASE")
endif()

set(SOURCES_LIST
    krw_kernblock.cpp
)

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Metalkit")
    list(APPEND SOURCES_LIST krw_util.cpp unix_dkrw.cpp)
    # supported with kbaseroll
    list(APPEND SOURCES_LIST kBaseRoll.cpp)
endif()

set(INCLUDE_LIST
    ${ANDROID_TOOL_BASE}/common/local_util
    ${ANDROID_TOOL_BASE}/common/bgrep_c
    ${ANDROID_TOOL_BASE}/common/kern_img/common
    ${ANDROID_TOOL_BASE}/common/kern_img/${MY_HOST_COMPILER}
    ${ANDROID_TOOL_BASE}/${MY_HOST_COMPILER}/${RW_DRIVER}/include)

if(${MY_HOST_COMPILER} MATCHES "windows")
    list(APPEND INCLUDE_LIST ${ANDROID_TOOL_BASE}/${MY_HOST_COMPILER}/${RW_DRIVER}/targkrw)
elseif(${MY_HOST_COMPILER} MATCHES "metalkit")
    list(APPEND INCLUDE_LIST ${METALKIT_EXT})
endif()

add_library(${PROJECT_NAME} STATIC ${SOURCES_LIST})

if (ARCH STREQUAL "arm64")
    list(APPEND INCLUDE_LIST ${ANDROID_TOOL_BASE}/common/hde_armv8)
elseif (ARCH STREQUAL "AMD64" OR ARCH STREQUAL "i386" OR ARCH MATCHES "x86_64")
    list(APPEND INCLUDE_LIST ${ANDROID_TOOL_BASE}/common/hde_x86)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_LIST})

add_library(krw_notsimple STATIC krw_notsimple.cpp)

target_include_directories(krw_notsimple PRIVATE ${INCLUDE_LIST})
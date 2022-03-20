project(krw_util)

if(NOT DEFINED ANDROID_TOOL_BASE)
    message(FATAL_ERROR "need ANDROID_TOOL_BASE")
endif()

add_library(${PROJECT_NAME} STATIC krw_util.cpp unix_dkrw.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${ANDROID_TOOL_BASE}/common/local_util
    ${ANDROID_TOOL_BASE}/common/bgrep_c
    ${ANDROID_TOOL_BASE}/common/kern_img/common
    ${ANDROID_TOOL_BASE}/common/kern_img/linux
    ${ANDROID_TOOL_BASE}/linux/rwDrv/include)

if (DEFINED LIVE_KERNEL)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        LIVE_KERNEL=${LIVE_KERNEL})
    if (ARCH STREQUAL "arm64")
        target_include_directories(${PROJECT_NAME} PRIVATE
            ${ANDROID_TOOL_BASE}/common/hde_armv8)
    endif()
endif()

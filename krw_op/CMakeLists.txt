project(krw_op)
add_executable(${PROJECT_NAME} main.cpp)

if(NOT DEFINED ANDROID_TOOL_BASE)
    message(FATAL_ERROR "need ANDROID_TOOL_BASE")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${ANDROID_TOOL_BASE}/common/local_util
    ${ANDROID_TOOL_BASE}/common/krw_util/krw_util
    ${ANDROID_TOOL_BASE}/common/kern_img/common
    ${ANDROID_TOOL_BASE}/common/kern_img/${MY_HOST_COMPILER}
    ${ANDROID_TOOL_BASE}/common/bgrep_c
    ${ANDROID_TOOL_BASE}/${MY_HOST_COMPILER}/${RW_DRIVER}/include)

set(LIB_LIST
    bgrep_e krw_util targkrw
    localUtil localUtil_cpp ${KRW_USE}
    c_reginst # unnecessary til i reinsert heuristics.
    kern_${MY_HOST_COMPILER} kernel_block kernel_block_live
    ${MY_HOST_COMPILER}_dyn_offset)

if (ARCH MATCHES "arm64" OR ARCH MATCHES "arm64e")
    list(APPEND LIB_LIST hdeA64)
elseif(ARCH MATCHES "AMD64" OR ARCH MATCHES "i386")
    list(APPEND LIB_LIST hde_x86)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    list(APPEND LIB_LIST kernelresolver)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_options(${PROJECT_NAME} PRIVATE /DYNAMICBASE:NO)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_LIST})

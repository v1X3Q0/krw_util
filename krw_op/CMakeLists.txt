project(krw_op)

if(NOT DEFINED ANDROID_TOOL_BASE)
    message(FATAL_ERROR "need ANDROID_TOOL_BASE")
endif()

set(INCLUDE_LIST
    ${ANDROID_TOOL_BASE}/common/local_util
    # ${ANDROID_TOOL_BASE}/common/krw_util/krw_util
    ../krw_util
    ${ANDROID_TOOL_BASE}/common/kern_img/common
    ${ANDROID_TOOL_BASE}/common/kern_img/${MY_HOST_COMPILER}
    ${ANDROID_TOOL_BASE}/common/bgrep_c
    ${ANDROID_TOOL_BASE}/${MY_HOST_COMPILER}/${RW_DRIVER}/include
)

set(LIBDIR_LIST )

set(LIB_LIST
    krw_util targkrw
    ${KRW_USE}
    # ${CMAKE_SYSTEM_NAME}_dyn_offset 
    # kern_${MY_HOST_COMPILER} kernel_block bgrep_e
    # krw_util ${CMAKE_SYSTEM_NAME}_dyn_offset
    localUtil
    localUtil_cpp
    )

if(DEFINED WITH_CAPSTONE)
    list(APPEND LIB_LIST capstone)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    list(APPEND LIB_LIST kernelresolver)
# redundancy for linux builds
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    list(APPEND LIB_LIST krw_util)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_package(getopt_for_windows)
    list(APPEND INCLUDE_LIST ${getopt_for_windows_INCLUDE_DIR})
    list(APPEND LIBDIR_LIST ${getopt_for_windows_LIB_DIR})
    list(APPEND LIB_LIST getopt_for_windows_static)

    list(APPEND LIB_LIST krw_notsimple kern_windows kernel_block Windows_dyn_offset bgrep_e)
    if (ARCH MATCHES "arm64" OR ARCH MATCHES "arm64e" OR ARCH MATCHES "aarch64")
        list(APPEND LIB_LIST c_reginst_A64)
    elseif(ARCH MATCHES "AMD64" OR ARCH MATCHES "i386" OR ARCH MATCHES "x86_64")
        list(APPEND LIB_LIST c_reginst_x86)
    endif()
endif()

if (ARCH MATCHES "arm64" OR ARCH MATCHES "arm64e" OR ARCH MATCHES "aarch64")
    list(APPEND LIB_LIST hdeA64
    )
elseif(ARCH MATCHES "AMD64" OR ARCH MATCHES "i386" OR ARCH MATCHES "x86_64")
    list(APPEND LIB_LIST hde_x86
    )
endif()

add_executable(${PROJECT_NAME} main.cpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE SIMPLE_KRW=1)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_LIST})

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_options(${PROJECT_NAME} PRIVATE /DYNAMICBASE:NO)
endif()

target_link_directories(${PROJECT_NAME} PRIVATE ${LIBDIR_LIST})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_LIST})

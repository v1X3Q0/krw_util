cmake_minimum_required(VERSION 3.5.0)
project(krwutil_top)

if(NOT DEFINED ANDROID_TOOL_BASE)
    message("no ANDROID_TOOL_BASE, setting to working directory")
    set(ANDROID_TOOL_BASE ${CMAKE_CURRENT_SOURCE_DIR})

    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(MY_HOST_COMPILER xnu)
        set(RW_DRIVER rw_kext)
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(MY_HOST_COMPILER linux)
        set(RW_DRIVER rwDrv)
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Android")
        set(ARCH "arm64")
        set(MY_HOST_COMPILER linux)
        set(RW_DRIVER rwDrv)
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(MY_HOST_COMPILER windows)
        set(RW_DRIVER winkrw)
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Metalkit")
        set(MY_HOST_COMPILER metalkit)
        set(RW_DRIVER metalkit_api)
    else()
        message( FATAL_ERROR "need to be compiling from a valid host, can't be ${CMAKE_SYSTEM_NAME}" )
    endif()

    if (NOT DEFINED ARCH)
        set(ARCH ${CMAKE_HOST_SYSTEM_PROCESSOR})
    endif()

    add_subdirectory(common/hde_x86)
    add_subdirectory(common/hde_armv8)
    add_subdirectory(common/local_util)
    add_subdirectory(linux/rwDrv)
    execute_process(
        COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/common/krw_util
    )
    endif()

message("myhost ${MY_HOST_COMPILER}")

add_subdirectory(krw_op)
add_subdirectory(krw_util)
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
    ${ANDROID_TOOL_BASE}/${MY_HOST_COMPILER}/rwDrv/include)

target_link_libraries(${PROJECT_NAME} PRIVATE
    kern_${MY_HOST_COMPILER} kernel_block bgrep_e krw_util targkrw
    localUtil localUtil_cpp ${KRW_USE})
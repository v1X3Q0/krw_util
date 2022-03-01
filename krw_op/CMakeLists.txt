add_executable(krw_op main.cpp)
target_link_libraries(krw_op PRIVATE kern_img kernel_block bgrep_e krw_util targkrw localUtil ${KRW_USE})
#include <stdio.h>
#include <stdint.h>

#include <localUtil.h>

#include <kernel_block.h>
#include <kern_img.h>

#ifdef __APPLE__
#include <TargetConditionals.h>
#elif defined(_WIN32)
#include <Windows.h>
#include <ntkern_tools.h>
#endif

#ifdef __METALKIT__
#include <mk_extern_util/mod_prep.h>
kernel_metalkit* g_kernblock = 0;
#else
#if !defined(__linux__)
#include <kern_static.h>
#endif
#include <kern_dynamic.h>
kern_dynamic* g_kernblock = 0;
#endif


#include "krw_util.h"

int kInit_notsimple()
{
	int result = -1;
	size_t found_base = 0;
#ifndef __linux__
    kern_static* kern_tmp = 0;
#endif

	SAFE_BAIL(kernel_init() == -1);
#ifdef _WIN32
    kern_tmp = kernel_block::allocate_kern_img<kern_static>(KERNEL_PATH);
	SAFE_BAIL(kernel_base_block(kern_tmp, &found_base) == -1);
#elif defined(__APPLE__) || (defined(__linux__) && !defined(__METALKIT__))
	SAFE_BAIL(kernel_base(&found_base)== -1);
#endif
    
#ifdef _WIN32
	g_kernblock = kernel_block::grab_live_kernel<kern_dynamic>((void*)found_base, (kernel_block*)kern_tmp);
#elif defined(__APPLE__) || (defined(__linux__) && !defined(__METALKIT__))
	g_kernblock = kernel_block::grab_live_kernel<kern_dynamic>((void*)found_base);
#elif defined(__METALKIT__)
	g_kernblock = new kernel_metalkit((uint32_t*)found_base);
#endif
	SAFE_BAIL(g_kernblock == 0);

	result = 0;
fail:
	return result;
}

int kInit_simple()
{
	int result = -1;

	SAFE_BAIL(kernel_init() == -1);

	result = 0;
fail:
	return result;
}

int kRead(void* buf, size_t len, size_t offset)
{
	return kernel_read(buf, len, offset);
}

int kReadPtr(void* buf, size_t len, size_t offset)
{
	return kernel_read_ptr(buf, len, offset);
}

int kWrite(void* buf, size_t len, size_t offset)
{
	return kernel_write(buf, len, offset);
}

int kWritePtr(void* buf, size_t len, size_t offset)
{
	return kernel_write_ptr(buf, len, offset);
}

int kSlide(size_t* a_kernelSlide)
{
	return kernel_slide(a_kernelSlide);
}

int kBase(size_t* a_kernelBase)
{
	return kernel_base(a_kernelBase);
}

int kClose()
{
	return kernel_close();
}


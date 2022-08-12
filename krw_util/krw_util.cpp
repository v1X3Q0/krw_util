#include <stdio.h>
#include <stdint.h>

#include <localUtil.h>

#include <kernel_block.h>
#include <kern_img.h>

#ifdef __APPLE__
#include <TargetConditionals.h>
#elif defined _WIN32
#include <Windows.h>
#include <ntkern_tools.h>
#endif

#include "krw_util.h"

#ifdef __METALKIT__
extern kernel_metalkit* g_kernblock;
#else
#include <kern_dynamic.h>
#include <kern_static.h>
extern kern_dynamic* g_kernblock;
#endif

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
	SAFE_BAIL(kernel_base(kern_tmp, &found_base) == -1);
#else
	SAFE_BAIL(kernel_base(&found_base)== -1);
#endif
    
#ifdef _WIN32
	g_kernblock = kernel_block::grab_live_kernel<kern_dynamic>((void*)found_base, (kernel_block*)kern_tmp);
#else
	g_kernblock = kernel_block::grab_live_kernel<kern_dynamic>((void*)found_base);
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

size_t kSlideTarg(size_t targ)
{
	size_t resultTarg = -1;
	size_t kernSlideTmp = 0;
	SAFE_BAIL(kernel_slide(&kernSlideTmp) == -1);

	resultTarg = targ + kernSlideTmp;
fail:
	return resultTarg;
}

size_t kBaseTarg(size_t targ)
{
	size_t resultTarg = -1;
	size_t kernBaseTmp = 0;
	SAFE_BAIL(kernel_base(&kernBaseTmp) == -1);

	resultTarg = targ + kernBaseTmp;
fail:
	return resultTarg;
}

void* kdlsym(const char* symbol)
{
	void* result = 0;

	FINISH_IF(g_kernblock->ksym_dlsym(symbol, (size_t*)&result) == -1);

finish:
	return result;
}

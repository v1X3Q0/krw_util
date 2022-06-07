#include <stdio.h>
#include <stdint.h>

#include <localUtil.h>
#include <kernel_block.h>

#include <kern_img.h>
#include <kern_dynamic.h>
#include <kern_static.h>

#ifdef __APPLE__
#include <TargetConditionals.h>
#elif defined _WIN32
#include <Windows.h>
#include <ntkern_tools.h>
#endif

#include "krw_util.h"

kern_dynamic* g_kernblock = 0;

int kInit()
{
	int result = -1;
	size_t found_base = 0;
    kern_static* kern_tmp = 0;

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

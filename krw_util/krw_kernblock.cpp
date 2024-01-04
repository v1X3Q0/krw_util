#include <stdio.h>
#include <stdint.h>

#include <localUtil.h>

#include <kernel_block.h>
#include <kern_img.h>

#include "krw_util.h"

#ifdef __APPLE__
#include <TargetConditionals.h>
#elif defined(_WIN32)
#include <Windows.h>
#include <ntkern_tools.h>
#endif

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


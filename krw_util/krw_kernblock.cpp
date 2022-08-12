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

#ifdef __METALKIT__
kernel_metalkit* g_kernblock = 0;
#else
#include <kern_dynamic.h>
#include <kern_static.h>
kern_dynamic* g_kernblock = 0;
#endif

#include "krw_util.h"

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


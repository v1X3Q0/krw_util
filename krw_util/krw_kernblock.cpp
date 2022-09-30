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

// mac is the most straightforward, with the driver framework and the
// ability to read the kernel as a file, and base it using the mach
// header.
int kInit_notsimple()
#ifdef __APPLE__
{
	int result = -1;
	size_t found_base = 0;
    kern_static* kern_tmp = 0;

	SAFE_BAIL(kernel_init() == -1);
	SAFE_BAIL(kernel_base(&found_base)== -1);
    
	g_kernblock = kernel_block::grab_live_kernel<kern_dynamic>((void*)found_base);
	SAFE_BAIL(g_kernblock == 0);

	result = 0;
fail:
	return result;
}
// windows also has the kernel accessible from the filesystem, so once
// we have a kernel read write and leak, we can then load the kernel as
// a file and then use that to guess where the kernel base is.
#elif defined(_WIN32)
{
	int result = -1;
	size_t found_base = 0;
    kern_static* kern_tmp = 0;

	SAFE_BAIL(kernel_init() == -1);
    kern_tmp = kernel_block::allocate_kern_img<kern_static>(KERNEL_PATH);
	SAFE_BAIL(kernel_base_block(kern_tmp, &found_base) == -1);
    
	g_kernblock = kernel_block::grab_live_kernel<kern_dynamic>((void*)found_base, (kernel_block*)kern_tmp);
	SAFE_BAIL(g_kernblock == 0);

	result = 0;
fail:
	return result;
}
// metalkit doesn't need a base, so perform a kernel init then get
// straight to it.
#elif defined(__METALKIT__)
{
	int result = -1;
	size_t found_base = 0;

	SAFE_BAIL(kernel_init() == -1);
    
	g_kernblock = new kernel_metalkit((uint32_t*)found_base);
	SAFE_BAIL(g_kernblock == 0);

	result = 0;
fail:
	return result;
}
// for linux, cause there are so many variants and a difference between
// root and not, the root population and internal population are separate.
// all things considered, populate with the devils you know, then with
// the devils you don't.
#elif defined(__linux__)
{
	int result = -1;
	size_t found_base = 0;


	SAFE_BAIL(kernel_init() == -1);

	SAFE_BAIL(kernel_base(&found_base)== -1);
	
	if (g_kernblock == 0)
	{
		g_kernblock = kernel_block::grab_live_kernel<kern_dynamic>((void*)found_base);
	}
	
    
	SAFE_BAIL(g_kernblock == 0);

	result = 0;
fail:
	return result;
}
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


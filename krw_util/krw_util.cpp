#include <stdio.h>
#include <stdint.h>

#include <localUtil.h>
#include <kernel_block.h>
#include <kern_img.h>
#include <kern_dynamic.h>

#include "krw_util.h"

kern_dynamic* g_kernblock = 0;

int kInit()
{
    int result = -1;
    size_t found_base = 0;

    SAFE_BAIL(kernel_init() == -1);
    SAFE_BAIL(kernel_base(&found_base) == -1);

    g_kernblock = kernel_block::grab_live_kernel<kern_dynamic>((void*)found_base);
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

    result = 0;
finish:
    return result;
}

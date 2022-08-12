#ifndef KRW_UTIL_H
#define KRW_UTIL_H

#include <stdio.h>
#include <stdint.h>

#ifndef __METALKIT__
#include "krw_kern_types.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __METALKIT__
#define kInit generic_init
#else
#define kInit kInit_notsimple
#endif

// these routines are provided and exported
int kInit_simple();
int kInit_notsimple();
int kRead(void* buf, size_t len, size_t offset);
int kReadPtr(void* buf, size_t len, size_t offset);
int kWrite(void* buf, size_t len, size_t offset);
int kWritePtr(void* buf, size_t len, size_t offset);
int kSlide(size_t* a_kernelSlide);
int kBase(size_t* a_kernelBase);
int kClose();
size_t kSlideTarg(size_t targ);
size_t kBaseTarg(size_t targ);

int kBaseRoll(size_t* kbase_a);

// putting declarators here, they need to be defined later.
int kernel_init();
// kernel leak is a funny one, for now i have it leaking printk or
// printf for android and mac respectively. All for future guys
int kernel_leak(size_t* leak_out);
int kernel_read(void* buf, size_t len, size_t offset);
int kernel_read_ptr(void* buf, size_t len, size_t offset);
int kernel_write(void* buf, size_t len, size_t offset);
int kernel_write_ptr(void* buf, size_t len, size_t offset);
int kernel_slide(size_t* a_kernelSlide);
int kernel_base(size_t* a_kernelBase);
int kernel_close();

#ifdef _WIN32
#include <Windows.h>
int wind_kernel_write(HANDLE fd, void* buf, size_t count);
int wind_kernel_read(HANDLE fd, void* buf, size_t count);
int kernelOp(void* inArr, DWORD inArrSz, void* outArr, DWORD outArrSz);
#define FDGLOB HANDLE
#else
#define FDGLOB int
#endif

// exporting these for mac and linux, maybe figure it out for windows
// later
int unix_dkread(FDGLOB _fd, void* buf, size_t len, size_t offset);
int unix_dkwrite(FDGLOB _fd, void* buf, size_t len, size_t offset);

void* kdlsym(const char* symbol);

#ifdef __cplusplus
}
#endif

#endif
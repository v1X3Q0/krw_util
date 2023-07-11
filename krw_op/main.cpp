/**
 * @file   testebbchar.c
 * @author Derek Molloy
 * @date   7 April 2015
 * @version 0.1
 * @brief  A Linux user space program that communicates with the ebbchar.c LKM. It passes a
 * string to the LKM and reads the response from the LKM. For this example to work the device
 * must be called /dev/ebbchar.
 * @see http://www.derekmolloy.ie/ for a full description and follow-up descriptions.
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#include <localUtil.h>
#include <localUtil_cpp.h>

#include <k_share.h>
#include <krw_util.h>
#include <krw_ptr.h>


#define CUROP_READ  1
#define CUROP_WRITE 2

#ifdef kInit
#undef kInit
#define kInit kInit_simple
#endif

void usage(const char* arg)
{
    printf("usage: %s [-d] op slide addr size\n", arg);
    printf("leaks: 0 is generic, %d is base, %d is slide\n", KERN_BASE_SHARE, KERN_SLIDE_SHARE);
    exit(1);
}

#ifdef WITH_CAPSTONE

#include <capstone/capstone.h>
int disassembleMem(uint8_t *base, size_t len)
{
    csh handle;
    int result = -1;
    cs_insn *insn;
    size_t count = 0;
    size_t j = 0;
    cs_arch architecure_targ;
    cs_mode mode_targ;

#if defined(__arm64__) || defined(__aarch64__)
    architecure_targ = CS_ARCH_ARM64;
    mode_targ = CS_MODE_ARM;
#elif defined(__AMD64__)
    architecure_targ = CS_ARCH_X86;
    mode_targ = CS_MODE_64;
#endif
    SAFE_BAIL(cs_open(architecure_targ, mode_targ, &handle) != CS_ERR_OK);
    count = cs_disasm(handle, base, len, 0x1000, 0, &insn);
    SAFE_PAIL(count <= 0, "ERROR: Failed to disassemble given code!\n");
    for (j = 0; j < count; j++)
    {
        printf("0x%" PRIx64 ":\t%s\t\t%s\n", insn[j].address, insn[j].mnemonic,
                insn[j].op_str);
    }
    cs_free(insn, count);

    cs_close(&handle);
fail:
    return result;
}
#endif // WITH_CAPSTONE

int main(int argc, char **argv)
{
#define OPTIND_PUSH(ARGUMENT) \
    if (optind == argc) \
    { \
        usage(argv[0]); \
    } \
    else \
    { \
        ARGUMENT = argv[optind]; \
        optind++; \
    }

#define OPTIND_PUSH_HEX(ARGUMENT) \
    if (optind == argc) \
    { \
        usage(argv[0]); \
    } \
    else \
    { \
        ARGUMENT = strtoull(argv[optind], NULL, 0x10); \
        optind++; \
    }


    SLIDE_BASE_t slid;
    int ret = 0;
    int curOp = 0;
    char* format = 0;
    size_t addr = 0;
    size_t len = 0;
    uint8_t* netBuf = 0;
    unsigned char *readBuf = 0;
    int i = 0;
    char default_format[] = "qx";
    int disassemble = 0;
    const char* format_arg = 0;
    const char* basis_arg = 0;

    int opt = 0;
    
    while ((opt = getopt(argc, argv, "d")) != -1)
    {
        switch (opt)
        {
        case 'd':
            disassemble = 1;
            break;
        default: /* '?' */
            usage(argv[0]);
        }
    }

    // pull mandatory argument
    OPTIND_PUSH(format_arg);
    OPTIND_PUSH(basis_arg);
    OPTIND_PUSH_HEX(addr);
    OPTIND_PUSH_HEX(len);

    if (*format_arg == 'r')
    {
        curOp = CUROP_READ;
        format = (char*)&format_arg[1];
    }
    else if (*format_arg == 'w')
    {
        curOp = CUROP_WRITE;
    }
    else
    {
        printf("invalid op arg");
        usage(argv[0]);
    }

    if (*basis_arg == 's')
    {
        slid = SLIDE_TARG;
    }
    else if (*basis_arg == 'b')
    {
        slid = BASE_TARG;
    }
    else if (*basis_arg == 'u')
    {
        slid = UNTOUCHED_TARG;
    }
    else
    {
        printf("invalid slide arg");
        usage(argv[0]);
    }

    if ((len % sizeof(size_t)) != 0)
    {
        len += (sizeof(size_t) - (len % sizeof(size_t)));
    }

    SAFE_BAIL(kInit() == -1);

    netBuf = (uint8_t*)calloc(1, len);
    readBuf = &netBuf[sizeof(size_t)];

    if (slid == SLIDE_TARG)
    {
        addr = kSlideTarg(addr);
    }
    else if (slid == BASE_TARG)
    {
        addr = kBaseTarg(addr);
    }

    // userspace evaluations
    if ((addr > LEAK_KERNMAX) && (addr < LEAK_USERMAX))
    {
        size_t to_leak = 0;
        switch (addr)
        {
        case KERN_BASE_SHARE:
            SAFE_BAIL(kBase(&to_leak) == -1);
            break;
        case KERN_SLIDE_SHARE:
            SAFE_BAIL(kSlide(&to_leak) == -1);
            break;
        default:
            goto fail;
        }
        dumpMem((uint8_t*)&to_leak, sizeof(size_t), default_format);
    }

    // all of the kernel enumerations are evaluated in the kernel during a read,
    // so just perform the read
    else if (curOp == CUROP_READ)
    {
        kRead(netBuf, len, addr);
        if (disassemble == 0)
        {
            dumpMem(netBuf, len, format);
        }
#ifdef WITH_CAPSTONE
        else
        {
            disassembleMem(netBuf, len);
        }
#endif
    }
    else if (curOp == CUROP_WRITE)
    {
        kWrite(netBuf, len, addr);
    }

fail:
    return 0;
}

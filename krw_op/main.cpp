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

int main(int argc, char **argv)
{
    SLIDE_BASE_t slid;
    int ret = 0;
    int curOp = 0;
    char format = 0;
    void *addr = 0;
    size_t len = 0;
    uint8_t* netBuf = 0;
    unsigned char *readBuf = 0;
    int i = 0;
    
    if (argc < 5)
    {
        printf("usage: %s op slide addr size\n", argv[0]);
        return -1;
    }

    if (*argv[1] == 'r')
    {
        curOp = CUROP_READ;
        format = argv[1][1];
    }
    else if (*argv[1] == 'w')
    {
        curOp = CUROP_WRITE;
    }
    else
    {
        printf("invalid op arg");
        return -1;
    }

    if (*argv[2] == 's')
    {
        slid = SLIDE_TARG;
    }
    else if (*argv[2] == 'b')
    {
        slid = BASE_TARG;
    }
    else if (*argv[2] == 'u')
    {
        slid = UNTOUCHED_TARG;
    }
    else
    {
        printf("invalid slide arg");
        return -1;
    }

    addr = (void *)strtoull(argv[3], NULL, 0x10);
    len = strtoull(argv[4], NULL, 0x10);

    if ((len % sizeof(size_t)) != 0)
    {
        len += (sizeof(size_t) - (len % sizeof(size_t)));
    }

    SAFE_BAIL(kInit() == -1);

    netBuf = (uint8_t*)calloc(1, len);
    readBuf = &netBuf[sizeof(size_t)];

    if (slid == SLIDE_TARG)
    {
        addr = (void *)kSlideTarg((size_t)addr);
    }
    else if (slid == BASE_TARG)
    {
        addr = (void *)kBaseTarg((size_t)addr);
    }

    // userspace evaluations
    if (((size_t)addr > LEAK_KERNMAX) && ((size_t)addr < LEAK_USERMAX))
    {
        size_t to_leak = 0;
        switch ((size_t)addr)
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
        dumpMem((uint8_t*)&to_leak, sizeof(size_t), 'q');
    }

    // all of the kernel enumerations are evaluated in the kernel during a read,
    // so just perform the read
    else if (curOp == CUROP_READ)
    {
        kRead(netBuf, len, (size_t)addr);
        dumpMem(netBuf, len, format);
    }
    else if (curOp == CUROP_WRITE)
    {
        kWrite(netBuf, len, (size_t)addr);
    }

fail:
    return 0;
}

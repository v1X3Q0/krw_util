#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#include <localUtil.h>

#include "k_share.h"
#include "krw_util.h"

#ifdef _WIN32
#define uread_pair(fd, seek_targ, read_addr, size_in) \
    kernelOp(seek_targ, sizeof(seek_struct), read_addr, size_in);
#define uwrite(fd, seek_targ, size_out) \
    kernelOp(seek_targ, size_out, NULL, NULL)
#else
#define uread_pair(fd, seek_targ, read_addr, size_in) \
    write(fd, seek_targ, sizeof(seek_struct)); \
    SAFE_BAIL(read(fd, read_addr, size_in) != size_in);
#define uwrite(fd, seek_targ, size_out) \
    write(fd, seek_targ, size_out)
#endif

int unix_dkread(FDGLOB _fd, void *buf, size_t len, size_t offset)
{
    int result = -1;
    seek_struct destOff{SEEK_OP | READ_OP, offset, len};
    size_t curSz = 0;
    signed long rw_res = 0;

    for (size_t i = 0; i < len; i += KBUF_SIZE)
    {
        curSz = len - i;
        if ((len - i) > KBUF_SIZE)
        {
            curSz = KBUF_SIZE;
        }
        destOff.offset = offset + i;
        destOff.len = curSz;
        uread_pair(_fd, &destOff, (uint8_t *)buf + i, curSz);
    }
    result = 0;
fail:
    return result;
}

int unix_dkwrite(FDGLOB _fd, void *buf, size_t len, size_t offset)
{
    int result = -1;
    size_t curSz = 0;
    uint8_t writeBuf[KBUF_NET];
    seek_struct* destOff = (seek_struct*)writeBuf;
    destOff->opVals = SEEK_OP | WRITE_OP;

    for (size_t i = 0; i < len; i += KBUF_SIZE)
    {
        curSz = len - i;
        if ((len - i) > KBUF_SIZE)
        {
            curSz = KBUF_SIZE;
        }
        curSz += sizeof(seek_struct);
        memcpy(destOff + 1, (uint8_t *)buf + i, curSz);
        destOff->offset = offset + i;
        destOff->len = curSz;
        SAFE_BAIL(uwrite(_fd, writeBuf, curSz) != curSz);
    }

    result = 0;
fail:
    return result;
}

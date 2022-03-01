#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <localUtil.h>

#include "k_share.h"
#include "krw_util.h"

int unix_dkread(int _fd, void *buf, size_t len, size_t offset)
{
    int result = -1;
    seek_struct destOff{SEEK_OP | READ_OP, offset, len};
    size_t curSz = 0;
    ssize_t rw_res = 0;

    for (size_t i = 0; i < len; i += KBUF_SIZE)
    {
        curSz = len - i;
        if ((len - i) > KBUF_SIZE)
        {
            curSz = KBUF_SIZE;
        }
        destOff.offset = offset + i;
        destOff.len = curSz;
        rw_res = write(_fd, &destOff, sizeof(seek_struct));
        SAFE_BAIL(read(_fd, (uint8_t *)buf + i, curSz) != curSz);
    }

    result = 0;
fail:
    return result;
}

int unix_dkwrite(int _fd, void *buf, size_t len, size_t offset)
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
        SAFE_BAIL(write(_fd, writeBuf, curSz) != curSz);
    }

    result = 0;
fail:
    return result;
}

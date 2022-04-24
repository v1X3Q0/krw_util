#ifndef K_SHARE_H
#define K_SHARE_H

#include <drv_share.h>

typedef enum
{
#ifdef OP_MAX
    READ_OP=OP_MAX << 1,
#else
    READ_OP=1,
#endif
    WRITE_OP=READ_OP << 1,
    SEEK_OP=WRITE_OP << 1
} opVals_t;

typedef struct
{
    int opVals;
    size_t offset;
    size_t len;
} seek_struct;

#define KBUF_NET        (KBUF_SIZE + sizeof(seek_struct))

#endif
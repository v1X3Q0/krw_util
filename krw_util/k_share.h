#ifndef K_SHARE_H
#define K_SHARE_H

#include <drv_share.h>

typedef enum
{
    READ_OP=1,
    WRITE_OP=2,
    SEEK_OP=4
} opVals_t;

typedef struct
{
    int opVals;
    size_t offset;
    size_t len;
} seek_struct;

#define KBUF_NET        (KBUF_SIZE + sizeof(seek_struct))

#endif
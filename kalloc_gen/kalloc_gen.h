#ifndef MACH_MSG_KALLOC_H
#define MACH_MSG_KALLOC_H

#include <stdint.h>

class kalloc_gen
{
private:
protected:
    size_t kva;
    char* backing;
    size_t net_sz;

public:
    virtual int kalloc_mech(size_t allocSz) = 0;
    // static kalloc_mm* kalloc(size_t allocSz)
    // {
    //     kalloc_mm* = 0;
    //     SAFE_BAIL(mach_msg_kalloc(allocSz) == -1);
    // fail:
        
    // }

    template<typename kalloc_type>
    static kalloc_gen* kalloc_static(size_t sizeIn)
    {
        int result = 0;
        kalloc_type* tmpKalloc = 0;

        tmpKalloc = new kalloc_type();
        FINISH_IF(tmpKalloc->kalloc_mech(sizeIn) == 0);

        SAFE_DEL(tmpKalloc);
    fail:
        return tmpKalloc;
    }

    kalloc_gen() : kva(0), backing{0}, net_sz(0) {};
    size_t operator*() const
    {
        return kva;
    }

};

#endif // MACH_MSG_KALLOC_H
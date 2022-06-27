#ifndef MACH_MSG_KALLOC_H
#define MACH_MSG_KALLOC_H

#include <stdint.h>

class kalloc_gen
{
private:
protected:
    char* backing;
    size_t net_sz;
    size_t kva;

    virtual int kalloc_mech(size_t allocSz) = 0;
public:
    // static kalloc_mm* kalloc(size_t allocSz)
    // {
    //     kalloc_mm* = 0;
    //     SAFE_BAIL(mach_msg_kalloc(allocSz) == -1);
    // fail:
        
    // }
    kalloc_gen(size_t allocSz)
    {
        kalloc_mech(allocSz);
    };
    kalloc_gen() : kva(0), backing{0}, net_sz(0) {};
    size_t operator*() const
    {
        return kva;
    }

};

#endif // MACH_MSG_KALLOC_H
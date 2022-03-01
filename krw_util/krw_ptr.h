#ifndef KRW_PTR_H
#define KRW_PTR_H

#include <string>
#include <stdio.h>
#include <kernel_block.h>
#include <kern_img.h>
#include <kern_dynamic.h>

extern kern_dynamic *g_kernblock;

// to be used for kernel pointers of primitive types
template <typename t>
class kern_ptr
{
private:
    size_t kern_address;
public:
    kern_ptr(size_t kernel_address_a) : kern_address(kernel_address_a) {};
//    Point& operator++();       // Prefix increment operator.
//    Point operator++(int);     // Postfix increment operator.
    kern_ptr& operator++()
    {
        kern_address += sizeof(t);
        return *this;
    }

    kern_ptr operator++(int)
    {
        kern_ptr ptrBkp = *this;
        kern_address += sizeof(t);
        return ptrBkp;
    }

    kern_ptr& operator--()
    {
        kern_address -= sizeof(t);
        return *this;
    }

    kern_ptr operator--(int)
    {
        kern_ptr ptrBkp = *this;
        kern_address -= sizeof(t);
        return ptrBkp;
    }

    t operator*() const
    {
        t tempObj;
        kRead(&tempObj, sizeof(tempObj), kern_address);
        return tempObj;
    }
};


class kern_struct
{
protected:
    size_t kern_address;
public:
    kern_struct(size_t new_address) : kern_address(new_address) {};
    
    template<typename t>
    t kread_member(std::string member_key)
    {
        size_t offset_local = 0;
        size_t new_kern_addr = 0;

        g_kernblock->kstruct_offset(member_key, &offset_local);
        
        kRead(&new_kern_addr, sizeof(new_kern_addr), kern_address + offset_local);
        return t(kern_address);
    }
    
    template<typename t>
    t kadd_member(std::string member_key)
    {
        size_t offset_local = 0;
        size_t new_kern_addr = 0;

        g_kernblock->kstruct_offset(member_key, &offset_local);

        kern_address += offset_local;

        return t(kern_address);
    }
    
    size_t operator*() const
    {
        return kern_address;
    }
};

// SABER is the constructor for each kern_struct inheritor
#define SABER(KSTRUCT_NAME, ...) \
class KSTRUCT_NAME : public kern_struct \
{ \
    using kern_struct::kern_struct; \
    __VA_ARGS__ \
    KSTRUCT_NAME operator[](const int location) \
    { \
        size_t offset_local = 0; \
        g_kernblock->kstruct_offset(# KSTRUCT_NAME ".size", &offset_local); \
        kern_address += location * offset_local; \
        return *this; \
    } \
    KSTRUCT_NAME& operator++() \
    { \
        size_t offset_local = 0; \
        g_kernblock->kstruct_offset(# KSTRUCT_NAME ".size", &offset_local); \
        kern_address += offset_local; \
        return *this; \
    } \
    KSTRUCT_NAME operator++(int) \
    { \
        KSTRUCT_NAME ptrBkp = *this; \
        size_t offset_local = 0; \
        g_kernblock->kstruct_offset(# KSTRUCT_NAME ".size", &offset_local); \
        kern_address += offset_local; \
        return ptrBkp; \
    } \
    KSTRUCT_NAME& operator--() \
    { \
        size_t offset_local = 0; \
        g_kernblock->kstruct_offset(# KSTRUCT_NAME ".size", &offset_local); \
        kern_address -= offset_local; \
        return *this; \
    } \
    KSTRUCT_NAME operator--(int) \
    { \
        KSTRUCT_NAME ptrBkp = *this; \
        size_t offset_local = 0; \
        g_kernblock->kstruct_offset(# KSTRUCT_NAME ".size", &offset_local); \
        kern_address -= offset_local; \
        return ptrBkp; \
    } \
};

#define RIDER(RET_TYPE, OFFSET_BASE, MEM_NAME) \
        RET_TYPE MEM_NAME() { return kread_member<RET_TYPE>( # OFFSET_BASE "." # MEM_NAME); };

#define CASTER

SABER(task_struct, RIDER(void*, task_struct, tasks));

#endif
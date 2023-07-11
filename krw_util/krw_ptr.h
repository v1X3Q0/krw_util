#ifndef KRW_PTR_H
#define KRW_PTR_H

#include <string>
#include <stdio.h>

#include <kernel_block.h>
#include <kern_img.h>

#ifndef __METALKIT__
#include <kern_dynamic.h>
#endif

#include <krw_util.h>

#ifdef __METALKIT__
extern kernel_metalkit *g_kernblock;
#else
extern kern_dynamic *g_kernblock;
#endif

// MORTIMERE is when the struct has a size component, then we can add size operators
#define MORTIMERE(KSTRUCT_NAME)                                            \
    KSTRUCT_NAME operator[](const int location)                            \
    {                                                                      \
        uint64_t offset_local = 0;                                           \
        g_kernblock->kstruct_offset(#KSTRUCT_NAME ".size", &offset_local); \
        kern_address += location * offset_local;                           \
        return *this;                                                      \
    }                                                                      \
    KSTRUCT_NAME &operator++()                                             \
    {                                                                      \
        uint64_t offset_local = 0;                                           \
        g_kernblock->kstruct_offset(#KSTRUCT_NAME ".size", &offset_local); \
        kern_address += offset_local;                                      \
        return *this;                                                      \
    }                                                                      \
    KSTRUCT_NAME operator++(int)                                           \
    {                                                                      \
        KSTRUCT_NAME ptrBkp = *this;                                       \
        uint64_t offset_local = 0;                                           \
        g_kernblock->kstruct_offset(#KSTRUCT_NAME ".size", &offset_local); \
        kern_address += offset_local;                                      \
        return ptrBkp;                                                     \
    }                                                                      \
    KSTRUCT_NAME &operator--()                                             \
    {                                                                      \
        uint64_t offset_local = 0;                                           \
        g_kernblock->kstruct_offset(#KSTRUCT_NAME ".size", &offset_local); \
        kern_address -= offset_local;                                      \
        return *this;                                                      \
    }                                                                      \
    KSTRUCT_NAME operator--(int)                                           \
    {                                                                      \
        KSTRUCT_NAME ptrBkp = *this;                                       \
        uint64_t offset_local = 0;                                           \
        g_kernblock->kstruct_offset(#KSTRUCT_NAME ".size", &offset_local); \
        kern_address -= offset_local;                                      \
        return ptrBkp;                                                     \
    }

// SABER is the constructor for each kern_struct inheritor
#define SABER(KSTRUCT_NAME, ...)            \
    class KSTRUCT_NAME : public kern_struct \
    {                                       \
        using kern_struct::kern_struct;     \
                                            \
    public:                                 \
        __VA_ARGS__                         \
    };

// berserker is the template variant of saber
#define BERSERKER(KSTRUCT_NAME, TYPE_NAME, ...) \
    template <TYPE_NAME>                        \
    SABER(KSTRUCT_NAME, __VA_ARGS__)

// rider retrieves the kernel prim at the address
#define RIDER(RET_TYPE, OFFSET_BASE, MEM_NAME) \
    RET_TYPE MEM_NAME() { return kread_member<RET_TYPE>(#OFFSET_BASE "." #MEM_NAME); };

// lancer is used as a getter for the offset of a type
#define LANCER(RET_TYPE, OFFSET_BASE, MEM_NAME) \
    RET_TYPE MEM_NAME() { return kadd_member<RET_TYPE>(#OFFSET_BASE "." #MEM_NAME); };

#define JEAN(RET_TYPE, OFFSET_BASE, MEM_NAME) \
    RET_TYPE MEM_NAME() { return kadd_member_save<RET_TYPE>(#OFFSET_BASE "." #MEM_NAME); };

#define ASTOLPHO(RET_TYPE, OFFSET_BASE, MEM_NAME) \
    RET_TYPE MEM_NAME() { return kread_member_save<RET_TYPE>(#OFFSET_BASE "." #MEM_NAME); };

// to be used for kernel pointers of primitive types
template <typename t>
class kern_ptr
{
private:
    size_t kern_address;

public:
    kern_ptr(size_t kernel_address_a) : kern_address(kernel_address_a){};
    //    Point& operator++();       // Prefix increment operator.
    //    Point operator++(int);     // Postfix increment operator.
    size_t get_kaddr() { return kern_address; };
    kern_ptr operator[](int index_delta)
    {
        kern_ptr<t> ptrBkp = 0;
        size_t new_address_out = 0;

        new_address_out = kern_address + (sizeof(t) * index_delta);
        ptrBkp = (kern_ptr<t>)(new_address_out);
        return ptrBkp;
    }

    kern_ptr &operator++()
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

    kern_ptr &operator--()
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

    size_t operator!() const
    {
        return get_kaddr();
    }

    t operator=(t other)
    {
        kWrite(&other, sizeof(t), kern_address);
        return other;
    }
};

template <typename bp_class>
class kern_bkptr
{
protected:
    size_t kern_address;
    size_t off_save;
public:
    kern_bkptr(size_t new_address, size_t off_save_a) : kern_address(new_address), off_save(off_save_a){};

    bp_class operator*() const
    {
        return bp_class(kern_address - off_save);
    }

    size_t operator!() const
    {
        return kern_address;
    }
};

class kern_struct
{
protected:
    size_t kern_address;
    size_t off_save;
    std::string class_name;

public:
    kern_struct(size_t new_address) : kern_address(new_address), off_save(0){};
    kern_struct(size_t new_address, size_t off_save_a) : kern_address(new_address), off_save(off_save_a){};

    // read a member whose offset is specified by key member_key
    template <typename t>
    t kread_member(std::string member_key)
    {
        uint64_t offset_local = 0;
        size_t new_kern_addr = 0;

        SAFE_PEXIT(g_kernblock->kstruct_offset(member_key, &offset_local) == -1, "symerror on symbol %s\n", member_key.data());

        kReadPtr(&new_kern_addr, sizeof(new_kern_addr), kern_address + offset_local);
        return t(new_kern_addr);
    }

    template <typename t>
    t kread_member_save(std::string member_key)
    {
        uint64_t offset_local = 0;
        size_t new_kern_addr = 0;

        SAFE_PEXIT(g_kernblock->kstruct_offset(member_key, &offset_local) == -1, "symerror on symbol %s\n", member_key.data());

        kReadPtr(&new_kern_addr, sizeof(new_kern_addr), kern_address + offset_local);
        return t(new_kern_addr, offset_local + off_save);
    }

    template <typename t>
    t kadd_member(std::string member_key)
    {
        uint64_t offset_local = 0;

        SAFE_PEXIT(g_kernblock->kstruct_offset(member_key, &offset_local) == -1, "symerror on symbol %s\n", member_key.data());

        return t(kern_address + offset_local);
    }

    template <typename t>
    t kadd_member_save(std::string member_key)
    {
        uint64_t offset_local = 0;
        size_t new_kern_addr = 0;

        SAFE_PEXIT(g_kernblock->kstruct_offset(member_key, &offset_local) == -1, "symerror on symbol %s\n", member_key.data());

        return t(kern_address + offset_local, offset_local + off_save);
    }

    size_t operator*() const
    {
        return kern_address;
    }

    bool operator==(const size_t &other) const
    {
        return kern_address == other;
    }

    bool operator!=(const size_t &other) const
    {
        return !(kern_address == other);
    }
};

// generic definitions for all operating systems
BERSERKER(list_entry, typename T,
    ASTOLPHO(T, list_entry<T>, next)
    ASTOLPHO(T, list_entry<T>, prev));

#endif

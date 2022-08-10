#ifndef PHYSMEM_H
#define PHYSMEM_H

#ifdef __cplusplus
extern "C"
{
#endif

size_t phystovirt(size_t arg1);
int kvtop(size_t kv, size_t* phys_addr);

#ifdef __cplusplus
}
#endif

#endif // PHYSMEM_H
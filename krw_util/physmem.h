#ifndef PHYSMEM_H
#define PHYSMEM_H

#ifdef __cplusplus
extern "C"
{
#endif

size_t phystovirt(size_t arg1);

#ifdef __cplusplus
}
#endif

#ifdef __METALKIT__
#ifdef __cplusplus
extern "C"
#endif // __cplusplus
#endif // __METALKIT__
// metalkit seems to need this definition?
int kvtop(size_t kv, size_t* phys_addr);

#endif // PHYSMEM_H
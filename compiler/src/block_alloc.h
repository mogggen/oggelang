#ifndef BLOCK_ALLOC_H
#define BLOCK_ALLOC_H

struct _MemBlock
{
    _MemBlock* next;
    char data[0];
};

struct BlockAlloc
{
    int block_size;
    int size;
    _MemBlock* first;
    _MemBlock* current;
};

BlockAlloc create_block_alloc(int size);
void* allocate(BlockAlloc& alloc, int size);

void dealloc(BlockAlloc& alloc);

template<class T>
T* allocate_assign(BlockAlloc& alloc, T obj)
{
    T* d = (T*)allocate(alloc, sizeof(T));
    memcpy(d, &obj, sizeof(T)); // memcpy to copy virtual table
    *d = obj; // call to copy constructor
    return d;
}

#endif

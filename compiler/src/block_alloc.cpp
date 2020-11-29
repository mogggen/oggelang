#include "block_alloc.h"

#include <stdlib.h>
#include <stdio.h>

BlockAlloc create_block_alloc(int size)
{
    BlockAlloc alloc;
    alloc.block_size = size;
    alloc.first = (_MemBlock*)malloc(alloc.block_size + sizeof(_MemBlock));
    alloc.first->next = nullptr;
    alloc.current = alloc.first;

    alloc.size = 0;

    return alloc;
}

void* allocate(BlockAlloc& alloc, int size)
{
    if( alloc.size + size <= alloc.block_size )
    {
        void* ret = (void*)(alloc.current->data+alloc.size);
        alloc.size += size;
        return ret;
    }
    else
    {
        alloc.block_size *= 2;
        if(alloc.block_size < size)
            alloc.block_size = size*2;

        alloc.current->next = (_MemBlock*)malloc(alloc.block_size + sizeof(_MemBlock));
        alloc.current = alloc.current->next; 
    
        alloc.current->next = nullptr;
        alloc.size = size;
    
        return (void*)alloc.current->data;
    }
}

void dealloc(BlockAlloc& alloc)
{
    _MemBlock* next = alloc.first;
    while(next != nullptr)
    {
        _MemBlock* t = next;
        next = next->next;
        free(t);
    }
    alloc.first = nullptr;
}

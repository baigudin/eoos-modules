/** 
 * The operating system kernel memory allocator.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2017, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef MODULE_ALLOCATOR_HPP_
#define MODULE_ALLOCATOR_HPP_

#include "api.Heap.hpp"

namespace module
{
    class Allocator
    {
    
    public:
    
        /**
         * Allocates memory.
         *
         * @param size number of bytes to allocate.
         * @return allocated memory address or a null pointer.
         */    
        static void* allocate(size_t size);
      
        /**
         * Frees an allocated memory.
         *
         * @param ptr address of allocated memory block or a null pointer.
         */      
        static void free(void* ptr);
        
        /**
         * Sets some heap memory.
         *
         * @param heap a constructed heap object.
         */
        static void setHeap(::api::Heap& heap);
      
    private:
      
        /**
         * Pointer to constructed heap memory (no boot).
         */
        static ::api::Heap* heap_;  
  
    };
}
#endif // MODULE_ALLOCATOR_HPP_

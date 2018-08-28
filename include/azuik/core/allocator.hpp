#ifndef AZUIK_CORE_ALLOCATOR_HPP
#define AZUIK_CORE_ALLOCATOR_HPP

#include <azuik/core/memory.hpp>
#include <cstdlib>
#include <cassert>
#include <new>

namespace azuik
{
    namespace core
    {
        template<class T,class A>
        allocator_pointer<T,A> allocate(A& a,allocator_size<T,A> n = 1)
        {
            return a.template allocate(n);
        }
        template<class T,class A>
        void deallocate(A& a,allocator_pointer<T,A> p,allocator_size<T,A> n = 1)
        {
            a.template deallocate(p,n);
        }
        
        struct allocator
        {
            using self_type = allocator;
            
            template<class T>
            allocator_pointer<T,self_type> allocate(allocator_size<T,self_type> n)
            {
                return static_cast<allocator_pointer<T,self_type> >(::std::malloc(sizeof(T)*n));
            }
            template<class T>
            void deallocate(allocator_pointer<T,self_type> p,allocator_size<T,self_type> )
            {
                ::std::free(p);
            }
#if 0
            void* expand(void* ptr,std::size_t bytes) noexcept
            {
                return _expand(ptr,bytes);
            }
#endif
        };
        
        template<class A = uint8_c<8> , class Enable = void>
        struct aligner;
        
        template<class A>
        struct aligner<A, enable_if<mpl::is_power2<A> > >
        {
        public:
            using self_type = aligner;
            using alignment_type = value_type<A>;
        public:
            template<class Size>
            static Size aligned_size(Size sz) noexcept
            {
                return sz + sizeof(alignment_type) + A::value -1;
            }
            static void* align(void* ptr)
            {
                alignment_type* aligned_ptr =
                reinterpret_cast<alignment_type*>(
                                                  reinterpret_cast<std::size_t>(
                                                                                (static_cast<unsigned char*>(ptr)+sizeof(alignment_type)+A::value-1)) & ~(A::value-1));
                *(aligned_ptr-1) = static_cast<alignment_type>(aligned_ptr - ptr);
                return aligned_ptr;
            }
            static void* unalign(void* ptr)
            {
                return static_cast<alignment_type*>(ptr)
                -  *(static_cast<alignment_type*>(ptr)-1);
            }
        };
        template<class Alloc>
        class aligned_allocator : private Alloc
        {
        public:
            template<class T>
            void* allocate(size_t sz,size_t alignment)
            {
                assert( !(alignment & (alignment-1)) && "improper alignment");
                size_t* p = static_cast<size_t*>(std::malloc(sz+alignment));
                if(nullptr == p)
                {
                    throw std::bad_alloc();
                }
               
                size_t offset = alignment - (size_t(p) & (alignment-1));
                size_t* result = p + offset;
                result[-1] = offset;
                return result;
            }
            template<class T>
            void dealloate(allocator_pointer<T,Alloc> p)
            {
                size_t* mem = static_cast<size_t*>(p);
                mem = mem - mem[-1];
                Alloc::deallocate(allocator_pointer<T,Alloc>(mem));
            }
        };
    }
}
#endif

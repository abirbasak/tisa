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
        template <class T, class A>
        struct allocator_traits {
            using value_type = T;
            using size_type = size_t;
            using difference_type = ptrdiff_t;
            using reference = T&;
            using const_reference = T const&;
            using pointer = T*;
            using const_pointer = T const*;
        };
        //define type functions
        template <class T, class A>
        using allocator_value = typename allocator_traits<T, A>::value_type;
        template <class T, class A>
        using allocator_size = typename allocator_traits<T, A>::size_type;
        template <class T, class A>
        using allocator_difference = typename allocator_traits<T, A>::difference_type;
        template <class T, class A>
        using allocator_reference = typename allocator_traits<T, A>::reference;
        template <class T, class A>
        using allocator_pointer = typename allocator_traits<T, A>::pointer;
        template <class T, class A>
        using allocator_const_reference = typename allocator_traits<T, A>::const_reference;
        template <class T, class A>
        using allocator_const_pointer = typename allocator_traits<T, A>::const_pointer;

        template <class T, class A>
        struct iterable_traits_from_allocator {
            using value_type = allocator_value<T, A>;
            using size_type = allocator_size<T, A>;
            using difference_type = allocator_difference<T, A>;
            using reference = allocator_reference<T, A>;
            using pointer = allocator_pointer<T, A>;
            using const_reference = allocator_const_reference<T, A>;
            using const_pointer = allocator_const_pointer<T, A>;
        };

        template <class S>
        struct iterable_traits_from_sequence {
            using value_type = core::value_type<S>;
            using size_type = core::size_type<S>;
            using difference_type = core::difference_type<S>;
            using reference = core::reference<S>;
            using pointer = core::pointer<S>;
            using const_reference = core::const_reference<S>;
            using const_pointer = core::const_pointer<S>;
        };

        template <class T, class A>
        allocator_pointer<T, A> allocate(A& a, allocator_size<T, A> n = 1)
        {
            return a.template allocate(n);
        }
        template <class T, class A>
        void deallocate(A& a, allocator_pointer<T, A> p, allocator_size<T, A> n = 1)
        {
            a.template deallocate(p, n);
        }

        struct allocator {
            using self_type = allocator;

            template <class T>
            allocator_pointer<T, self_type> allocate(allocator_size<T, self_type> n)
            {
                return static_cast<allocator_pointer<T, self_type>>(::std::malloc(sizeof(T) * n));
            }
            template <class T>
            allocator_pointer<T, self_type> allocate()
            {
                return static_cast<allocator_pointer<T, self_type>>(::std::malloc(sizeof(T)));
            }
            template <class T>
            void deallocate(allocator_pointer<T, self_type> p, allocator_size<T, self_type>)
            {
                ::std::free(p);
            }
            template <class T>
            void deallocate(allocator_pointer<T, self_type> p)
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

        template <class A = uint8_c<8>, class Enable = void>
        struct aligner;

        template <class A>
        struct aligner<A, enable_if<mpl::is_power2<A>>> {
        public:
            using self_type = aligner;
            using alignment_type = value_type<A>;

        public:
            template <class Size>
            static Size aligned_size(Size sz) noexcept
            {
                return sz + sizeof(alignment_type) + A::value - 1;
            }
            static void* align(void* ptr)
            {
                alignment_type* aligned_ptr = reinterpret_cast<alignment_type*>(
                    reinterpret_cast<std::size_t>(
                        (static_cast<unsigned char*>(ptr) + sizeof(alignment_type) + A::value - 1))
                    & ~(A::value - 1));
                *(aligned_ptr - 1) = static_cast<alignment_type>(aligned_ptr - ptr);
                return aligned_ptr;
            }
            static void* unalign(void* ptr)
            {
                return static_cast<alignment_type*>(ptr) - *(static_cast<alignment_type*>(ptr) - 1);
            }
        };
        template <class Alloc>
        class aligned_allocator : private Alloc {
        public:
            template <class T>
            void* allocate(size_t sz, size_t alignment)
            {
                assert(!(alignment & (alignment - 1)) && "improper alignment");
                size_t* p = static_cast<size_t*>(std::malloc(sz + alignment));
                if (nullptr == p)
                {
                    throw std::bad_alloc();
                }

                size_t offset = alignment - (size_t(p) & (alignment - 1));
                size_t* result = p + offset;
                result[-1] = offset;
                return result;
            }
            template <class T>
            void dealloate(allocator_pointer<T, Alloc> p)
            {
                size_t* mem = static_cast<size_t*>(p);
                mem = mem - mem[-1];
                Alloc::deallocate(allocator_pointer<T, Alloc>(mem));
            }
        };

        template <class T, class A>
        struct memory_guard {
        private:
            using alloc_ptr = A*;
            using value_ptr = core::allocator_pointer<T, A>;

        public:
            explicit memory_guard(A& alloc)
                : m_alloc{&alloc}
                , m_ptr{alloc.template allocate<T>()}
            {}
            template <class... Args>
            auto construct(Args&&... args) -> value_ptr
            {
                core::construct(m_ptr, static_cast<Args&&>(args)...);
                m_alloc = nullptr;
                return m_ptr;
            }
            ~memory_guard()
            {
                if (m_alloc)
                {
                    (*m_alloc).template deallocate<T>(m_ptr);
                }
            }

        private:
            alloc_ptr m_alloc;
            value_ptr m_ptr;
        };

        template <class T, class A, class... Args>
        auto constexpr allocate_and_construct(A& a, Args&&... args) -> core::allocator_pointer<T, A>
        {
            memory_guard<T, A> g{a};
            return g.construct(static_cast<Args&&>(args)...);
        }
    }; // namespace core

} // namespace azuik
#endif

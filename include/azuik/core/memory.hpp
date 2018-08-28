#ifndef AZUIK_CORE_MEMORY_HPP
#define AZUIK_CORE_MEMORY_HPP

#include <azuik/core/mpl.hpp>

namespace azuik
{
    namespace core
    {
        template <class T, class A>
        struct allocator_value_t : identity<T> {};
        template <class T, class A>
        struct allocator_size_t : identity<size_t> {};
        template <class T, class A>
        struct allocator_difference_t : identity<ptrdiff_t> {};

        template <class T, class A>
        struct allocator_reference_t : identity<T&> {};
        template <class T, class A>
        struct allocator_pointer_t : identity<T*> {};
        template <class T, class A>
        struct allocator_const_reference_t : identity<T const&> {};
        template <class T, class A>
        struct allocator_const_pointer_t : identity<T const*> {};
        template <class T, class A>
        using allocator_value = alias<allocator_value_t<T, A>>;
        template <class T, class A>
        using allocator_size = alias<allocator_size_t<T, A>>;
        template <class T, class A>
        using allocator_difference = alias<allocator_difference_t<T, A>>;
        template <class T, class A>
        using allocator_reference = alias<allocator_reference_t<T, A>>;
        template <class T, class A>
        using allocator_pointer = alias<allocator_pointer_t<T, A>>;
        template <class T, class A>
        using allocator_const_reference = alias<allocator_const_reference_t<T, A>>;
        template <class T, class A>
        using allocator_const_pointer = alias<allocator_const_pointer_t<T, A>>;

        struct for_each_iter_fn {
            template <class InIter, class Fn>
            void operator()(InIter f, InIter l, Fn fn) const noexcept
            {
                for (; f != l; ++f)
                {
                    fn(f);
                }
            }
        } const for_each_iter{};

        struct construct_fn {
            template <class T, class... Args>
            void operator()(T* p, Args&&... x) const
            {
                ::new (static_cast<void*>(p)) T(static_cast<Args&&>(x)...);
            }
        } const construct{};

        struct destroy_fn {
            template <class T>
            enable_if<is_trivially_destructible<T>, void> operator()(T* p) const
            {}
            template <class T>
            disable_if<is_trivially_destructible<T>, void> operator()(T* p) const
            {
                (*p).~T();
            }
        } const destroy{};

        struct destroy_range_fn {
            template <class InIter,
                      enable_if<core::is_trivially_destructible<core::value_type<InIter>>, int> = 0>
            void operator()(InIter, InIter) const noexcept
            {}
            template <class InIter,
                      core::disable_if<core::is_trivially_destructible<core::value_type<InIter>>,
                                       int> = 0>
            void operator()(InIter first, InIter last) const noexcept
            {
                using T = value_type<InIter>;
                for (; first != last; ++first)
                {
                    (*first).~T();
                }
            }
        } const destroy_range{};

        template <class T>
        class ref {
        public:
            using self_type = ref;

        private:
            using object_ptr = core::pointer<T>;

        public:
            ref(T&& x)
                : m_ptr(&x)
            {}
            template <class Slot, class... A>
            void operator()(Slot s, A&&... x)
            {
                (*m_ptr)(s, static_cast<A&&>(x)...);
            }
            template <class Slot, class... A>
            void operator()(Slot s, A&&... x) const
            {
                (*m_ptr)(s, static_cast<A&&>(x)...);
            }

        private:
            object_ptr m_ptr;
        };

    } // namespace core
} // namespace azuik
#endif

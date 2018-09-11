#ifndef AZUIK_CONTAINER_SET_HPP
#define AZUIK_CONTAINER_SET_HPP
#include <azuik/algorithm/functional.hpp>
#include <azuik/core/allocator.hpp>
#include <azuik/container/vector.hpp>
namespace azuik
{
    namespace core
    {
        template <class T, class Pr = less_fn, class A = allocator>
        class tree_set {
        private:
        };
        template <class T, class Pr = less_fn, class A = allocator>
        class tree_multiset {};

        template <class V, class Pr = less_fn>
        class linear_set;

        template <class V, class Pr>
        struct ordered_iterable_traits<linear_set<V, Pr>> : ordered_traits_from_sequence<V, Pr> {};
        template <class V, class Pr>
        struct sequence_traits<linear_set<V, Pr>> {
            using allocator_type = core::allocator_type<V>;
        };
        template <class V, class Pr>
        class linear_set : private assoc_vector<V, Pr> {
        private:
            using base_type = assoc_vector<V, Pr>;

        public:
            using self_type = linear_set;

            using allocator_type = core::allocator_type<self_type>;
            using value_type = core::value_type<self_type>;
            using difference_type = core::difference_type<self_type>;
            using size_type = core::size_type<self_type>;
            using reference = core::reference<self_type>;
            using const_reference = core::const_reference<self_type>;
            using pointer = core::pointer<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;
            using key_compare = core::key_cpmpare<self_type>;
            using key_type = core::key_type<self_type>;

        public:
            linear_set(value_compare const& comp = {})
                : value_compare{comp}
            {}
            template <class FwdIter>
            linear_set(FwdIter first, FwdIter last, value_compare const& comp = {})
                : value_compare{comp}
                , m_impl(first, last)
            {
                std::sort(first, last, get_comparator());
            }
            constexpr auto begin() noexcept -> iterator
            {
                return m_impl.begin();
            }
            constexpr auto end() noexcept -> iterator
            {
                return m_impl.end();
            }
            constexpr auto begin() const noexcept -> const_iterator
            {
                return m_impl.begin();
            }
            constexpr auto end() const noexcept -> const_iterator
            {
                return m_impl.end();
            }
            constexpr auto size() const noexcept -> size_type
            {
                return m_impl.size();
            }
            constexpr auto empty() const noexcept -> bool
            {
                return m_impl.empty();
            }
            constexpr auto capacity() const noexcept -> size_type
            {
                return m_impl.capacity();
            }
            template <class V>
            constexpr auto find(V&& v) noexcept -> iterator
            {
                return ordered_find(m_impl.begin(), m_impl.end(), static_cast<V&&>(x),
                                    get_comparator());
            }
            template <class V>
            constexpr auto find(V&& v) const noexcept -> const_iterator
            {
                return ordered_find(m_impl.begin(), m_impl.end(), static_cast<V&&>(x),
                                    get_comparator());
            }

        private:
            constexpr auto get_comparator() const noexcept -> value_compare const&
            {
                return static_cast<value_compare const&>(*this);
            }

        private:
            storage_type m_impl;
        };
        template <class T, class Pr = less_fn, class A = allocator>
        class linear_multiset {};
        template <class T, class Hash = hash_fn, class Eq = equal_fn, class A = allocator>
        class hash_set {};
        template <class T, class Hash = hash_fn, class Eq = equal_fn, class A = allocator>
        class hash_multiset {};
    } // namespace core
} // namespace azuik
#endif

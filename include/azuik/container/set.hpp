#ifndef AZUIK_CONTAINER_SET_HPP
#define AZUIK_CONTAINER_SET_HPP
#include <azuik/algorithm/functional.hpp>
#include <azuik/core/allocator.hpp>
#include <vector>
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
        template <class T, class Pr = less_fn, class A = allocator>
        class linear_set : private Pr {
        private:
            using storage_type = std::vector<T, A>;

        public:
            using self_type = linear_set;
            using value_compare = Pr;
            using allocator_type = storage_type::allocator_type;
            using value_type = storage_type::value_type;
            using difference_type = storage_type::difference_type;
            using size_type = storage_type::size_type;
            using reference = storage_type::reference;
            using const_reference = storage_type::const_reference;
            using pointer = storage_type::pointer;
            using const_pointer = storage_type::const_pointer;
            using iterator = storage_type::iterator;
            using const_iterator = storage_type::const_iterator;

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

#ifndef AZUIK_CONTAINER_VIEW_HPP
#define AZUIK_CONTAINER_VIEW_HPP

#include <azuik/container/iterable.hpp>
#include <cassert>
#include <algorithm>
#include <numeric>

namespace azuik
{
    namespace core
    {
        template <class T>
        struct is_view : false_c {};

        template <class S>
        class range_view;

        template <class S>
        struct is_view<range_view<S>> : true_c {};

        template <class S>
        class range_view {
        public:
            using self_type = range_view;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;
            using value_type = core::value_type<self_type>;
            using size_type = core::size_type<self_type>;
            using difference_type = core::difference_type<self_type>;
            using reference = core::reference<self_type>;
            using pointer = core::pointer<self_type>;
            using const_referene = core::const_reference<self_type>;
            using const_pointer = core::const_pointer<self_type>;

        public:
            constexpr range_view() noexcept
                : m_first{}
                , m_last{}
            {}
            constexpr explicit range_view(S& s) noexcept
                : m_first{std::begin(s)}
                , m_last{std::end(s)}
            {}
            constexpr range_view(iterator f, iterator l) noexcept
                : m_first{f}
                , m_last{l}
            {}

            constexpr auto begin() noexcept -> iterator
            {
                return iterator{m_first};
            }
            constexpr auto end() noexcept -> iterator
            {
                return iterator{m_last};
            }
            constexpr auto begin() const noexcept -> const_iterator
            {
                return const_iterator{m_first};
            }
            constexpr auto end() const noexcept -> const_iterator
            {
                return const_iterator{m_last};
            }
            constexpr auto size() const noexcept -> size_type
            {
                return m_last - m_first;
            }
            constexpr auto empty() const noexcept -> bool
            {
                return m_last == m_first;
            }

        private:
            iterator m_first;
            iterator m_last;
        };

    } // namespace core

} // namespace azuik

#endif

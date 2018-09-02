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
        template <class S>
        class range_view;

        template <class S>
        struct iterable_traits<range_view<S>> : iterable_traits<S> {};

        template <class S>
        class range_view {
        public:
            using self_type = range_view;
            using value_type = core::value_type<self_type>;
            using size_type = core::size_type<self_type>;
            using difference_type = core::difference_type<self_type>;
            using reference = core::reference<self_type>;
            using pointer = core::pointer<self_type>;
            using const_referene = core::const_reference<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;

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

        template <class V>
        class singular_view {
        public:
        private:
        };

        template <class S, class F>
        class transform_view {
        public:
            using function_type = F;
            using sequence_type = S;
            using value_type = F;
            using size_type = core::size_type<S>;
            using difference_type = core::difference_type<S>;

        public:
            explicit transform_view(sequence_type& s, function_type&& fn)
                : m_seq{&s}
                , m_fn(std::move(fn))
            {}
            constexpr auto get_function() noexcept -> function_type&
            {
                return m_fn;
            }
            constexpr auto get_function() const noexcept -> function_type const&
            {
                return m_fn;
            }
            constexpr auto begin() noexcept
            {
                return iterator{begin(*m_seq), m_fn};
            }
            constexpr auto end() noexcept
            {
                return iterator{end(*m_seq), m_fn};
            }
            constexpr auto begin() const noexcept
            {
                return const_iterator{begin(*m_seq), m_fn};
            }
            constexpr auto end() const noexcept
            {
                return const_iterator{end(*m_seq), m_fn};
            }
            constexpr auto empty() const noexcept
            {
                return is_empty(*m_seq);
            }
            constexpr auto size() const noexcept -> size_type
            {
                return size(*m_seq);
            }

        private:
            sequence_type* m_seq;
            function_type m_fn;
        };

    } // namespace core

} // namespace azuik

#endif

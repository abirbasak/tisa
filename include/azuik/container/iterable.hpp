#ifndef AZUIK_CONTAINER_ITERABLE_HPP
#define AZUIK_CONTAINER_ITERABLE_HPP
#include <azuik/core/mpl.hpp>
#include <azuik/container/iterator.hpp>

namespace azuik
{
    namespace core
    {
        struct iterable_tag {};
        struct forward_tag {};
        struct bidirectional_tag {};
        struct random_access_tag {};
        struct contiguous_tag {};

        template <class Fn>
        class iterable {};

        template <class C>
        struct unordered_iterable_traits : iterable_traits<C> {
            using key_type = typename C::key_type;
            using mapped_type = typename C::mapped_type;
            using hasher = typename C::hasher;
            using key_equal = typename C::key_equal;
        };

        AZUIK_SLOT(size, size);
        AZUIK_SLOT(empty, empty);
        AZUIK_SLOT(capcaity, capacity);

        AZUIK_SLOT(begin, begin);
        AZUIK_SLOT(end, end);

        AZUIK_SLOT(push_front, push_front);
        AZUIK_SLOT(push_back, push_back);

        AZUIK_SLOT(pop_front, pop_front);
        AZUIK_SLOT(pop_back, pop_back);

        AZUIK_SLOT(insert, insert);
        AZUIK_SLOT(erase, erase);

        AZUIK_SLOT(push, push);
        AZUIK_SLOT(pop, pop);

        inline static constexpr struct full_fn {
            template <class S>
            auto constexpr operator()(S&& s) const noexcept -> bool
            {
                return capacity(s) == size(s);
            }
        } const full = {};

        inline static constexpr struct cbegin_fn {
            template <class S>
            auto constexpr operator()(S const& s) const noexcept
            {
                return core::begin(s);
            }
        } const cbegin = {};

        inline static constexpr struct cend_fn {
            template <class S>
            auto constexpr operator()(S const& s) const noexcept
            {
                return core::end(s);
            }
        } const cend = {};

        inline static constexpr struct source_fn {
            template <class I>
            auto constexpr operator()(I&& i) const noexcept
            {
                return *i;
            }
        } const source = {};

        struct front_fn {
            template <class S>
            auto constexpr operator()(S&& s) const noexcept
            {
                return *begin(static_cast<S&&>(s));
            }
        };
        inline static constexpr const front_fn front = {};

        struct back_fn {
            template <class S>
            auto constexpr operator()(S&& s) const noexcept
            {
                return *(--end(static_cast<S&&>(s)));
            }
        };
        inline static constexpr const back_fn back = {};

    } // namespace core

} // namespace azuik
#endif

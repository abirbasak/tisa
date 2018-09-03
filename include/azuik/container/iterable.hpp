#ifndef AZUIK_CONTAINER_ITERABLE_HPP
#define AZUIK_CONTAINER_ITERABLE_HPP
#include <azuik/core/mpl.hpp>
#include <iterator>

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

        template <class S, class Policy>
        class standard_iterator : private Policy {
        private:
            using base_type = Policy;

        public:
            using self_type = standard_iterator;
            using other_type = standard_iterator<flip_const<S>, Policy>;
            using value_type = core::value_type<S>;
            using difference_type = core::difference_type<S>;
            using reference = core::reference<S>;
            using pointer = core::pointer<S>;
            using iterator_category = typename base_type::iterator_category;

        private:
            using nonconst_self = standard_iterator<remove_const<S>, Policy>;

        public:
            template <class... Args>
            constexpr standard_iterator(S& s, Args&&... args)
                : Policy{s, static_cast<Args&&>(args)...}
            {}
            explicit constexpr standard_iterator(nonconst_self that) noexcept
                : base_type{that}
            {}
            auto constexpr operator=(nonconst_self& that) noexcept -> self_type&
            {
                static_cast<base_type&>(*this) = that;
                return *this;
            }
            auto constexpr operator*() const noexcept
            {
                return base_type::deref();
            }
            auto constexpr operator++() noexcept
            {
                base_type::next();
                return *this;
            }
            auto constexpr operator--() noexcept
            {
                base_type::prev();
                return *this;
            }
            auto constexpr operator++(int) noexcept
            {
                self_type temp{*this};
                ++(*this);
                return temp;
            }
            auto constexpr operator--(int) noexcept
            {
                self_type temp{*this};
                --(*this);
                return temp;
            }
            auto constexpr operator+=(difference_type n) noexcept
            {
                base_type::increment(n);
                return *this;
            }
            auto constexpr operator-=(difference_type n) noexcept
            {
                base_type::decrement(n);
                return *this;
            }
            auto constexpr operator-(self_type const& that) const noexcept
            {
                return base_type::distance(that);
            }
            auto constexpr operator==(self_type const& that) const noexcept
            {
                return base_type::equal(that);
            }
            auto constexpr operator!=(self_type const& that) const noexcept
            {
                return !base_type::equal(that);
            }
            auto constexpr operator==(other_type const& that) const noexcept
            {
                return base_type::equal(that);
            }
            auto constexpr operator!=(other_type const& that) const noexcept
            {
                return !base_type::equal(that);
            }
        };

        namespace detail_
        {
            template <class S>
            struct forward_policy {
                using node_ptr = cond<is_const<S>, typename S::node_cptr, typename S::node_ptr>;
                using iterator_category = std::forward_iterator_tag;

                constexpr forward_policy(S& s, node_ptr ptr) noexcept
                    : m_ptr{ptr}
                {}
                constexpr auto deref() const noexcept
                {
                    return *m_ptr;
                }
                constexpr auto increment() noexcept
                {
                    m_ptr = (*m_ptr).next;
                }
                template <class That>
                constexpr auto equal(That const& that) const noexcept
                {
                    return m_ptr == that.m_ptr;
                }
                node_ptr m_ptr;
            };
            template <class S>
            struct bidirectional_policy {
                using node_ptr = cond<is_const<S>, typename S::node_cptr, typename S::node_ptr>;
                using iterator_category = std::bidirectional_iterator_tag;
                constexpr bidirectional_policy(S& s, node_ptr ptr) noexcept
                    : m_ptr{ptr}
                {}
                constexpr auto deref() const noexcept
                {
                    return *m_ptr;
                }
                constexpr auto increment() noexcept
                {
                    m_ptr = (*m_ptr).next;
                }
                constexpr auto decrement() noexcept
                {
                    m_ptr = (*m_ptr).prev;
                }
                template <class That>
                constexpr auto equal(That const& that) const noexcept
                {
                    return m_ptr == that.m_ptr;
                }
                node_ptr m_ptr;
            };
            template <class S>
            struct contiguous_policy {
                using iterator_category = std::random_access_iterator_tag;
                using difference_type = core::difference_type<S>;
                using node_ptr = cond<is_const<S>, typename S::node_cptr, typename S::node_ptr>;

            public:
                constexpr contiguous_policy(S& s, node_ptr ptr) noexcept
                    : m_ptr{ptr}
                {}
                constexpr auto increment() noexcept
                {
                    ++m_ptr;
                }
                constexpr auto decrement() noexcept
                {
                    --m_ptr;
                }
                constexpr auto increment(difference_type n) noexcept
                {
                    m_ptr += n;
                }
                constexpr auto decrement(difference_type n) noexcept
                {
                    m_ptr -= n;
                }
                template <class That>
                constexpr auto distance(That const& that) const noexcept
                {
                    return m_ptr - that.m_ptr;
                }
                constexpr auto deref() const noexcept
                {
                    return *m_ptr;
                }
                template <class That>
                constexpr auto equal(That const& that) const noexcept
                {
                    return m_ptr == that.m_ptr;
                }

            private:
                node_ptr m_ptr;
            };
        } // namespace detail_
        template <class S>
        using forward_iterator = standard_iterator<S, detail_::forward_policy<S>>;
        template <class S>
        using bidirectional_iterator = standard_iterator<S, detail_::bidirectional_policy<S>>;
        template <class S>
        using contiguous_iterator = standard_iterator<S, detail_::contiguous_policy<S>>;
    } // namespace core

} // namespace azuik
#endif

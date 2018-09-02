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
            explicit constexpr standard_iterator(nonconst_self that) noexcept
                : base_type{that}
            {}
            constexpr self_type& operator=(nonconst_self& that) noexcept
            {
                static_cast<base_type&>(*this) = that;
                return *this;
            }
            constexpr auto operator*() const noexcept
            {
                return base_type::deref();
            }
            constexpr auto operator++() noexcept
            {
                base_type::next();
                return *this;
            }
            constexpr auto operator--() noexcept
            {
                base_type::prev();
                return *this;
            }
            constexpr auto operator++(int) noexcept
            {
                self_type temp{*this};
                ++(*this);
                return temp;
            }
            constexpr auto operator--(int) noexcept
            {
                self_type temp{*this};
                --(*this);
                return temp;
            }
            constexpr auto operator==(self_type const& that) const noexcept
            {
                return base_type::equal(that);
            }
            constexpr auto operator!=(self_type const& that) const noexcept
            {
                return !base_type::equal(that);
            }
            constexpr auto operator==(other_type const& that) const noexcept
            {
                return base_type::equal(that);
            }
            constexpr auto operator!=(other_type const& that) const noexcept
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

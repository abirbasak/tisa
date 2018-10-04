#ifndef AZUIK_CONTAINER_ITERATOR_HPP
#define AZUIK_CONTAINER_ITERATOR_HPP
#include <azuik/core/mpl.hpp>
#include <iterator>
namespace azuik
{
    //iterator traits
    namespace core
    {
        using input_iterator_tag = std::input_iterator_tag;
        using forward_iterator_tag = std::forward_iterator_tag;
        using bidirectional_iterator_tag = std::bidirectional_iterator_tag;
        using random_access_iterator_tag = std::random_access_iterator_tag;

        struct contiguous_iterator_tag : virtual random_access_iterator_tag {};

        template <class I>
        struct iterator_traits {
            using difference_type = typename I::difference_type;
            using value_type = typename I::value_type;
            using reference = typename I::reference;
            using pointer = typename I::pointer;
            using iterator_category = typename I::iterator_category;
        };
        template <class T>
        struct iterator_traits<T*> {
            using difference_type = std::ptrdiff_t;
            using value_type = remove_const<T>;
            using reference = T&;
            using pointer = T*;
            using iterator_category = random_access_iterator_tag;
        };

        template <class I>
        using iterator_category = typename iterator_traits<I>::iterator_category;

    } // namespace core
    namespace core
    {
        template <class S, class Policy>
        class standard_iterator : public Policy {
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
            friend S;

        private:
            using nonconst_self = standard_iterator<remove_const<S>, Policy>;
            using base_type::get_node;

        private:
            template <class... Args>
            constexpr explicit standard_iterator(S& s, Args&&... args) noexcept
                : base_type{const_cast<remove_const<S>*>(&s), static_cast<Args&&>(args)...}
            {}

        public:
            constexpr standard_iterator() noexcept
                : base_type{nullptr, nullptr}
            {}

            constexpr standard_iterator(nonconst_self that) noexcept
                : base_type{static_cast<base_type&>(that)}
            {}
            auto constexpr operator=(nonconst_self const& that) noexcept -> self_type&
            {
                static_cast<base_type&>(*this) = static_cast<base_type const&>(that);
                return *this;
            }
            auto constexpr operator*() const noexcept -> reference
            {
                return base_type::deref();
            }
            auto constexpr operator++() noexcept -> standard_iterator&
            {
                base_type::increment();
                return *this;
            }
            auto constexpr operator--() noexcept -> standard_iterator&
            {
                base_type::decrement();
                return *this;
            }
            auto constexpr operator++(int) noexcept -> standard_iterator
            {
                self_type temp{*this};
                ++(*this);
                return temp;
            }
            auto constexpr operator--(int) noexcept -> standard_iterator
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
            auto constexpr operator-(difference_type n) const noexcept
            {
                return self_type{*this} -= n;
            }
            auto constexpr operator+(difference_type n) const noexcept
            {
                return self_type{*this} += n;
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
            class forward_policy {
            public:
                using node_ptr = typename S::node_ptr;
                using iterator_category = std::forward_iterator_tag;
                using reference = core::reference<S>;

                constexpr explicit forward_policy(S*, node_ptr ptr) noexcept
                    : m_ptr{ptr}
                {}

                constexpr auto deref() const noexcept -> reference
                {
                    return (*m_ptr).value();
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
                auto constexpr get_node() const noexcept -> node_ptr
                {
                    return m_ptr;
                }
                node_ptr m_ptr;
            };
            template <class S>
            class bidirectional_policy {
            public:
                using node_ptr = typename S::node_ptr;
                using iterator_category = std::bidirectional_iterator_tag;
                using reference = core::reference<S>;

                constexpr explicit bidirectional_policy(S*, node_ptr ptr) noexcept
                    : m_ptr{ptr}
                {}

                constexpr auto deref() const noexcept -> reference
                {
                    return (*m_ptr).value();
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
                auto constexpr get_node() const noexcept -> node_ptr
                {
                    return m_ptr;
                }
                node_ptr m_ptr;
            };
            template <class S>
            struct contiguous_policy {
                using iterator_category = std::random_access_iterator_tag;
                using difference_type = core::difference_type<S>;
                using node_ptr = typename S::node_ptr;
                using reference = core::reference<S>;

            public:
                constexpr explicit contiguous_policy(S* s, node_ptr ptr) noexcept
                    : m_ptr{ptr}
                {}
                constexpr auto deref() const noexcept -> reference
                {
                    return *m_ptr;
                }
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

                template <class That>
                constexpr auto equal(That const& that) const noexcept
                {
                    return m_ptr == that.m_ptr;
                }
                auto constexpr get_node() const noexcept -> node_ptr
                {
                    return m_ptr;
                }

            private:
                node_ptr m_ptr;
            };

            template <class S>
            struct indexed_policy {
            public:
                using iterator_category = std::random_access_iterator_tag;

            private:
                using sequence_type = S;
                using index_type = size_type<S>;
                using difference_type = core::difference_type<S>;

            private:
                auto constexpr increment() noexcept
                {
                    ++m_idx;
                }
                auto constexpr decrement() noexcept
                {
                    --m_idx;
                }
                auto constexpr increment(difference_type n) noexcept
                {
                    m_idx += n;
                }

                auto constexpr decrement(difference_type n) noexcept
                {
                    m_idx -= n;
                }
                template <class That>
                auto constexpr difference(That const& that) const noexcept
                {
                    assert(m_seq == that.m_seq && "comparing iterators from different container");
                    return m_idx - that.m_idx;
                }
                template <class That>
                auto constexpr equal(That const& that) const noexcept -> bool
                {
                    assert(m_seq == that.m_seq && "comparing iterators from different container");
                    return m_idx == that.m_idx;
                }
                auto constexpr deref() const noexcept
                {
                    return (*m_seq)[m_idx];
                }

            private:
                index_type m_idx;
                sequence_type* m_seq;
            };
        } // namespace detail_
        template <class S>
        using forward_iterator = standard_iterator<S, detail_::forward_policy<remove_const<S>>>;
        template <class S>
        using bidirectional_iterator =
            standard_iterator<S, detail_::bidirectional_policy<remove_const<S>>>;
        template <class S>
        using contiguous_iterator =
            standard_iterator<S, detail_::contiguous_policy<remove_const<S>>>;
        template <class S>
        using indexed_iterator = standard_iterator<S, detail_::indexed_policy<remove_const<S>>>;
    } // namespace core
} // namespace azuik
#endif

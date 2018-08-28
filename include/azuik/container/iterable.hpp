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

        //iterable type traits
        namespace detail
        {
            template <class S>
            struct iterator_t : identity<typename S::iterator> {};
            template <class S>
            struct iterator_t<S const> : identity<typename S::const_iterator> {};
        } // namespace detail
        template <class S>
        struct const_iterator_t : identity<typename remove_ref<S>::const_iterator> {};
        template <class S>
        using const_iterator = alias<const_iterator_t<S>>;

        template <class S>
        struct iterator_t : detail::iterator_t<remove_ref<S>> {};
        template <class S>
        using iterator = alias<iterator_t<S>>;

        template <class Fn>
        class iterable {};

        template <class S>
        class bidirectional_iterator {
            template <class>
            friend class bidirectional_iterator;

        public:
            using self_type = bidirectional_iterator;
            using const_self = bidirectional_iterator<add_const<S>>;
            using nonconst_self = bidirectional_iterator<remove_const<S>>;
            using value_type = value_type<S>;
            using difference_type = difference_type<S>;
            using reference = reference<S>;
            using pointer = pointer<S>;
            using iterator_category = bidirectional_tag;

        private:
            using node_ptr = typename S::node_ptr;

        private:
            static void increment(self_type& self)
            {
                self.m_ptr = (*self.m_ptr).next;
            }
            static void decrement(self_type& self)
            {
                self.m_ptr = (*self.m_ptr).prev;
            }
            static reference dereference(self_type& self)
            {
                return (*self.m_ptr).value;
            }

        private:
            node_ptr m_ptr;
        };

    } // namespace core

} // namespace azuik
#endif

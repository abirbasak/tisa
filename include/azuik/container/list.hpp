#ifndef AZUIK_CONTAINER_LIST_HPP
#define AZUIK_CONTAINER_LIST_HPP
#include <azuik/core/allocator.hpp>
#include <azuik/container/iterable.hpp>

namespace azuik
{
    namespace core
    {
        namespace detail_
        {
            struct forward_tag {};
            struct reverse_tag {};
            struct bidirectional_tag : virtual forward_tag, virtual reverse_tag {};

            template <class Ptr, class Tag>
            struct empty_node;

            template <class Ptr>
            struct empty_node<Ptr, forward_tag> {
            public:
                empty_node(Ptr, Ptr next)
                    : m_next{next}
                {}

            private:
                Ptr m_next;
            };

            template <class Ptr>
            struct empty_node<Ptr, reverse_tag> {
            public:
                empty_node(Ptr prev, Ptr)
                    : m_prev{prev}
                {}

            private:
                Ptr m_prev;
            };
            template <class Ptr>
            struct empty_node<Ptr, bidirectional_tag> {
            public:
                empty_node(Ptr prev, Ptr next)
                    : m_prev{prev}
                    , m_next{next}
                {}

            private:
                Ptr m_prev;
                Ptr m_next;
            };
            template <class T, class Tag>
            struct node : private empty_node<T*, Tag> {
            private:
                using base_type = empty_node<T*, Tag>;
                using value_type = T;

            public:
                template <class... Args>
                explicit node(T* prev, T* next, Args&&... args)
                    : base_type{prev, next}
                    , m_value{static_cast<Args&&>(args)...}
                {}
                constexpr auto value() noexcept
                {
                    return m_value;
                }
                constexpr auto value() const noexcept
                {
                    return m_value;
                }

            private:
                T m_value;
            };
        } // namespace detail_

        template <class T, class A>
        class list;

        template <class T, class A>
        struct allocator_type_t<list<T, A>> : identity<A> {};
        template <class T, class A>
        struct value_type_t<list<T, A>> : allocator_value_t<T, A> {};
        template <class T, class A>
        struct size_type_t<list<T, A>> : allocator_size_t<T, A> {};
        template <class T, class A>
        struct difference_type_t<list<T, A>> : allocator_difference_t<T, A> {};
        template <class T, class A>
        struct reference_t<list<T, A>> : allocator_reference<T, A> {};
        template <class T, class A>
        struct const_reference_t<list<T, A>> : allocator_const_reference_t<T, A> {};
        template <class T, class A>
        struct pointer_t<list<T, A>> : allocator_pointer_t<T, A> {};
        template <class T, class A>
        struct const_pointer_t<list<T, A>> : allocator_const_pointer_t<T, A> {};

        template <class T, class A>
        struct iterator_t<list<T, A>> : identity<bidirectional_iterator<list<T, A>>> {};

        template <class T, class A>
        class forward_list;

        template <class T, class A>
        class reversed_list;

        template <class T, class A>
        class forward_list {
        public:
            using self_type = forward_list;
            using allocator_type = core::allocator_type<self_type>;
            using value_type = core::value_type<self_type>;
            using size_type = core::size_type<self_type>;
            using difference_type = core::difference_type<self_type>;
            using pointer = core::pointer<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using reference = core::reference<self_type>;
            using const_reference = core::const_reference<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;

        private:
            struct node;
            using node_ptr = node*;
            struct empty_node {
                node_ptr next;
            };

            struct node : private empty_node {
                template <class... Args>
                explicit constexpr node(Args&&... args)
                    : m_value{static_cast<Args&&>(args)...}
                {}
                value_type m_value;
            };

        public:
            forward_list()
                : m_head{}
            {
                m_head.next = &m_head;
            }
            constexpr auto empty() const noexcept
            {
                return m_head.next == &m_head;
            }
            constexpr auto begin() noexcept
            {
                return m_head.next;
            }
            constexpr auto end() noexcept
            {
                return &m_head;
            }
            constexpr auto begin() const noexcept
            {
                return m_head.next;
            }
            constexpr auto end() const noexcept
            {
                return &m_head;
            }
            template <class... Args>
            void push_back(Args&&... args)
            {}

        private:
            empty_node m_head;
        };

        template <class T, class A>
        class reversed_list {
        public:
            using self_type = reversed_list;
            using allocator_type = core::allocator_type<self_type>;
            using value_type = core::value_type<self_type>;
            using size_type = core::size_type<self_type>;
            using difference_type = core::difference_type<self_type>;
            using pointer = core::pointer<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using reference = core::reference<self_type>;
            using const_reference = core::const_reference<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;
        };

        namespace detail_
        {
            template <class S>
            class bidirectional_iterator {};
        } // namespace detail_
        template <class T, class A>
        class list {
        public:
            using self_type = list;
            using allocator_type = core::allocator_type<self_type>;
            using value_type = core::value_type<self_type>;
            using size_type = core::size_type<self_type>;
            using pointer = core::pointer<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using reference = core::reference<self_type>;
            using const_reference = core::const_reference<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;

        private:
            struct node {};
            struct empty_node {};

        public:
            constexpr auto begin() noexcept {}

        private:
            empty_node m_head;
        };
    } // namespace core
} // namespace azuik
#endif
